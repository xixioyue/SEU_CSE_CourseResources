/*Description
背景：在一条笔直的河流边部署了许多地面传感器，来监测河流及岸边环境。无人机需要沿着笔直的河岸飞行，并采集这些传感器的数据。
现在，需要你帮助确定无人机在这过程中的飞行速度（可以随时改变），使得在确保所有传感器的数据都能被完全采集的同时，最小化无人机的飞行能耗。

无人机在固定的高度飞行，且总飞行距离固定，为D米。将这条直线飞行路径用数轴表示，无人机从起点x=0飞向终点x=D。
沿途共部署了n个传感器，从1到n分别编号，它们需要将自身的数据上传至无人机。

传感器的传输距离并非无限远，无人机只有在某个传感器的传输范围以内才可以采集它的数据。
第 i 个传感器的传输范围是从x=Li到x=Ri的区间，也可记为(Li, Ri)，0<=Li<Ri<=D。

多个传感器的传输范围可能存在重叠或包含的情况。
例如，图1中两个传输范围存在部分重叠，图2中传感器2的传输范围被传感器1的传输范围所包含。

尽管传输范围可能有重叠，但无人机在同一时刻只能采集一个传感器的数据。不过，你可以决定采集的顺序。
例如图1，当无人机在(L2,R1)之间飞行时，可以先采集传感器1的数据，再采集传感器2的数据；也可以先采集传感器2的数据，再采集传感器1的数据。但无论以何种顺序采集，都不允许超出对应传感器的传输范围。

注意：
（1）我们既不允许无人机悬停，也不允许无人机反向飞行。因此，在图1中，在飞出传感器1的传输范围（飞过x=R1）之前，必须完成对传感器1的数据采集。
（2）有的区域可能未被任何一个传感器的传输范围覆盖，但仍需要确定无人机在这段区间的飞行速度。

采集数据需花费一定的时间。无人机采集第 i 个传感器的数据需要花费 ti 秒。

另外，无人机在飞行时会消耗能量。当无人机以速度 v 飞行时，消耗的功率为P(v)。
例如，当无人机以 1 m/s 的速度从x=5飞至x=8（米），消耗的能量为 P(1)*(8-5)/1。

P(v)= 0.07v^3+0.0391v^2— 13.196v +390.95

请你设计一个算法，能够输出无人机在从x=0飞到x=D过程中的速度方案，使无人机在能完整收集所有传感器信息的前提下，最小化飞行能耗。假如无人机飞行速度调整可以在瞬时完成并且不消耗额外的能量。
*/

// Input
// 第一行输入一个整数T (T<=10)，表示有T组数据。
// 每组数据第一行为一个整数n (n<=150)，表示传感器数量；
// 第二行为一个实数D，表示终点，注：起点为0；
// 接下来的n行，每行3个实数，Li, Ri, ti，分别表示第 i 个传感器的传输范围左端点、右端点、所需传输时间。

// Output
// 对于T组数据，每组输出无人机的飞行速度方案。
// 每个方案包括三行：
// 第一行一个正整数m>=1，表示无人机在飞行过程中使用了的速度次数；
// 第二行从小到大依次输出m个实数x_1, x_2,..., x_m，分别表示无人机在x=x_j处改变了速度，其中x_m一定等于D，如果无人机全程匀速，则可以设置x_1=D，m=1。
// 第三行m个实数 v_1, ..., v_m，表示无人机在(0,x_1)区间以速度v_1飞行，在(x_{j-1},x_j)区间以速度v_j飞行。

// Sample Input
// 2
// 2
// 15
// 0 10 50
// 5 15 40
// 2
// 10
// 0 10 5
// 3 8 10

// Sample Output
// 1
// 15
// 0.1666667
// 3
// 3 8 10
// 1 0.5 1

// 说明：
// 第一组数据（参考图1）：
// 无人机全程以0.1666667 m/s的速度匀速飞行。
// 在(0,8.3333333)采集传感器1的数据，在(8.3333333, 15)采集传感器2的数据。
// 能耗 E = P(0.1666667) * 15 / 0.1666667

// 第二组数据（参考图2）：
// 无人机在(0,3)以1 m/s的速度飞行，在(3,8)以0.5 m/s的速度飞行，在(8,10)以1 m/s的速度飞行。
// 在(0,3)和(8,10)采集传感器1的数据，在(3,8)采集传感器2的数据。
// 能耗 E = P(1)*(3-0)/1 + P(0.5)*(8-3)/0.5 + P(1)*(10-8)/1

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

double P(double v) {
    return 0.07 * v * v * v + 0.0391 * v * v - 13.196 * v + 390.95;
}

struct Sensor {
    double l, r, t;
};

struct Segment {
    double x, v;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    cout.setf(ios::fixed);
    cout.precision(7);

    const double EPS = 1e-10;
    while (T--) {
        int n;
        double D;
        cin >> n >> D;
        vector<Sensor> sensors(n);
        vector<double> points{0.0, D};
        double total_time = 0.0;

        for (auto &s : sensors) {
            cin >> s.l >> s.r >> s.t;
            points.push_back(s.l);
            points.push_back(s.r);
            total_time += s.t;
        }
        
        sort(points.begin(), points.end());
        vector<double> xs;
        for (double x : points) {
            if (xs.empty() || fabs(xs.back() - x) > EPS) xs.push_back(x);
        }

        vector<Segment> ans;
        for (int i = 1; i < (int)xs.size(); ++i) {
            double left = xs[i - 1], right = xs[i];
            double len = right - left;
            if (len <= EPS) continue;

            bool covered = false;
            for (const auto &s : sensors) {
                if (left + EPS >= s.l && right <= s.r + EPS) {
                    covered = true;
                    break;
                }
            }

            double v = 1.0;
            if (covered && total_time > EPS) v = len / total_time;
            if (!ans.empty() && fabs(ans.back().v - v) <= 1e-9) {
                ans.back().x = right;
            } else {
                ans.push_back({right, v});
            }
        }

        if (ans.empty() || fabs(ans.back().x - D) > EPS) {
            if (!ans.empty() && fabs(ans.back().v - 1.0) <= 1e-9) {
                ans.back().x = D;
            } else {
                ans.push_back({D, 1.0});
            }
        }

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

