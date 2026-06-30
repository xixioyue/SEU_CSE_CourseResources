// Description
// 某国为了防御敌国的导弹袭击，发展出一种导弹拦截系统。但是这种导弹拦截系统有一个缺陷：虽然它的第一发炮弹能够到达任意的高度，但是以后每一发炮弹都不能高于前一发的高度。某天，雷达捕捉到敌国的导弹来袭。由于该系统还在试用阶段，所以只有一套系统，因此有可能不能拦截所有的导弹。

// Input
// 第一行输入M表示包含M组测试数据，每组第一个输入N (N<100)表示后面有N个整数，表示导弹依次飞来的高度(雷达给出的高度数据是不大于30000的正整数)。

// Output
// 对于每组输入数据，第一行输出这套系统最多能拦截多少导弹。

// Sample Input
// 2
// 7 300 250 275 252 200 138 245
// 7 181 205 471 782 1033 1058 1111

// Sample Output
// 5
// 1

/*
算法说明：动态规划类
①dp含义
dp[i]表示以第i枚导弹作为最后一枚时，最多能拦截的导弹数量，
也就是以i结尾的最长非上升子序列长度。

②dp转移方程
对所有j < i，如果a[j] >= a[i]，则第i枚导弹可以接在第j枚后面：
dp[i] = max(dp[i], dp[j] + 1)。

③dp初始值
每枚导弹都可以单独被拦截，所以dp[i] = 1。
最终答案为max(dp[i])。

④算法时间复杂度
双重循环枚举每个结尾及其之前元素，时间复杂度为O(n^2)，空间复杂度为O(n)。
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
        vector<int> a(n), dp(n, 1);
        for (int &x : a) cin >> x;

        int ans = 0;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < i; ++j) {
                if (a[j] >= a[i]) {
                    dp[i] = max(dp[i], dp[j] + 1);
                }
            }
            ans = max(ans, dp[i]);
        }
        cout << ans << '\n';
    }
    return 0;
}