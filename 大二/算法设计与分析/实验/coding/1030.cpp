/*Description - 黑白连线
给定直线上2n个点的序列P[1,2,… ,2n]，每个点P[i]要么是白点要么是黑点，其中共有n个白点和n个黑点，相邻两个点之间距离均为1，请设计一个算法将每个白点与一黑点相连，使得连线的总长度最小。

Input
第一行输入m表示有m组测试. 每组测试首先输入n(n<=10000)，接下来输入2n个0或者1, 分别表示白色或者黑色, 其中0和1的个数分别为n个.

Output
对每组测试数据输出最小总连线长度.

Sample Input
2
4 
1 1 0 1 0 0 0 1
4
0 0 1 0 1 1 1 0

Sample Output
8
8       */

/*
算法说明：贪心类
①算法思想
把所有白点位置和黑点位置分别按从左到右的顺序记录。
第i个白点与第i个黑点相连，可以避免连线交叉，并使总距离最小。

②伪代码
MinPairDistance(Zero, One):
    记录所有0的位置zero[]
    记录所有1的位置one[]
    ans = 0
    for i = 0 to n - 1:
        ans = ans + abs(zero[i] - one[i])
    return ans

③如何根据子问题最优确定总问题最优
若存在交叉配对，即白点a在白点b左侧，但黑点A在黑点B右侧，
交换为a-A、b-B后总距离不会增大。不断消除交叉后，最优解一定是按顺序配对。
因此每次匹配当前最靠左的未匹配白点和黑点，剩余部分仍是同样的最优子问题。
*/

#include <cstdlib>
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
        vector<int> zero, one;
        zero.reserve(n);
        one.reserve(n);
        for (int i = 1; i <= 2 * n; ++i) {
            int color;
            cin >> color;
            if (color == 0) {
                zero.push_back(i);
            } else {
                one.push_back(i);
            }
        }
        
        // 交叉配对不会比相邻配对更优
        long long ans = 0;
        for (int i = 0; i < n; ++i) ans += llabs((long long)zero[i] - one[i]);
        cout << ans << '\n';
    }
    return 0;
}