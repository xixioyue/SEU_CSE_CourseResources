// Description
// 某国为了防御敌国的导弹袭击，发展出一种导弹拦截系统。但是这种导弹拦截系统有一个缺陷：虽然它的第一发炮弹能够到达任意的高度，但是以后每一发炮弹都不能高于前一发的高度。某天，雷达捕捉到敌国的导弹来袭。由于该系统还在试用阶段，所以只有一套系统，因此有可能不能拦截所有的导弹。

// Input
// 第一行输入M表示包含M组测试数据，每组第一个输入N (N<100)表示后面有N个整数，表示导弹依次飞来的高度(雷达给出的高度数据是不大于30000的正整数)。

// Output
// 对于每组输入数据，第一行输出这套系统最多能拦截多少导弹，以及输出如果要拦截所有导弹最少要配备多少套这种导弹拦截系统。

// Sample Input
// 2
// 7 300 250 275 252 200 138 245
// 7 181 205 471 782 1033 1058 1111

// Sample Output
// 5 2
// 1 7

/*
算法说明：动态规划类
①dp含义
dp[i]表示以第i枚导弹结尾时，一套系统最多能拦截的非上升子序列长度。
lis[i]表示以第i枚导弹结尾时的最长上升子序列长度，用于计算最少系统数。

②dp转移方程
若a[j] >= a[i]，则dp[i] = max(dp[i], dp[j] + 1)。
若a[j] < a[i]，则lis[i] = max(lis[i], lis[j] + 1)。

③dp初始值
每枚导弹单独作为一个序列时长度为1，因此dp[i] = 1，lis[i] = 1。
答案分别为所有dp[i]的最大值和所有lis[i]的最大值。

④算法时间复杂度
两次子序列动态规划都需要枚举i和j，时间复杂度为O(n^2)，空间复杂度为O(n)。
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
        int n;
        cin >> n;
        vector<int> a(n);
        for (int &x : a) cin >> x;

        vector<int> dp(n, 1);
        int longest_non_increasing = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (a[j] >= a[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            longest_non_increasing = max(longest_non_increasing, dp[i]);
        }

        vector<int> lis(n, 1);
        int systems = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (a[j] < a[i]) {
                    lis[i] = max(lis[i], lis[j] + 1);
                }
            }
            systems = max(systems, lis[i]);
        }

        cout << longest_non_increasing << ' ' << systems << '\n';
    }
    return 0;
}