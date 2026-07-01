/*Description   0/1背包问题
有一个容量为C(C<=100)的奇怪背包，这个背包可以被带走仅当它恰好被装满。现在你手边有N(N<=500)颗宝石，第i颗宝石大小为si，价值为vi。由于条件限制，你手边只有这个奇怪的背包可作为你搬运宝石的唯一工具。现在你想知道在这样的条件下你最多可以带走多大利润的宝石。  */

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据第一行输入N、C，表示宝石数目以及背包容量；接下来一行输入N组(si,vi), si和vi均为整数，表示每颗宝石的大小和价值。

// Output
// 输出M行正整数，第i行表示第i组数据可以带走的宝石的最大代价, 背包可被带走仅当它恰好被装满。

// Sample Input
// 3
// 3 10
// 1 3 2 5 7 2
// 3 10
// 1 3 2 5 6 2
// 5 10
// 5 6 5 7 2 8 8 1 5 9

// Sample Output
// 10
// 0
// 16

/*
算法说明：动态规划类
①dp含义
dp[j]表示在已经处理的宝石中，恰好装满容量j时能得到的最大价值。
若容量j无法恰好装满，则dp[j]为负无穷。

②dp转移方程
对每颗宝石(size=s, value=v)，容量倒序枚举：
if dp[j - s]有效:
    dp[j] = max(dp[j], dp[j - s] + v)
倒序枚举保证每颗宝石最多使用一次。

③dp初始值
dp[0] = 0，表示容量0可以用价值0恰好装满。
其他dp[j]初始化为负无穷，最终若dp[C]无效则输出0。

④算法时间复杂度
对每颗宝石枚举容量，时间复杂度为O(nC)，空间复杂度为O(C)。
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
    const int NEG = -1000000000;
    while (T--) {
        int n, c;
        cin >> n >> c;
        vector<int> dp(c + 1, NEG);
        dp[0] = 0;
        for (int i = 0; i < n; ++i) {
            int s, v;
            cin >> s >> v;
            for (int j = c; j >= s; --j) {
                if (dp[j - s] != NEG)  // 保证只有恰好装满背包时才更新 dp[c]
                    dp[j] = max(dp[j], dp[j - s] + v);
            }
        }
        cout << (dp[c] < 0 ? 0 : dp[c]) << '\n';
    }
    return 0;
}