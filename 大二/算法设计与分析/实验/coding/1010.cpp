// Description
// 给定一递增有序数组a[0,1,...,n-1], 请在数组中搜索给定元素. 搜索过程中请使用mid=(low+high)/2.

// Input
// 第一行输入M表示包含M组测试数据，每组输入N和X, N (1000<=N<=20000) 表示后面有N个整数递增有序数组, 表示将搜索的元素.

// Output
// 搜索成功输出success及父亲, 否则输出not found及父亲.

// Sample Input
// 2
// 7 10 1 3 5 7 9 11 13
// 7 10 2 4 6 8 10 12 14

// Sample Output
// not found, father is 9
// success, father is 12

// *请注意逗号后加空格

/*
算法说明：基础算法类
①算法思想
二分查找利用数组递增有序的性质，每次取mid=(low+high)/2。
若中间元素小于目标值，则目标只可能在右半部分；若大于目标值，则只可能在左半部分。
查找过程中记录最后访问的中间元素作为father。

②伪代码
BinarySearch(A, x):
    low = 0, high = n - 1, father = -1
    while low <= high:
        mid = (low + high) / 2
        if A[mid] == x:
            return success, father
        father = A[mid]
        if A[mid] < x:
            low = mid + 1
        else:
            high = mid - 1
    return not found, father

③时间复杂度
最好情况下第一次比较命中，时间复杂度为O(1)。
最坏情况下区间不断减半直到为空，时间复杂度为O(logn)。
*/

#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, x;
        cin >> n >> x;
        vector<int> a(n);
        for (int &v : a) cin >> v;

        int low = 0, high = n - 1;
        int father = -1;
        bool found = false;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (a[mid] == x) {
                found = true;
                break;
            }
            father = a[mid];
            if (a[mid] < x) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        if (found) {
            cout << "success, father is " << father << '\n';
        } else {
            cout << "not found, father is " << father << '\n';
        }
    }
    return 0;
}