/*Description
背景：在一条笔直的河流边部署了许多地面传感器，来监测河流及岸边环境。无人机需要沿着笔直的河岸飞行，并采集这些传感器的数据。
现在，需要你帮助确定无人机在这过程中的飞行速度（可以随时改变），使得在确保所有传感器的数据都能被完全采集的同时，最小化无人机的飞行能耗。

无人机在固定的高度飞行，且总飞行距离固定，为 D 米。将这条直线飞行路径用数轴表示，无人机从起点 x=0 飞向终点 x=D。沿途共部署了 n (<=1000) 个传感器，从 1 到 n 分别编号，它们需要将自身的数据上传至无人机。

传感器的传输距离并非无限远，无人机只有在某个传感器的传输范围以内才可以采集它的数据。
第 i 个传感器的传输范围是从 x=L_i 到 x=R_i 的区间，也可记为 (L_i, R_i)，其中 L_i<R_i 。另外，传感器之间的位置关系满足 0 <= L_1 < L_2 < ... < L_n < D 且 0 < R_1 < R_2 < ... < R_n = D。

采集数据需花费一定的时间，采集 传感器i 的数据需要花费 t_i 秒。无人机按照传感器编号顺序采集数据，即先采集 传感器1 的数据，再采集 传感器2 的数据……最后采集 传感器n 的数据。当完成 传感器i 的数据采集后，无人机才能开始对 传感器i+1 的数据采集。

多个传感器的传输范围可能存在重叠，但并不会相互包含。

另外，无人机在飞行时会消耗能量。当无人机以速度 v 飞行时，消耗的功率为 P(v) 。
例如，当无人机以 v=1 的速度从 x=9 飞至 x=18，消耗的能量为 P(1)*(18-9)/1。

P(v) = 0.37v^3 +0.136v^2 - 150v + 1160

注意：
我们既不允许无人机悬停，也不允许无人机反向飞行。例如，在上图中，在飞出 传感器1 的传输范围（飞过 x=R_1）之前，必须完成对 S1 的数据采集。

请你设计一个算法，能够输出无人机在从 x=0 飞到 x=D 过程中的速度方案，使无人机在能完整收集所有传感器信息的前提下，最小化飞行能耗。假如无人机飞行速度调整可以在瞬时完成并且不消耗额外的能量。
*/

// Input
// 第一行输入一个整数 T，表示有 T 组测试数据。
// 每组数据第一行为一个实数 D，表示终点（注：起点为0）；
// 第二行为一个整数 n，表示传感器数量；
// 接下来的 n 行，每行三个实数 L_i R_i t_i，分别表示第 i 个传感器的传输范围左端点、右端点、所需传输时间。

// Output
// 对于 T 组数据，每组输出无人机的飞行速度方案。每个方案包含三行，格式为：
// 第一行一个正整数 m；
// 第二行依次输出 m 个实数，x_1, x_2, ..., x_m，其中 x_1 < x_2 < ... < x_m；
// 第三行依次输出 m 个实数，v_1, v_2, ..., v_m；
// 表示无人机在(0, x_1)以速度v_1匀速飞行，在(x_{j-1}, x_j)以速度 v_j 匀速飞行。x_m必须等于D，否则将会被视为无效答案。
// 实数输出均保留六位小数。

// Sample Input
// 2
// 18
// 2
// 0 10 10
// 9 18 18
// 18
// 2
// 0 10 10
// 9 18 10

// Sample Output
// 2
// 9.000000 18.000000
// 0.900000 0.500000
// 1
// 18.000000
// 0.900000

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
using namespace std;

struct Sensor {
    double l, r, t;
};

struct Segment {
    double x, v;
};

void addSegment(vector<Segment>& ans, double x, double v) {
    const double EPS = 1e-9;
    if (!ans.empty() && fabs(ans.back().v - v) <= EPS) {
        ans.back().x = x;
    } else {
        ans.push_back({x, v});
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    cout << fixed << setprecision(6);

    while (T--) {
        double D;
        int n;
        cin >> D >> n;
        vector<Sensor> sensors(n);
        for (auto& s : sensors) cin >> s.l >> s.r >> s.t;

        vector<Segment> ans;
        double cur = 0.0;
        for (int i = 0; i < n; ++i) {
            double start = max(cur, sensors[i].l);
            if (start > cur) {
                addSegment(ans, start, 1.0);
                cur = start;
            }

            double end = (i + 1 < n) ? min(sensors[i].r, sensors[i + 1].l) : sensors[i].r;
            if (end <= cur) end = sensors[i].r;
            if (end <= cur) continue;

            double len = end - cur;
            double v = (sensors[i].t > 0.0) ? len / sensors[i].t : 1.0;
            addSegment(ans, end, v);
            cur = end;
        }

        if (cur < D) addSegment(ans, D, 1.0);
        if (ans.empty()) ans.push_back({D, 1.0});
        ans.back().x = D;

        cout << ans.size() << '\n';
        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i].x;
        }
        cout << '\n';
        for (int i = 0; i < (int)ans.size(); ++i) {
            if (i) cout << ' ';
            cout << ans[i].v;
        }
        cout << '\n';
    }
    return 0;
}
