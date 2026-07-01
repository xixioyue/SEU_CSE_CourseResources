/*Description   最优二叉搜索树
给定N个整数关键字, 每个关键字有一搜索概率, 关键字外区间(共N+1个区间)也有搜索概率.
可根据关键字构造二叉搜索树来减少搜索代价. 假定二叉搜索树中关键字节点的搜索代价为该节点到树的根的路径上关键字节点的个数, 关键字之外区间的搜索代价也为该区间到树的根的路径上关键字节点的个数. 
二叉搜索树的期望代价为所有关键字和关键字之外区间的期望代价之和. 
请构造一棵最优二叉搜索树, 计算最优二叉搜索树的期望代价。
*/

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据先输入N(N<=500), 表示N个关键字. 接下来
// 输入一行N个从小到大排好序的关键字, 一行N个关键字的搜索概率, 以及一行N+1个关键字外
// 区间的搜索概率。

// Output
// 输出M行正整数，第i行表示第i组数据的最优二叉搜索树的期望代价, 保留小数点后6位。

// Sample Input
// 2
// 2
// 10 20
// 0.1 0.3
// 0.2 0.2 0.2
// 3
// 10 20 30
// 0.1 0.2 0.3
// 0.1 0.1 0.1 0.1

// Sample Output
// 1.500000
// 1.800000

/*
算法说明：动态规划类
①dp含义
dp[i][j]表示关键字i到j构成一棵最优二叉搜索树时的最小期望代价。
weight(i,j)表示该区间内所有关键字概率和外部区间概率之和。

②dp转移方程
枚举root作为区间[i,j]的根：
dp[i][j] = min(dp[i][root - 1] + dp[root + 1][j] + weight(i,j))
其中 i <= root <= j。
选定根后，左右子树的所有节点深度都会增加1，因此需要加上weight(i,j)。

③dp初始值
空区间代价为0，即dp[i][i-1] = 0。
代码中二维数组初始为0，按区间长度从1到n递推。

④算法时间复杂度
枚举区间长度、左端点和根节点，时间复杂度为O(n^3)，空间复杂度为O(n^2)。
*/

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    cout << fixed << setprecision(6);
    while (T--) {
        int n;
        cin >> n;

        vector<int> key(n + 1);
        for (int i = 1; i <= n; ++i) cin >> key[i]; // 关键字

        vector<double> p(n + 1), q(n + 1);
        for (int i = 1; i <= n; ++i) cin >> p[i];   // 关键字搜索概率
        for (int i = 0; i <= n; ++i) cin >> q[i];   // 关键字外区间搜索概率

        vector<double> prefix_p(n + 1, 0.0), prefix_q(n + 1, 0.0);
        for (int i = 1; i <= n; ++i) prefix_p[i] = prefix_p[i - 1] + p[i]; // 关键字搜索概率前缀和
        prefix_q[0] = q[0];
        for (int i = 1; i <= n; ++i) prefix_q[i] = prefix_q[i - 1] + q[i];  // 关键字外区间搜索概率前缀和
        
        // dp[i][j]表示关键字i到j的最优二叉搜索树的期望代价
        // dp[i][j] = min{ dp[i][k-1] + dp[k+1][j] + weight(i,j) } for k in [i,j]
        vector<vector<double>> dp(n + 2, vector<double>(n + 1, 0.0));
        for (int len = 1; len <= n; ++len) {
            for (int l = 1; l + len - 1 <= n; ++l) {
                int r = l + len - 1;
                double weight = (prefix_p[r] - prefix_p[l - 1]) + (prefix_q[r] - (l >= 2 ? prefix_q[l - 2] : 0.0)); 
                dp[l][r] = numeric_limits<double>::infinity();
                for (int root = l; root <= r; ++root) {
                    dp[l][r] = min(dp[l][r], dp[l][root - 1] + dp[root + 1][r] + weight);
                }
            }
        }
        cout << dp[1][n] << '\n';
    }
    return 0;
}