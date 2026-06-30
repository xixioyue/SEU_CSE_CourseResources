// Description
// 在一个火车旅行很受欢迎的国度，你提前一年计划了一些火车旅行。在接下来的一年里，你要旅行的日子将以一个名为 days 的数组给出。每一项是一个从 1 到 365 的整数。

// 火车票有三种不同的销售方式：
// 一张为期一天的通行证售价为 costs[0] 美元；
// 一张为期七天的通行证售价为 costs[1] 美元；
// 一张为期三十天的通行证售价为 costs[2] 美元。
// 通行证允许数天无限制的旅行。 例如，如果我们在第 2 天获得一张为期 7 天的通行证，那么我们可以连着旅行 7 天：第 2 天、第 3 天、第 4 天、第 5 天、第 6 天、第 7 天和第 8 天。

// 返回你想要完成在给定的列表 days 中列出的每一天的旅行所需要的最低消费。


// Input
// 第一行输入nums表示有nums组测试
// 对每组测试用例
// 第一行输入m
// 第二行输入具有m个元素的days数组，days[i]表示你将在days[i]这天旅行
// 第三行输入具有3个元素的costs数组，具体释义见Description

// Output
// 对每组测试数据，输出你想要完成在给定的 days数组 中列出的每一天的旅行所需要的最低消费。

// Sample Input
// 2
// 6
// 1 4 6 7 8 20
// 2 7 15
// 12
// 1 2 3 4 5 6 7 8 9 10 30 31
// 2 7 15

// Sample Output
// 11
// 17

// 提示：
// 1 <= days.length <= 365
// 1 <= days[i] <= 365
// days 按顺序严格递增
// costs.length == 3
// 1 <= costs[i] <= 1000

// 动态规划解法

/*
算法说明：动态规划类
①dp含义
dp[day]表示覆盖从第1天到第day天所有旅行日所需的最低费用。
travel[day]记录第day天是否需要旅行。

②dp转移方程
若第day天不旅行：
dp[day] = dp[day - 1]
若第day天旅行：
dp[day] = min(dp[day - 1] + cost1,
              dp[max(0, day - 7)] + cost7,
              dp[max(0, day - 30)] + cost30)

③dp初始值
dp[0] = 0，表示第0天之前没有花费。
从第1天递推到第365天，答案为dp[365]。

④算法时间复杂度
按一年365天递推，时间复杂度为O(365)，空间复杂度为O(365)，也可记为O(maxDay)。
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
        int m;
        cin >> m;
        vector<int> days(m), costs(3);
        for (int i = 0; i < m; ++i) cin >> days[i];
        for (int i = 0; i < 3; ++i) cin >> costs[i];

        vector<int> travel(366, 0);
        for (int d : days) travel[d] = 1;

        vector<int> dp(366, 0);
        for (int day = 1; day <= 365; ++day) {
            if (!travel[day]) {
                dp[day] = dp[day - 1];
            } else {
                dp[day] = min({
                    dp[day - 1] + costs[0],
                    dp[max(0, day - 7)] + costs[1],
                    dp[max(0, day - 30)] + costs[2]
                });
            }
        }

        cout << dp[365] << '\n';
    }
    return 0;
}