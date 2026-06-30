// Description
// 有n个作业需要在一台机器上执行，一个时刻机器上只能执行一个作业，每个作业可在单位时间内完成，作业i有截止时间di，当作业i在截止时间被执行完，则可获得pi的收益。求最大收益。

// Input
// 第一行输入T(T<=10)表示有T组数据。每组数据先输入一个正整数N(1<=N<=50000)，表示共有N个作业，随后输入N组(di,pi)，表示每个作业的截止时间和收益, di是不大于50000的整数。

// Output
// 输出T行正整数，第i行表示第i组数据下能获得的最大收益。

// Sample Input
// 3
// 1
// 4 10
// 4
// 1 5
// 1 6
// 2 3
// 3 10
// 4
// 2 5
// 2 6
// 3 3
// 3 10

// Sample Output
// 10
// 19
// 21

/*
算法说明：贪心类
①算法思想
按收益从大到小考虑作业。对每个作业，尽量安排在不超过其截止时间的最晚空闲时间槽。
使用并查集维护每个时间点之前最近的空闲槽，从而快速找到可安排位置。

②伪代码
JobSchedule(Jobs):
    按收益从大到小排序作业
    初始化并查集表示可用时间槽
    ans = 0
    for 每个作业(deadline, profit):
        slot = deadline之前最晚的空闲时间槽
        if slot存在:
            安排该作业
            ans = ans + profit
            占用slot
    return ans

③如何根据子问题最优确定总问题最优
收益最高的作业若能安排，就应优先保留；把它放在尽可能晚的位置，
不会影响更早截止的作业，给后续作业留下更多选择。
选择并占用该时间槽后，剩余作业仍是在剩余时间槽中求最大收益的同类问题。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Job {
    int deadline;
    long long profit;
};

struct DSU {
    vector<int> parent;

    explicit DSU(int n) : parent(n + 1) {
        for (int i = 0; i <= n; ++i) parent[i] = i;
    }

    // 查找根节点
    int find(int x) {
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    // 占用一个时间槽
    void occupy(int x) {
        parent[x] = find(x - 1);
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n;
        cin >> n;
        vector<Job> jobs(n);
        int max_deadline = 0;
        for (auto& job : jobs) {
            cin >> job.deadline >> job.profit;
            max_deadline = max(max_deadline, job.deadline);
        }

        sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) { // 根据收益降序排列
            return a.profit > b.profit;
        });

        DSU dsu(max_deadline); // 初始化并查集
        long long ans = 0;
        for (const auto& job : jobs) { // 根据收益降序查找可用时间槽
            int slot = dsu.find(min(job.deadline, max_deadline)); // 查找可用的时间槽
            if (slot > 0) {
                ans += job.profit;
                dsu.occupy(slot);
            }
        }
        cout << ans << '\n';
    }
    return 0;
}