// Description
// 给定一维int型数组, 请找到第2小的数.

// Input
// 输入第1行有一个int型正整数m (m<100), 表示有m行输入.
// 每行输入的第一个数为int型正整数n (0<n<1000), 后面接着输入n个int型整数.

// Output
// 输出m行, 每行为找第2小数.

// Sample Input
// 2
// 8 3 8 4 1 6 7 3 2
// 9 2 4 5 9 8 7 6 4 3

// Sample Output
// 2
// 3

/*
算法说明：分治类
①基本思想
采用“中位数的中位数”选择算法。把数组每5个元素分为一组，组内堆排序后取中位数，
再递归求这些中位数的中位数作为划分基准。按照小于、等于、大于基准分成三部分，
根据第k小元素所在的区间继续递归。

②伪代码
Select(S, k):
    if |S| < 50:
        HeapSort(S)
        return S[k]
    将S每5个元素分为一组
    对每组HeapSort，并取组内中位数放入M
    pivot = Select(M, ceil(|M| / 2))
    A = S中小于pivot的元素
    B = S中等于pivot的元素
    C = S中大于pivot的元素
    if k <= |A|:
        return Select(A, k)
    else if k <= |A| + |B|:
        return pivot
    else:
        return Select(C, k - |A| - |B|)

③时间复杂度
组内排序和划分为O(n)，递归规模分别约为n/5和不超过7n/10，
因此最好、最坏时间复杂度均为O(n)。
*/

#include <iostream>
#include <algorithm>
using namespace std;

int arr[105];

void heapSort(int l, int r) {
    make_heap(arr + l, arr + r + 1);
    sort_heap(arr + l, arr + r + 1);
}

// 在 arr[l...r] 中找第 k 小，k 从 1 开始
int selectKth(int l, int r, int k) {
    int n = r - l + 1;

    if (n < 50) {
        heapSort(l, r);
        return arr[l + k - 1];
    }

    int medianCnt = 0;

    for (int i = l; i <= r; i += 5) {
        int right = min(i + 4, r);
        heapSort(i, right);

        int len = right - i + 1;
        int medianPos = i + (len - 1) / 2;

        swap(arr[l + medianCnt], arr[medianPos]);
        medianCnt++;
    }

    int pivot = selectKth(l, l + medianCnt - 1, (medianCnt + 1) / 2);

    int lt = l, i = l, gt = r;

    while (i <= gt) {
        if (arr[i] < pivot) {
            swap(arr[lt], arr[i]);
            lt++;
            i++;
        } else if (arr[i] > pivot) {
            swap(arr[i], arr[gt]);
            gt--;
        } else {
            i++;
        }
    }

    int lessCnt = lt - l;
    int equalCnt = gt - lt + 1;

    if (k <= lessCnt) {
        return selectKth(l, lt - 1, k);
    } else if (k <= lessCnt + equalCnt) {
        return pivot;
    } else {
        return selectKth(gt + 1, r, k - lessCnt - equalCnt);
    }
}

int main() {
    int m;
    cin >> m;

    while (m--) {
        int n;
        cin >> n;

        for (int i = 0; i < n; i++) {
            cin >> arr[i];
        }

        cout << selectKth(0, n - 1, 2) << '\n';
    }

    return 0;
}