// Description
// 给定一个长度为N的int型数组a[0,1,2,...N-1], 请计算逆序对个数.当i<j且a[i]>a[j], 则称a[i]与a[j]是一对逆序对.

// Input
// 第一行输入M表示包含M组测试数据，每组先输入N (N<=50000), 接着输入N个int型整数.

// Output
// 输出逆序对个数.

// Sample Input
// 2
// 5 1 5 2 1 3
// 6 85 16 44 99 66 1

// Sample Output
// 4
// 9

/*
算法说明：分治类
①基本思想
逆序对可以分为三类：左半部分内部、右半部分内部、跨越左右两半。
递归统计左右两半的逆序对数量，再在归并两个有序区间时统计跨区间逆序对。
当左侧当前元素大于右侧当前元素时，左侧剩余元素都能与该右侧元素形成逆序对。

②伪代码
MergeCount(A, l, r):
    if l >= r:
        return 0
    mid = (l + r) / 2
    ans = MergeCount(A, l, mid) + MergeCount(A, mid + 1, r)
    合并A[l..mid]和A[mid+1..r]
    if A[i] > A[j]:
        ans += mid - i + 1
    return ans

③时间复杂度
每层归并统计总代价O(n)，递归层数O(logn)，
最好、最坏时间复杂度均为O(nlogn)。
*/

#include <iostream>
#include <vector>
using namespace std;

long long merge_count(vector<int> &a, vector<int> &tmp, int l, int r) {
    //递归出口
    if (r - l <= 0) return 0;
    //分治
    int mid = (l + r) / 2;
    long long ans = merge_count(a, tmp, l, mid) + merge_count(a, tmp, mid + 1, r);

    int i = l, j = mid + 1, k = l;
    while (i <= mid && j <= r){
        if(a[i]<= a[j])
            tmp[k++] = a[i++];
        else{
            ans += mid - i + 1;
            tmp[k++] = a[j++];
        }
    }
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= r) tmp[k++] = a[j++];
    for (int t = l; t <= r; ++t) a[t] = tmp[t];

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
        for (int &x : a) cin >> x;
        cout << merge_count(a, tmp, 0, n-1) << '\n';
    }
    return 0;
}


/*
    //递归出口
    if (r - l <= 1) return 0;
    //分治
    int mid = (l + r) / 2;
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
    for (int t = l; t < r; ++t) a[t] = tmp[t];
    return ans;
*/