// Description
// 一长方形电路板两长边分别有n个焊点, 分别记作1,2,...,n. 现需要将一边的焊点与另一边的焊点用导线相连, 
// 共需要n条导线连接n对焊点. 我们用(i,xi)来表示一根导线的连接方式, 即一边的第i点与另一边的第xi相连. 
// 两条导线(i,xi)和(j,xj)交叉, 当i<j且xi>xj, 或者i>j且xi<xj.

// 当给定焊点的连接方式时, 请设计一分治算法计算有交叉点的个数.

// Input
// 第一行输入m表示有m组测试. 每组测试首先输入n (n<50000)，接下来输入n个int型整数, 表示xi.

// Output
// 对每组测试数据输出交叉点的个数

// Sample Input
// 2
// 8 
// 2 5 1 7 6 3 8 4
// 8
// 3 1 6 8 2 5 4 7

// Sample Output
// 10
// 10

/*
算法说明：分治类
①基本思想
焊线交叉条件等价于序列中的逆序对。
把序列分成左右两半，递归统计两半内部交叉数，
再在归并过程中统计左半元素大于右半元素产生的跨区间交叉数。

②伪代码
MergeCount(A, l, r):
    if r - l <= 1:
        return 0
    mid = (l + r) / 2
    ans = MergeCount(A, l, mid) + MergeCount(A, mid, r)
    合并两个有序区间
    if A[i] > A[j]:
        ans += mid - i
    return ans

③时间复杂度
每层归并统计为O(n)，共有O(logn)层，
最好、最坏时间复杂度均为O(nlogn)。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

long long merge_count(vector<int>& a, vector<int>& tmp, int l, int r) {//[l, r)
    //递归出口
    if (r - l <= 1) return 0;
    //分治
    int mid = (r + l) / 2;
    long long ans = merge_count(a, tmp, l, mid) + merge_count(a, tmp, mid, r);

    int i = l, j = mid, k = l;
    while (i < mid && j < r) {
        if (a[i] <= a[j]) {
            tmp[k++] = a[i++];
        } else {
            ans += mid - i;
            tmp[k++] = a[j++];
        }
    }
    while (i < mid) tmp[k++] = a[i++];
    while (j < r) tmp[k++] = a[j++];
    for (int p = l; p < r; ++p) a[p] = tmp[p];
    
    return ans;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<int> a(n), tmp(n);
        for (int i = 0; i < n; ++i) cin >> a[i];
        cout << merge_count(a, tmp, 0, n) << '\n';
    }
    return 0;
}