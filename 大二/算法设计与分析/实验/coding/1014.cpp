// Description
// 给定一个长度为N的int型数组a[0,1,2,...N-1], 请计算最大子数组和.

// Input
// 第一行输入M表示包含M组测试数据，每组先输入N (N<=50000), 接着输入N个int型整数.

// Output
// 输出最大子数组和.

// Sample Input
// 2
// 5 -1 -5 -2 -1 -3
// 5 2 -1 3 -2 4

// Sample Output
// -1
// 6

/*
算法说明：动态规划类
①dp含义
cur表示以当前元素结尾的最大子数组和。
best表示目前扫描过的所有位置中的最大子数组和。

②dp转移方程
对当前元素x：
cur = max(x, cur + x)
表示要么从当前元素重新开始，要么把当前元素接到前一个最优子数组后面。
best = max(best, cur)。

③dp初始值
读入第一个元素后，cur = best = 第一个元素。
这样可以正确处理数组全为负数的情况。

④算法时间复杂度
只需从左到右扫描一次数组，时间复杂度为O(n)，空间复杂度为O(1)。
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
        long long best, cur;
        cin >> cur;
        best = cur;
        for (int i = 1; i < n; ++i) {
            long long x;
            cin >> x;
            cur = max(x, cur + x);
            best = max(best, cur);
        }
        cout << best << '\n';
    }
    return 0;
}