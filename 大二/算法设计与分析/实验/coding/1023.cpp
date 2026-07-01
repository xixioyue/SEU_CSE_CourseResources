/*Description
贫穷的小A有一个梦想，就是到t国去一次穷游，但现实是残酷的。
小A所在的世界一共有n(n<=500)个国家，国家与国家之间总共有E(E<=50000)条道路相连，第i个国家对于进入它的外国人都要收取Bi的费用，而小A家住在s国，他必须通过这些道路在各个国家之间中转最终到达t国（除非他运气够好可以直接从s国到达t国）。
但是贫穷的小A只剩下M(M<=100)元家底了，因此他必须精打细算旅途的费用，同时小A对于t国实在太向往了，因此他希望能够走最短的路尽快到达t国。
这个问题难倒了小A，现在他请你帮他算一算他到达t国的最短路径有多长。*/

/*Input
第一行输入T(T<=10)表示有T组数据。
每组数据第一行输入5个整数n、E、s、t、M，分别表示小A所在世界的国家数、国家之间的总道路数、小A的国籍、小A向往的国家以及小A的家底；
接下来一行输入n个正整数Bi，表示第i个国家收取的过路费（由于小A是s国人，因此s国不会收取，但t国会）；
接下来输入E行每行三个正整数u(1<=u<=n)、v(1<=v<=n)、w，表示u国和v国之间存在着一条长度为w的无向边（可能有重边）。
输入保证最终结果不会使int溢出。 */

// Output
// 输出T行正整数，第i行表示第i组数据小A花费不超过M元到达t国的最短路。若小A无法到达t国，输出-1.

// Sample Input
// 3
// 2 2 1 2 10
// 20 10
// 1 2 1
// 1 2 2
// 3 1 1 3 10
// 1 1 1
// 2 3 1
// 3 3 1 3 10
// 1 11 1
// 1 2 1
// 1 2 3
// 2 3 1

// Sample Output
// 1
// -1
// -1

/*
算法说明：图算法
①算法思想
在普通最短路的顶点基础上增加“已经花费的钱数”这一维状态。
dist[u][cost]表示到达国家u且已花费cost元时的最短路程。
由于道路长度为正，使用优先队列Dijkstra，每次扩展当前路程最短的合法状态，
转移到相邻国家时同时增加路程和过路费，超出预算的状态丢弃。

②伪代码
BudgetDijkstra(G, s, t, M):
    dist[s][0] = 0
    将(s, 0, 0)加入优先队列
    while 队列非空:
        取出距离最小的状态(d, u, cost)
        if u == t:
            return d
        for 每条边u -> v:
            nextCost = cost + fee[v]
            if nextCost <= M且nextDist更优:
                更新dist[v][nextCost]
                新状态入队
    return -1

③时间复杂度
设预算为M，扩展状态数最多为O(nM)，边状态转移最多为O(EM)。
最好情况下目标状态很早出队，可接近O(log(nM))次核心操作；
最坏时间复杂度为O(EMlog(nM))，空间复杂度为O(nM)。
*/

#include <functional>
#include <iostream>
#include <queue>
#include <tuple>
#include <vector>
using namespace std;

struct Edge {
    int to;
    int w;
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, e, s, t, money;
        cin >> n >> e >> s >> t >> money;
        vector<int> fee(n + 1); // fee[i] 表示进入第i个国家需要支付的费用
        for (int i = 1; i <= n; ++i) cin >> fee[i];

        vector<vector<Edge>> g(n + 1); // g[i] 存储与国家i相连的边
        for (int i = 0; i < e; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            g[u].push_back({v, w});
            g[v].push_back({u, w});
        }

        const int INF = 1000000000;
        vector<vector<int>> dist(n + 1, vector<int>(money + 1, INF)); // dist[i][j] 表示 到达国家i,花费j元 的最短距离
        priority_queue<tuple<int, int, int>, vector<tuple<int, int, int>>, greater<tuple<int, int, int>>> pq; //{ 当前距离d， 当前国家u，当前花费cost}
        dist[s][0] = 0;
        pq.push({0, s, 0});

        int ans = -1;
        while (!pq.empty()) {
            auto [d, u, cost] = pq.top();
            pq.pop();
            if (d != dist[u][cost]) continue; // 跳过非最优状态
            
            if (u == t) { // 已经到达小A向往的t国
                ans = d;
                break;
            }

            for (const Edge &edge : g[u]) {
                int v = edge.to; // 下一个国家
                int next_cost = cost + (v == s ? 0 : fee[v]); // 进入下一个国家的总费用
                if (next_cost > money) continue;
                int next_dist = d + edge.w; // 进入下一个国家的总距离
                if (next_dist < dist[v][next_cost]) { // 在同花费下 进入下一个v国 的花费比当前记录的更小
                    dist[v][next_cost] = next_dist;
                    pq.push({next_dist, v, next_cost});
                }
            }
        }
        cout << ans << '\n';
    }
    return 0;
}
