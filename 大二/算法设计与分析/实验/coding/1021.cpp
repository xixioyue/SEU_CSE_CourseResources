// Description
// 给定一根长度为n(n<=10000)的钢条以及一张价格表, 请计算这根钢条能卖出的最大总收益. 价格表表示为(li,pi), 1<=i<=k. 不在价格表中的钢条可卖出价格为0.

// Input
// 第一行输入m(m<=10)表示有M组数据。每组数据第一行输入两个int型整数n和k，分别表示钢条长度以及价格表中不同价格数量. 接下来一行输入k个价格的表示(li,pi), 均为整数, li可能大于n。

// Output
// 输出m行整数，第i行表示第i组数据的最大总收益。

// Sample Input
// 2
// 27 3
// 35 41 61 49 73 74
// 94 2
// 21 55 88 64 

// Sample Output
// 0
// 220

/*
算法说明：动态规划类
①dp含义
dp[len]表示长度为len的钢条可以获得的最大收益。

②dp转移方程
枚举最后切下的一段长度l及其价格p：
dp[len] = max(dp[len], dp[len - l] + p)
其中l <= len。由于同一种长度可以反复使用，这是完全背包形式。

③dp初始值
dp[0] = 0，其他长度初始为0。
不在价格表中的长度收益视为0，因此无法得到更高收益时保持0。

④算法时间复杂度
枚举钢条长度和价格表中的切割长度，时间复杂度为O(nk)，空间复杂度为O(n)。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, k;
        cin >> n >> k;
        vector<pair<int, int>> price;
        for (int i = 0; i < k; ++i) {
            int l, p;
            cin >> l >> p;
            if (l <= n) price.push_back({l, p});
        }

        vector<long long> dp(n + 1, 0);
        for (int len = 1; len <= n; ++len) {
            for (auto [l, p] : price) {
                if (l <= len) {
                    dp[len] = max(dp[len], dp[len - l] + p);
                }
            }
        }
        cout << dp[n] << '\n';
    }
    return 0;
}