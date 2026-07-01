/*Description
海面上有一些船需要与陆地进行通信，需要在海岸线上布置一些基站。
现将问题抽象为，在x轴上方，给出n条船的坐标p1,p2,…,pn，其中pi=(xi,yi)，0≤yi≤d, 1≤i≤n，在x轴安放的基站可以覆盖半径为d的区域内的所有船只，问在x轴至少要安放几个基站才可以将x轴上方的船只都覆盖到。 

Input
第一行输入m表示有m组测试. 每组测试首先输入两个整数n(n<=10000)和d，接下来输入n个整数坐标(x,y)，其中0≤y≤d.

Output
对每组测试数据输出所需最少基站个数.

提示:
判断两点距离是否小于d可能需要考虑精度损失, 建议使用
(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) - d*d <= 1e-10
而非
(x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) <= d*d 

Sample Input
2
3 2
0 1
2 1
3 2

4 4
0 1
1 1
2 1
3 2

Sample Output
2
1       */

/*
算法说明：贪心类
①算法思想
每艘船对应x轴上一个可放置基站的区间[l, r]。
问题转化为用最少的点覆盖所有区间。将区间按右端点从小到大排序，
每次遇到尚未被覆盖的区间，就把基站放在该区间的右端点。

②伪代码
MinStations(Ships, d):
    for 每艘船(x, y):
        计算能覆盖它的基站区间[l, r]
    按区间右端点从小到大排序
    ans = 0, pos = -INF
    for 每个区间[l, r]:
        if l > pos:
            ans = ans + 1
            pos = r
    return ans

③如何根据子问题最优确定总问题最优
对当前右端点最小且尚未覆盖的区间，任何可行解都必须在它内部放一个基站。
把基站放在最右端不会减少对后续区间的覆盖能力，因此是局部最优且安全的选择。
选择后删除所有被覆盖区间，剩余仍是同类子问题，反复执行得到全局最优。
*/

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

struct Interval { // 区间
    double l, r;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        double d;
        cin >> n >> d;
        vector<Interval> seg; // 存储每个船只的覆盖区间
        seg.reserve(n);
        for (int i = 0; i < n; ++i) { // 计算每个船只的覆盖区间
            double x, y;
            cin >> x >> y;
            double dx = sqrt(max(0.0, d * d - y * y));
            seg.push_back({x - dx, x + dx});
        }

        sort(seg.begin(), seg.end(), [](const Interval& a, const Interval& b) { // 按照 右端点 从小到大 排序
            if (fabs(a.r - b.r) > 1e-10) return a.r < b.r;
            return a.l < b.l;
        });

        int ans = 0;
        // pos表示当前基站的位置
        double pos = -1e100;
        for (const auto& it : seg) {
            if (it.l - pos > 1e-10) { // 如果当前区间的左端点大于当前基站位置，则需要增加基站
                ++ans;
                pos = it.r;
            }
        }
        cout << ans << '\n';
    }
    return 0;
}