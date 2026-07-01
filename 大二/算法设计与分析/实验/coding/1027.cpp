// Description
// 给定n个活动，活动ai表示为一个三元组(si,fi,vi)，其中si表示活动开始时间，fi表示活动的结束时间，vi表示活动的权重, si<fi。带权活动选择问题是选择一些活动，使得任意被选择的两个活动ai和aj执行时间互不相交，即区间[si,fi)与[sj,fj)互不重叠，并且被选择的活动的权重和最大。
// 请设计一种方法求解带权活动选择问题。

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据输入整数N(N<=10000), 接下来输入N个活动。

// Output
// 输出M行正整数，第i行表示第i组数据的能够选择活动最大权值和。

// Sample Input
// 2
// 5
// 7 9 9
// 7 8 1
// 6 7 9
// 6 8 5
// 4 9 9
// 5
// 4 7 9
// 3 4 4
// 7 8 8
// 8 9 6
// 4 5 9

// Sample Output
// 18
// 27

/*
算法说明：动态规划类
①dp含义
先按活动结束时间排序。
dp[i]表示只考虑前i个活动时，能够取得的最大权值和。
p(i)表示第i个活动之前最后一个与它不冲突的活动编号。

②dp转移方程
对第i个活动有两种选择：
不选：dp[i - 1]
选：dp[p(i)] + v[i]
因此dp[i] = max(dp[i - 1], dp[p(i)] + v[i])。

③dp初始值
dp[0] = 0，表示没有活动时权值和为0。
活动按结束时间递推，p(i)通过二分查找得到。

④算法时间复杂度
活动排序为O(nlogn)，每个活动二分查找p(i)，总时间复杂度为O(nlogn)，空间复杂度为O(n)。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Activity {
    int s, f;
    long long v;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<Activity> a(n + 1);
        for (int i = 1; i <= n; ++i) cin >> a[i].s >> a[i].f >> a[i].v;
        sort(a.begin() + 1, a.end(), [](const Activity& x, const Activity& y) { // 按结束时间 / 开始时间排序
            if (x.f != y.f) return x.f < y.f;
            return x.s < y.s;
        });

        vector<int> finish(n + 1); // 第 i 个活动的结束时间
        for (int i = 1; i <= n; ++i) finish[i] = a[i].f;

        vector<long long> dp(n + 1, 0); // 前i个活动的最大权值和
        for (int i = 1; i <= n; ++i) {
            int p = int(upper_bound(finish.begin() + 1, finish.begin() + i, a[i].s) - finish.begin()) - 1; // 找到最近的不冲突活动的位置
            dp[i] = max(dp[i - 1], dp[p] + a[i].v); // 转移：dp[i] = dp[i-1] + v[i]  这里的max相当于最后再进行max{dp[i]}
        }
        cout << dp[n] << '\n';
    }
    return 0;
}