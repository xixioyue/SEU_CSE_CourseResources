// Description
// 给出N个1-9的数字 (v1,v2,…,vN), 不改变它们的相对位置, 在中间加入K个乘号和N-K-1个加号, 括号随便加, 使最终结果最大。因为乘号和加号一共就是N-1个，所以恰好每两个相邻数字之间都有一个符号。
// 例如： N=5, K=2，5个数字分别为1、2、3、4、5，可以进行如下运算： 
// 1*2*(3+4+5)=24 
// 1*(2+3)*(4+5)=45 
// (1*2+3)*(4+5)=45
// 等等.

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据输入两整数N和K(N<=20, K<20), 接下来输入N个1-9的数字。

// Output
// 输出M行正整数，第i行表示第i组数据的最大结果, 你可能需要用long long类型存储结果。

// Sample Input
// 2
// 5 2
// 1 2 3 4 5 
// 6 3
// 1 2 3 4 5 6

// Sample Output
// 120
// 720

/*
算法说明：动态规划类
①dp含义
dp[i][j]表示前i个数字中放入j个乘号后能够得到的最大结果。
prefix用于快速计算某一段数字的和，因为两个乘号之间的数字用加法合并。

②dp转移方程
枚举最后一个乘号放在split之后：
dp[i][j] = max(dp[split][j - 1] * (prefix[i] - prefix[split]))
其中 j <= split < i。

③dp初始值
不放乘号时，前i个数字全部相加：
dp[i][0] = prefix[i]。
其他状态初始为负无穷，表示尚不可达。

④算法时间复杂度
枚举乘号数量、前缀长度和分割点，时间复杂度为O(kn^2)，空间复杂度为O(kn)。
*/

#include <algorithm>
#include <iostream>
#include <limits>
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
        // 由于两个乘号之间的数字使用括号必然会使结果更大，所以只需要考虑乘号如何分割
        vector<long long> prefix(n + 1, 0); // prefix[i] 表示前i个数字的和
        for (int i = 1; i <= n; ++i) {
            long long x;
            cin >> x;
            prefix[i] = prefix[i - 1] + x;
        }

        vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, numeric_limits<long long>::min())); // dp[i][j] 表示前i个数字，使用j个乘号的最大结果
        for (int i = 1; i <= n; ++i) dp[i][0] = prefix[i];

        for (int mul = 1; mul <= k; ++mul) { // 乘号数量
            for (int i = mul + 1; i <= n; ++i) { // 前i个数字
                for (int split = mul; split < i; ++split) { // 在位置split处分割，进行乘法
                    if (dp[split][mul - 1] == numeric_limits<long long>::min()) continue; // 子问题无解
                    long long part = prefix[i] - prefix[split];
                    dp[i][mul] = max(dp[i][mul], dp[split][mul - 1] * part);
                }
            }
        }

        cout << dp[n][k] << '\n';
    }
    return 0;
}