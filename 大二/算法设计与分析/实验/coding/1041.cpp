// Description
// 给定两个大小为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。
// 请你找出并返回这两个正序数组的中位数。
// 进阶：你能设计一个时间复杂度为 O(log (m+n)) 的算法解决此问题吗？

// Input
// 第一行输入nums表示有nums组测试
// 每组测试输入n和m，分别表示数组nums1和nums2的长度
// 然后输入正序数组nums1
// 接着输入正序数组nums2

// Output
// 对每组测试数据输出两个正序数组的中位数

// Sample Input
// 2
// 2 1
// 1 3
// 2
// 2 2
// 1 2
// 3 4

// Sample Output
// 2.00000
// 2.50000

// 提示：
// nums1.length == n
// nums2.length == m
// 0 <= m <= 1000
// 0 <= n <= 1000
// 1 <= m + n <= 2000
// -10^6 <= nums1[i], nums2[i] <= 10^6

/*
算法说明：基础算法类
①算法思想
在较短数组上二分切分位置i，同时确定另一个数组切分位置j，
使左半部分元素总数等于右半部分或多1个。
当leftA <= rightB且leftB <= rightA时，两个数组被正确划分，
中位数由左半最大值和右半最小值确定。

②伪代码
FindMedian(A, B):
    保证A是较短数组
    在A上二分切分位置i
    j = (n + m + 1) / 2 - i
    if leftA <= rightB且leftB <= rightA:
        根据左右两边最大/最小值返回中位数
    else if leftA > rightB:
        i向左移动
    else:
        i向右移动

③时间复杂度
若第一次二分即满足划分条件，最好时间复杂度为O(1)。
最坏情况下在较短数组上二分，时间复杂度为O(log(min(n,m)))。
*/

#include <iomanip>
#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
    // 二分查找法 O(log(min(m,n)))
    int n = nums1.size();
    int m = nums2.size();
    vector<int>& a = nums1;
    vector<int>& b = nums2;

    int len = (n + m + 1) / 2;
    int left = 0, right = n; 
    while (left <= right) {
        int i= (right + left) / 2;
        int j = len - i;

        int leftA = (i == 0) ? INT_MIN : a[i - 1];
        int rightA = (i == n) ? INT_MAX : a[i];
        int leftB = (j == 0) ? INT_MIN : b[j - 1];
        int rightB = (j == m) ? INT_MAX : b[j];

        if (leftA <= rightB && leftB <= rightA) { // end
            if ((n + m) % 2 == 1) 
                return max(leftA, leftB);
            else
                return (max(leftA, leftB) + min(rightA, rightB)) / 2.0;
        } else if (leftA > rightB) {
            right = i - 1;
        } else {
            left = i + 1;
        }
    }
    return -1;
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    cout << fixed << setprecision(5);
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<int> a(n), b(m);
        for (int i = 0; i < n; ++i) cin >> a[i];
        for (int i = 0; i < m; ++i) cin >> b[i];

        // // 归并排序法 O(m+n)
        // int total = n + m;
        // int left_mid = (total - 1) / 2; // 中位数的左索引
        // int right_mid = total / 2; // 中位数的右索引
        // int i = 0, j = 0;
        // int current = 0, left_value = 0, right_value = 0;
        // for (int idx = 0; idx <= right_mid; ++idx) {
        //     if (j >= m || (i < n && a[i] <= b[j])) {
        //         current = a[i++];
        //     } else {
        //         current = b[j++];
        //     }
        //     if (idx == left_mid) left_value = current;
        //     if (idx == right_mid) right_value = current;
        // }

        // cout << (left_value + right_value) / 2.0 << '\n';

        if(n < m)
            cout << findMedianSortedArrays(a, b) << '\n';
        else 
            cout << findMedianSortedArrays(b, a) << '\n';
    }
    return 0;
}