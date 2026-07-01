// Description
// 两个矩阵A(r行s列)和B(s行t列)相乘, 乘法代价为r*s*t. 现给定N(N<=500)个矩阵连乘问题, 请计算最小乘法代价。

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据第一行输入N，表示矩阵个数；接下来一行输入N个矩阵的行数和列数。

// Output
// 输出M行正整数，第i行表示第i组数据的最小乘法代价。

// Sample Input
// 2
// 3
// 1 2 2 3 3 4
// 3
// 4 3 3 2 2 1 

// Sample Output
// 18
// 18

//动态规划 - 区间DP

/*
算法说明：动态规划类
①dp含义
dp[i][j]表示从第i个矩阵到第j个矩阵连乘所需的最小乘法代价。
dim[i]表示第i个矩阵的行数，dim[i+1]表示第i个矩阵的列数。

②dp转移方程
枚举最后一次断开的位置k：
dp[i][j] = min(dp[i][k] + dp[k + 1][j] + dim[i] * dim[k + 1] * dim[j + 1])
其中 i <= k < j。

③dp初始值
单个矩阵不需要乘法：
dp[i][i] = 0。
其他区间在计算前初始化为INF，按区间长度从小到大递推。

④算法时间复杂度
枚举区间长度、左端点和断点，时间复杂度为O(n^3)，空间复杂度为O(n^2)。
*/

#include <iostream>
#include <cstring>
using namespace std;

const int MAXN = 505;//最大矩阵个数
const int INF = INT_MAX;//无穷大

int dim[MAXN];//dim[i]表示第i个矩阵的行数, dim[i+1]表示第i个矩阵的列数，数组有效长度为n+1
int dp[MAXN][MAXN];//dp[i][j]表示从第i个矩阵到第j个矩阵连乘的最小代价

int main() {
    int T;
    if (cin >> T) {
        while (T--) {
            int n;
            cin >> n;
            for (int i = 0; i < n; ++i) {//输入矩阵的行数和列数，并存储在dim数组中
                int r, c;
                cin >> r >> c;
                if (i == 0) dim[0] = r;
                dim[i + 1] = c;
            }

            for (int i = 0; i < n; ++i) dp[i][i] = 0;//初始化dp数组，单个矩阵的乘法代价为0

            for (int len = 2; len <= n; ++len) {//枚举区间长度
                for (int l = 0; l <= n - len; ++l) {//遍历求该区间长度的区间dp
                    int r = l + len - 1;
                    dp[l][r] = INF;
                    for (int k = l; k < r; ++k) {
                        int cost = dp[l][k] + dp[k + 1][r] + dim[l] * dim[k + 1] * dim[r + 1];
                        if (cost < dp[l][r]) dp[l][r] = cost;
                    }
                }
            }
            cout << dp[0][n - 1] << '\n';
        }
    }
    return 0;
}