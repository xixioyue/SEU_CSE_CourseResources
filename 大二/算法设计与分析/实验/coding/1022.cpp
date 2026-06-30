// Description
// 给定两个字符串A和B, 请计算这两人个字符串的最长公共子序列长度。

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据输入两行字符串, 字符串的长度不长于500。

// Output
// 输出M行正整数，第i行表示第i组数据的最长公共子序列长度。

// Sample Input
// 2
// abcdefg
// cemg
// abcdefgh
// ceaaegh

// Sample Output
// 3
// 4

/*
算法说明：动态规划类
①dp含义
二维写法中dp[i][j]表示A的前i个字符与B的前j个字符的最长公共子序列长度。
当前代码使用一维数组压缩，dp[j]对应当前行的dp[i][j]，prev保存旧的dp[i-1][j-1]。

②dp转移方程
若A[i-1] == B[j-1]：
dp[i][j] = dp[i-1][j-1] + 1
否则：
dp[i][j] = max(dp[i-1][j], dp[i][j-1])

③dp初始值
空串与任意字符串的LCS长度为0，
因此dp[0][j] = 0，dp[i][0] = 0；一维数组初始全为0。

④算法时间复杂度
设两个字符串长度分别为n和m，时间复杂度为O(nm)。当前一维压缩写法空间复杂度为O(m)。
*/

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        string a, b;
        cin >> a >> b;
        
        //二维数组解法
        // vector<vector<int>> dp(a.size() + 1, vector<int>(b.size() + 1, 0 ));
        // for (int i = 1; i <= (int)a.size(); ++i) {
        //     for (int j = 1; j <= (int)b.size(); ++j) {
        //         if (a[i - 1] == b[j - 1]) {
        //             dp[i][j] = dp[i - 1][j - 1] + 1;
        //         } else {
        //             dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        //         }
        //     }
        // }
        // cout << dp[a.size()][b.size()] << '\n';

        //一维数组解法
        //对照二维数组解法,prev存储dp[i-1][j-1]的值,
        //dp[j]存储dp[i-1][j]的值,dp[j-1]存储dp[i][j-1]的值
        vector<int> dp(b.size() + 1, 0);
        for (char ca : a) {
            int prev = 0;
            for (int j = 1; j <= (int)b.size(); ++j) {
                int saved = dp[j];
                if (ca == b[j - 1]) {
                    dp[j] = prev + 1;
                } else {
                    dp[j] = max(dp[j], dp[j - 1]);
                }
                prev = saved;
            }
        }
        cout << dp[b.size()] << '\n';
    }
    return 0;
}