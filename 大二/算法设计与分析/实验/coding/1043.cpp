// Description
// 你将获得 K 个鸡蛋，并可以使用一栋从 1 到 N  共有 N 层楼的建筑。
// 每个蛋的功能都是一样的，如果一个蛋碎了，你就不能再把它掉下去。
// 你知道存在楼层 F ，满足 0 <= F <= N 任何从高于 F 的楼层落下的鸡蛋都会碎，从 F 楼层或比它低的楼层落下的鸡蛋都不会破。
// 每次移动，你可以取一个鸡蛋（如果你有完整的鸡蛋）并把它从任一楼层 X 扔下（满足 1 <= X <= N）。
// 你的目标是确切地知道 F 的值是多少。
// 无论 F 的初始值如何，你确定 F 的值的最小移动次数是多少？

// Input
// 第一行输入nums表示有nums组测试
// 每组测试输入K, N，表示有K个鸡蛋，N层楼

// Output
// 对每组测试数据，输出确定F的最小移动次数

// Sample Input
// 3
// 1 2
// 2 6
// 3 14

// Sample Output
// 2
// 3
// 4

// 提示：
// 1 <= K <= 100
// 1 <= N <= 10000

/*
算法说明：动态规划类
①dp含义
dp[eggs]表示在当前moves次操作内，使用eggs个鸡蛋最多能够确定多少层楼。

②dp转移方程
增加一次操作后，在某层扔鸡蛋：
若碎了，可以用eggs-1个鸡蛋和moves-1次操作确定下面dp[eggs-1]层；
若没碎，可以用eggs个鸡蛋和moves-1次操作确定上面dp[eggs]层；
再加上当前测试的这一层：
dp[eggs] = dp[eggs] + dp[eggs - 1] + 1。

③dp初始值
0次操作时任何鸡蛋数都不能确定楼层，因此dp[eggs] = 0。
不断增加moves，直到dp[K] >= N，此时moves就是最少操作次数。

④算法时间复杂度
设最终答案为moves，每增加一次操作要更新K个鸡蛋状态，时间复杂度为O(K*moves)，空间复杂度为O(K)。
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
        int k, n;
        cin >> k >> n;

        vector<long long> dp(k + 1, 0);
        int moves = 0;
        while (dp[k] < n) {
            ++moves;
            for (int eggs = k; eggs >= 1; --eggs) {
                dp[eggs] = dp[eggs] + dp[eggs - 1] + 1;
            }
        }

        cout << moves << '\n';
    }
    return 0;
}