// Description
// 给定一个长度为N的整数数组, 请计算该数组中最长非降了序列长度。

// Input
// 第一行输入M(M<=10)表示有M组数据。每组数据输入N(N<=10000), 接下来输入N个整数。

// Output
// 输出M行正整数，第i行表示第i组数据的长非降子序列长度。

// Sample Input
// 2
// 4 
// 1 3 2 4
// 9
// 4 1 7 3 2 3 5 7 6

// Sample Output
// 3
// 5

/*
算法说明：动态规划类
①dp含义
tail[len - 1]表示长度为len的非降子序列能够取得的最小尾元素。
tail越小，后续数字越容易接上，因此它代表同长度子问题的最优状态。

②dp转移方程
对当前数字x，在tail中找到第一个大于x的位置pos。
若不存在，则x可以接到最长序列后面，tail追加x。
否则用x替换tail[pos]，表示得到相同长度但尾元素更小的非降子序列。

③dp初始值
tail初始为空。
依次处理所有元素后，tail的长度就是最长非降子序列长度。

④算法时间复杂度
每个元素在tail数组中二分查找一次，时间复杂度为O(nlogn)，空间复杂度为O(n)。
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
        vector<int> tail; // tail[i] 表示长度为i+1的非降序列的最小尾部元素
        tail.reserve(n); // 预分配内存
        for (int i = 0; i < n; ++i) { // 每次找到长度为i的最优非降子序列
            int x;
            cin >> x;
            auto it = upper_bound(tail.begin(), tail.end(), x); // tail中第一个大于x的位置 否则是 tail末尾
            if (it == tail.end()) {
                tail.push_back(x);
            } else {
                *it = x;
            }
        }
        cout << tail.size() << '\n';
    }
    return 0;
}