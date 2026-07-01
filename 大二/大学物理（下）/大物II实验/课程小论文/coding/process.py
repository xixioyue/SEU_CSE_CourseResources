# process.py
# 读取 waveforms.csv，计算互相关并用二次插值（抛物线拟合）做亚采样峰值修正，输出传播时延（s）。
# 可通过命令行指定采样率 fs 和传播距离（用于估算声速）。
import numpy as np
import argparse
import csv
import matplotlib.pyplot as plt
import os

def load_waveforms_csv(filename):
    data = np.loadtxt(filename, delimiter=",", skiprows=1)
    # columns: time, tx, rx
    t = data[:,0]
    tx = data[:,1]
    rx = data[:,2]
    return t, tx, rx

def cross_correlation_and_parabolic_refinement(tx, rx, fs):
    """
    计算互相关，找到峰值索引 m_max（correlation array 中的索引），
    使用相邻三个点做二次插值获得亚样点偏移 delta (samples)。
    返回：delay_seconds, corr, lags, peak_index_in_corr, delta_samples
    """
    # 互相关（full）
    corr = np.correlate(rx, tx, mode='full')
    N = len(tx)
    lags = np.arange(-N + 1, len(rx))
    # 找到峰值
    max_idx = np.argmax(np.abs(corr))
    # 如果峰值在边界无法插值直接返回
    if max_idx == 0 or max_idx == len(corr) - 1:
        delta = 0.0
    else:
        y_m1 = corr[max_idx - 1]
        y0 = corr[max_idx]
        y_p1 = corr[max_idx + 1]
        denom = (y_m1 - 2.0 * y0 + y_p1)
        if denom == 0:
            delta = 0.0
        else:
            # 常用二次插值公式（峰值偏移，相对于中心点）
            delta = 0.5 * (y_m1 - y_p1) / denom
            # delta 的符号：正值表示真实峰更偏向 m-1 方向（取决于定义），处理后我们直接加到 lags
    # 对应的精确延时（秒）
    lag_samples = lags[max_idx]
    corrected_lag_samples = lag_samples + delta
    delay_seconds = corrected_lag_samples / fs
    return delay_seconds, corr, lags, max_idx, delta

def parse_args():
    p = argparse.ArgumentParser(description="Process waveforms.csv with cross-correlation + parabolic peak refinement")
    p.add_argument("--infile", type=str, default="waveforms.csv", help="input CSV file")
    p.add_argument("--fs", type=float, required=True, help="sampling rate (Hz)")
    p.add_argument("--distance", type=float, default=None, help="distance (m) if you want estimated c printed")
    p.add_argument("--plot", action="store_true", help="show plots")
    return p.parse_args()

if __name__ == "__main__":
    args = parse_args()
    if not os.path.exists(args.infile):
        raise FileNotFoundError(f"{args.infile} not found")
    t, tx, rx = load_waveforms_csv(args.infile)
    fs = args.fs

    delay_seconds, corr, lags, peak_idx, delta = cross_correlation_and_parabolic_refinement(tx, rx, fs)

    print("互相关 + 二次插值结果：")
    print(f"  峰值在互相关序列索引 (corr index) = {peak_idx}")
    print(f"  整数样点滞后 (samples) = {lags[peak_idx]:.3f}")
    print(f"  二次插值修正偏移 (samples) = {delta:.6f}")
    print(f"  校正后滞后 (samples) = {lags[peak_idx] + delta:.6f}")
    print(f"  传播时延 (s) = {delay_seconds:.9e}")

    if args.distance is not None:
        if delay_seconds > 0:
            est_c = args.distance / delay_seconds
            print(f"  给定 distance = {args.distance} m -> 估计声速 c = {est_c:.3f} m/s")
        else:
            print("  延时非正，无法估计声速")

    if args.plot:
        import matplotlib.pyplot as plt
        # 绘制互相关并放大峰值附近用于显示插值
        lags_ms = lags / fs * 1000.0
        fig, ax = plt.subplots(2,1, figsize=(10,6), constrained_layout=True)
        ax[0].plot(lags_ms, corr, label='cross-correlation')
        ax[0].axvline((lags[peak_idx] + delta)/fs*1000.0, color='r', linestyle='--', label='refined peak (ms)')
        ax[0].set_xlabel('lag (ms)')
        ax[0].set_ylabel('corr')
        ax[0].legend()
        # 峰值附近局部放大并绘制拟合抛物线
        idx0 = peak_idx
        left = max(0, idx0 - 8)
        right = min(len(corr)-1, idx0 + 8)
        xs = np.arange(left, right+1)
        ys = corr[left:right+1]
        ax[1].plot(xs, ys, 'o-', label='corr samples')
        # 若可插值则画出抛物线拟合
        if idx0 > 0 and idx0 < len(corr)-1:
            y_m1 = corr[idx0-1]; y0 = corr[idx0]; y_p1 = corr[idx0+1]
            # 构建局部抛物线 f(x) = ax^2 + bx + c，x = -1,0,1 对应 y_m1,y0,y_p1
            # 解系数（解析）
            A = np.array([[1, -1, 1], [1, 0, 0], [1, 1, 1]], dtype=float)  # rows for x=-1,0,1: [x^0,x^1,x^2] but easier to fit
            Y = np.array([y_m1, y0, y_p1], dtype=float)
            # But simpler: treat local x = [-1,0,1], form Vandermonde for [a,b,c] where y = a x^2 + b x + c
            V = np.vstack((( -1 )**2, (-1), 1)).T  # wrong shape; just build properly
            # Build properly for three points:
            X_m = np.array([[-1.0**2, -1.0, 1.0],
                            [0.0**2, 0.0, 1.0],
                            [1.0**2, 1.0, 1.0]], dtype=float)
            # Solve for coefficients a,b,c such that Y = X_m @ [a,b,c]
            coefs = np.linalg.solve(X_m, np.array([y_m1, y0, y_p1]))
            a, b, c = coefs[0], coefs[1], coefs[2]
            # 生成细网格用于绘制抛物线
            xs_fine = np.linspace(idx0-1.0, idx0+1.0, 201)
            ys_parab = a * (xs_fine - idx0)**2 + b * (xs_fine - idx0) + (y0)  # but note we solved around x=-1..1 -> adjust
            # The above attempt is somewhat convoluted; simpler: compute parabola centered at idx0:
            # Instead we parametrize by delta x relative to center: d in [-1,1], y(d)=A d^2 + B d + C
            # Using y(-1)=y_m1, y(0)=y0, y(1)=y_p1 solved above: coefs_par = [A, B, C]
            A_par = a; B_par = b; C_par = c
            d_fine = np.linspace(-1.0, 1.0, 201)
            y_fit = A_par * d_fine**2 + B_par * d_fine + C_par
            ax[1].plot(idx0 + d_fine, y_fit, '-', color='orange', label='parabolic fit')
            # Mark refined peak
            refined_x = idx0 + delta
            refined_y = A_par * (delta**2) + B_par * (delta) + C_par
            ax[1].plot(refined_x, refined_y, 'r*', markersize=12, label='refined peak')
        ax[1].set_xlabel('corr index')
        ax[1].set_ylabel('corr value')
        ax[1].legend()
        plt.show()