// Description
// 编写一个高效的算法来搜索 m x n 矩阵 matrix 中的一个目标值 target。该矩阵具有以下特性：
// 1. 每行的元素从左到右升序排列。
// 2. 每列的元素从上到下升序排列。

// Input
// 第一行输入nums表示有nums组测试
// 每组测试输入m、n，target，分别表示矩阵的行列数以及目标值
// 接下来输入m * n的二维矩阵

// Output
// 对每组测试数据输出 能否在矩阵中找到target
// 若能找到，输出true
// 若找不到，输出false

// Sample Input
// 1
// 5 5 5 
// 1 4 7 11 15
// 2 5 8 12 19
// 3 6 9 16 22
// 10 13 14 17 24 
// 18 21 23 26 30

// Sample Output
// true

// 提示：
// m <= 1000
// n <= 1000

/*
算法说明：贪心类
①算法思想
从矩阵右上角开始搜索。当前位置大于target时，该列下面的元素更大，
所以整列可排除，向左移动；当前位置小于target时，该行左边的元素更小，
所以整行可排除，向下移动。

②伪代码
SearchMatrix(matrix, target):
    row = 0, col = n - 1
    while row < m且col >= 0:
        if matrix[row][col] == target:
            return true
        else if matrix[row][col] > target:
            col = col - 1
        else:
            row = row + 1
    return false

③如何根据子问题最优确定总问题最优
每一步都利用行列有序性排除一整行或一整列，且被排除区域不可能含有target。
排除后剩余区域仍然满足行列递增的性质，因此同样的局部判断可以继续用于子问题。
直到找到target或搜索区域为空，即得到全局答案。
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
        int m, n, target;
        cin >> m >> n >> target;
        vector<vector<int>> matrix(m, vector<int>(n));
        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) cin >> matrix[i][j];
        }

        int row = 0, col = n - 1;
        bool found = false;
        while (row < m && col >= 0) {
            if (matrix[row][col] == target) {
                found = true;
                break;
            }
            if (matrix[row][col] > target) {
                --col;
            } else {
                ++row;
            }
        }
        cout << (found ? "true" : "false") << '\n';
    }
    return 0;
}