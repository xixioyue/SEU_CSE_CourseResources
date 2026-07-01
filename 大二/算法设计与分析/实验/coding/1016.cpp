// Description
// 给定一个N(N<=50000)个int型整数的集合以及一个int型整数X, 问集合中是否存在两个元素的和等于X.

// Input
// 第一行输入M表示有M组测试. 每组测试首先输入N和X，接下来输入N个int型整数.

// Output
// 若否存在两个元素的和等于X则输出yes, 否则输出no.

// Sample Input
// 2
// 8 7 
// 1 5 11 5 4 3 9 6
// 8 7 
// 1 5 11 5 5 3 9 5

// Sample Output
// yes
// no

// 双指针解法 时间复杂度为O(nlogn)
// 分治算法这样解决：
// 1. 将数组分成两半，分别对两半进行递归求解，得到两半的结果
// 2. 将两半的结果进行合并，在合并过程中判断是否存在两个元素的和等于X
// 3. 递归出口：当数组长度为1时，返回该元素；当数组长度为0时，返回空
// 分治算法的伪代码：
// bool divide(vector<int>& arr, int l, int r, int X) {
//     if (r - l <= 0) return false; // 递归出口
//     int mid = (l + r) / 2;
//     if (divide(arr, l, mid, X) || divide(arr, mid + 1, r, X)) return true; // 递归求解
//     // 合并两半的结果
//     sort(arr.begin() + l, arr.begin() + mid + 1);
//     sort(arr.begin() + mid + 1, arr.begin() + r + 1);
//     int i = l, j = r;
//     while (i <= mid && j > mid) {
//         int sum = arr[i] + arr[j];
//         if (sum == X) return true;
//         if (sum < X) ++i;
//         else --j;    
//     }
//     return false;
// }

/*
算法说明：贪心类
①算法思想
先将数组排序，再使用左右双指针。左指针指向当前最小元素，右指针指向当前最大元素。
若两数之和小于X，只能增大左指针；若大于X，只能减小右指针；若等于X则找到答案。

②伪代码
TwoSum(A, X):
    Sort(A)
    l = 0, r = n - 1
    while l < r:
        sum = A[l] + A[r]
        if sum == X:
            return yes
        else if sum < X:
            l = l + 1
        else:
            r = r - 1
    return no

③如何根据子问题最优确定总问题最优
排序后，若a[l] + a[r] < X，则a[l]与当前及更小的右侧元素都不可能组成X，
因此舍弃a[l]不会丢失最优解。若a[l] + a[r] > X，则a[r]与当前及更大的左侧元素都不可能组成X，
因此舍弃a[r]也安全。每次都排除一批不可能元素，直到得到总问题答案。
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
        long long x;
        cin >> n >> x;
        vector<long long> a(n);
        for (long long &v : a) cin >> v;
        sort(a.begin(), a.end());

        int l = 0, r = n - 1;
        bool ok = false;
        while (l < r) {
            long long sum = a[l] + a[r];
            if (sum == x) {
                ok = true;
                break;
            }
            if (sum < x) ++l;
            else --r;
        }
        cout << (ok ? "yes" : "no") << '\n';
    }
    return 0;
}