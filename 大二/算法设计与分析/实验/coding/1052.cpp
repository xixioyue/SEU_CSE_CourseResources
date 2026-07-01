// Description
// 学校准备在文化长廊摆放一排装饰摆件，总共需要摆放m件。现有n种不同样式的摆件，每种摆件数量有限：第i种摆件最多只有ai件。
// 摆放要求：
//     相同样式的摆伯须放在一起，不能分开；
//     所有摆件须按编号从小到大的顺序依次摆放；
//     最终摆放的摆件总数刚好为m件；
//     每种摆件可以选择不用（摆放0件）。
// 请您计算一共有多少种不同的摆放方案。

// Input
// 第一行输入一个整数t，表示t组测试数据。
// 后面每一行先输入2个整数n和m，分别表示n种不同的摆件以及须摆放的总件数；之后输入n个整数a1,a2,...,an分别表示每种摆件的最大可用数量。

// Output
// 输出t行, 表示不同摆放方案总数。因为方案数可能很多，请输出方案数对1000007取模的结果。

// Sample Input
// 1
// 2 4 3 2

// Sample Output
// 2

// 动态规划
// dp[i][j] 表示使用前i种摆件摆放j件的方案数
// 状态转移方程：dp[i][j] = sum(dp[i-1][j-k])，其中k从0到min(a[i], j)
#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1000007;

int main() {
    int t;
    cin >> t;
    while(t--){
        int n, m;
        cin >> n >> m;
        vector<int> a(n); // 每种摆件的最大可用数量
        for(int i = 0; i < n; i++){
            cin >> a[i];
        }
        
        vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
        dp[0][0] = 1;
        for(int i = 1; i <= n; i++){
            for(int j = 0; j <= m; j++){
                int k_max = min(a[i - 1], j);
                for(int k = 0; k <= k_max; k++){
                    dp[i][j] = (dp[i][j] + dp[i-1][j-k]) % MOD;
                }
            }
        }
        cout << dp[n][m] << endl;
    }
}