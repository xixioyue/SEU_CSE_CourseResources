import argparse
import numpy as np
import matplotlib.pyplot as plt
import os

def make_burst(frequency, fs, n_cycles=10, start_time=0.002, amplitude=1.0):
    """
    生成一个窄带正弦脉冲（有平滑窗的若干个周期）。
    返回：start_time, tb, burst
    - start_time: 脉冲在总体时间轴上的起始时间 (s)
    - tb: 脉冲短段的本地时间向量 (s)，从 0 开始
    - burst: 脉冲波形（carrier * window）
    """
    t0 = start_time
    duration = n_cycles / frequency
    N = int(np.ceil(duration * fs))
    if N < 3:
        # 最少取 3 个点以便窗函数有效
        N = 3
    tb = np.arange(N) / fs
    carrier = np.sin(2 * np.pi * frequency * tb)
    env = np.hanning(N)
    burst = amplitude * carrier * env
    return t0, tb, burst

def synthesize_signals(frequency=40000.0, fs=500000.0, duration=0.01,
                       distance=0.50, c=343.0, noise_std=0.02,
                       n_cycles=10, attenuation_func=None, tx_start_time=0.001):
    """
    合成发射(tx)和接收(rx)信号（仅时域波形），并返回启动时间节点。
    - frequency: 正弦频率 (Hz)
    - fs: 采样率 (Hz)
    - duration: 总信号时长 (s)
    - distance: 发射-接收距离 (m)
    - c: 声速 (m/s)
    - noise_std: 高斯噪声标准差（叠加到接收信号）
    - n_cycles: 发射脉冲的周期数
    - attenuation_func: optional(func(distance)->factor)
    - tx_start_time: 发射脉冲的起始时间 (s)
    返回：
      t (s 数组), tx (数组), rx (数组),
      true_delay (s), tx_start_time (s), rx_start_time (s)
    """
    # 总时间向量
    t = np.arange(0, duration, 1.0 / fs)

    tx = np.zeros_like(t)
    rx = np.zeros_like(t)

    # 构造发射脉冲
    t0, tb, burst = make_burst(frequency, fs, n_cycles=n_cycles, start_time=tx_start_time, amplitude=1.0)
    tx_start_idx = int(np.round(t0 * fs))
    end_idx = tx_start_idx + len(burst)
    if tx_start_idx < 0:
        raise ValueError("tx_start_time 导致起始索引 < 0")
    if tx_start_idx >= len(t):
        raise ValueError("tx_start_time 超出总持续时间，请增大 duration 或减小 tx_start_time")
    # 截断以防越界
    actual_len = min(len(burst), max(0, len(t) - tx_start_idx))
    if actual_len > 0:
        tx[tx_start_idx:tx_start_idx + actual_len] = burst[:actual_len]

    # 传播时延（单程）
    true_delay = distance / c
    rx_start_time = tx_start_time + true_delay
    rx_start_idx = rx_start_time * fs  # 可能非整数

    # 衰减函数（默认：1/(1 + 4*d)），可替换
    if attenuation_func is None:
        def attenuation_func(d):
            return 1.0 / (1.0 + 4.0 * d)
    att = attenuation_func(distance)

    # 为了支持非整数样点延时，我们对接收信号使用带窗的载波直接在 t 上评估相位与窗：
    # 对于 t_i，当 (t_i - rx_start_time) 在 [0, duration_burst) 时，rx(t_i) = att * sin(2πf*(t_i - rx_start_time)) * env( (t_i - rx_start_time) )
    # 先构造 burst 的 envelope 长度与采样点数
    burst_len = len(tb)
    burst_duration = burst_len / fs

    # 构造窗函数（与 burst 中使用的相同）
    env_full = np.hanning(burst_len)

    # 计算接收信号：对满足接收时间窗口的 t 索引计算
    # 找到 t 中满足条件的索引范围（可能是小数，处理为索引并用相位计算）
    t_rel = t - rx_start_time  # 相对于接收起始时刻的时间
    # 确定有效样点 mask
    mask = (t_rel >= 0.0) & (t_rel < burst_duration)
    if np.any(mask):
        # 在有效点上评估载波与包络（包络使用线性插值到相对时间位置）
        # 计算相对样点对应的 burst 本地索引（可能非整数）
        local_idx = t_rel[mask] * fs  # 以样点为单位（从 0 到 burst_len）
        # 载波相位计算（不受本地索引舍入影响）
        carrier_vals = np.sin(2.0 * np.pi * frequency * t_rel[mask])
        # envelope 通过在本地索引处插值得到
        # 为简单且高精度：对 env_full 使用 np.interp（在 [0, burst_len-1] 上）
        env_vals = np.interp(local_idx, np.arange(burst_len), env_full)
        rx[mask] = att * carrier_vals * env_vals

    # 添加高斯噪声
    rng = np.random.default_rng()
    noise = rng.normal(loc=0.0, scale=noise_std, size=t.shape)
    rx = rx + noise

    return t, tx, rx, true_delay, tx_start_time, rx_start_time

def save_waveforms_csv(filename, time_array, tx_array, rx_array,
                        time_fmt='%.12f', amp_fmt='%.6f'):
    """
    将三列 (time, tx, rx) 保存为 CSV 文件，使用指定格式防止科学计数法显示。
    - filename: 输出文件名（.csv）
    - time_array: 时间数组 (s)
    - tx_array, rx_array: 信号数组
    - time_fmt, amp_fmt: numpy.savetxt 的格式字符串
    """
    # 为 numpy.savetxt 指定每列格式
    fmt = [time_fmt, amp_fmt, amp_fmt]
    data = np.column_stack([time_array, tx_array, rx_array])
    header = "time_s,tx,rx"
    # 使用 newline='\n' 以兼容 Windows/Unix
    np.savetxt(filename, data, delimiter=",", header=header, comments='', fmt=fmt, newline='\n')
    print(f"已保存 CSV 文件: {filename} （列：{header}）")

def demo_and_export(frequency=40000.0, noise_std=0.02, fs=500000.0,
                    distance=0.50, c=343.0, duration=0.01, n_cycles=10,
                    out_csv="waveforms.csv", plot=False):
    """
    运行模拟，打印发射/接收开始时间，导出 CSV（列：time_s, tx, rx）。
    time_s 的单位为秒，且在 CSV 中以小数格式显示（避免科学记数法）。
    tx 和 rx 在 CSV 中以小数格式显示（6 位小数）。
    """
    t, tx, rx, true_delay, tx_start_time, rx_start_time = synthesize_signals(
        frequency=frequency, fs=fs, duration=duration,
        distance=distance, c=c, noise_std=noise_std, n_cycles=n_cycles, tx_start_time=0.001
    )

    # 打印关键信息（以秒显示，及以毫秒便于观察）
    print("===== 模拟参数 =====")
    print(f"frequency = {frequency} Hz, fs = {fs} Hz, duration = {duration} s")
    print(f"distance = {distance} m, sound speed c = {c} m/s")
    print(f"tx start time = {tx_start_time:.9f} s ({tx_start_time*1000.0:.6f} ms)")
    print(f"true travel delay = {true_delay:.9f} s ({true_delay*1000.0:.6f} ms)")
    print(f"rx expected start time = {rx_start_time:.9f} s ({rx_start_time*1000.0:.6f} ms)")
    print("=====================")

    # 导出 CSV：为了在普通表格软件中能"正常显示"，我们避免科学记数法：
    # time_s 使用 12 位小数（秒），tx/rx 使用 6 位小数
    save_waveforms_csv(out_csv, t, tx, rx, time_fmt='%.12f', amp_fmt='%.6f')

    if plot:
        # 绘图展示（默认展示前 5 ms）
        tview_end = min(0.005, duration)
        view_idx = t <= tview_end
        fig, axs = plt.subplots(2, 1, figsize=(10, 6), constrained_layout=True)
        axs[0].plot(t[view_idx] * 1000.0, tx[view_idx], label='tx (发射)')
        axs[0].axvline(tx_start_time * 1000.0, color='k', linestyle='--', label='tx start')
        axs[0].set_ylabel('Amplitude')
        axs[0].set_title('Transmit pulse (前 %.2f ms)' % (tview_end * 1000.0))
        axs[0].legend()
        axs[1].plot(t[view_idx] * 1000.0, rx[view_idx], label='rx (接收, 含噪声)')
        axs[1].axvline(rx_start_time * 1000.0, color='k', linestyle='--', label='true arrival')
        axs[1].set_xlabel('Time (ms)')
        axs[1].set_ylabel('Amplitude')
        axs[1].set_title('Received signal (前 %.2f ms)' % (tview_end * 1000.0))
        axs[1].legend()
        plt.show()

def parse_args():
    p = argparse.ArgumentParser(description="发射-接收波形模拟（时差法）并导出 CSV")
    p.add_argument("--frequency", type=float, default=40000.0, help="脉冲频率 (Hz)")
    p.add_argument("--fs", type=float, default=500000.0, help="采样率 (Hz)")
    p.add_argument("--duration", type=float, default=0.01, help="总信号时长 (s)")
    p.add_argument("--distance", type=float, default=0.50, help="发射-接收距离 (m)")
    p.add_argument("--c", type=float, default=343.0, help="声速 (m/s)")
    p.add_argument("--noise", type=float, default=0.02, help="接收信号高斯噪声标准差")
    p.add_argument("--ncycles", type=int, default=10, help="脉冲周期数")
    p.add_argument("--out", type=str, default="waveforms.csv", help="输出 CSV 文件名")
    p.add_argument("--plot", action="store_true", help="是否绘图显示（仅示意）")
    return p.parse_args()

if __name__ == "__main__":
    args = parse_args()

    # 确保输出目录存在
    out_dir = os.path.dirname(os.path.abspath(args.out))
    if out_dir and not os.path.exists(out_dir):
        os.makedirs(out_dir, exist_ok=True)

    demo_and_export(frequency=args.frequency,
                    noise_std=args.noise,
                    fs=args.fs,
                    distance=args.distance,
                    c=args.c,
                    duration=args.duration,
                    n_cycles=args.ncycles,
                    out_csv=args.out,
                    plot=args.plot)