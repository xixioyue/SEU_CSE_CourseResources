// Description - Dijkstra算法
// 给定n(n<=500)个顶点,以及E(E<=10000)条边，使用迪杰斯特拉算法计算顶点s到顶点t的最短路径.

// Input
// 第一行输入T表示有T组数据。每组数据第一行输入n、E、s、t，分别表示顶点数、边数、顶点s以及顶点t. 接下来输入E行每行三个正整数u(1<=u<=n)、v(1<=v<=n)、w，表示顶点u到顶点v之间无向边长度w（可能有重边）。

// Output
// 输出T行正整数，第i行表示第i组数据s到达t的最短路径长度。若s无法到达t国，输出-1.

// Sample Input
// 3
// 2 2 1 2
// 1 2 1
// 1 2 2
// 3 1 1 3
// 2 3 1
// 3 3 1 3
// 1 2 1
// 1 2 3
// 2 3 1

// Sample Output
// 1
// -1
// 2

/*
算法说明：图算法
①算法思想
使用Dijkstra算法求非负权图中从s到t的最短路。
dist[u]记录当前已知从s到u的最短距离，优先队列每次取出距离最小的点进行松弛。
当一个点以最短距离出队后，它的最短路已经确定。

②伪代码
Dijkstra(G, s, t):
    dist[s] = 0，其余为INF
    将(s, 0)加入优先队列
    while 队列非空:
        取出距离最小的点u
        if u == t:
            break
        for u的每条边(u, v, w):
            if dist[v] > dist[u] + w:
                dist[v] = dist[u] + w
                v入队
    return dist[t]

③时间复杂度
若s很快出队并到达t，最好情况下核心搜索代价接近O(logn)。
使用邻接表和优先队列时，最坏时间复杂度为O((n+E)logn)，空间复杂度为O(n+E)。
*/

#include <functional>
#include <iostream>
#include <limits>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, e, s, t;
        cin >> n >> e >> s >> t;
        vector<vector<pair<int, int>>> g(n + 1);
        for (int i = 0; i < e; ++i) {
            int u, v, w;
            cin >> u >> v >> w;
            g[u].push_back({v, w});
            g[v].push_back({u, w});
        }

        const long long INF = numeric_limits<long long>::max() >> 2;
        vector<long long> dist(n + 1, INF); // 距离
        priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> pq;
        dist[s] = 0;
        pq.push({0, s});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); // 取出距离起点距离最短的顶点
            pq.pop();
            if (d != dist[u]) continue; // 找到到顶点u的最短距离
            if (u == t) break;
            for (auto [v, w] : g[u]) { // 遍历u的邻接点
                if (dist[v] > d + w) { 
                    dist[v] = d + w;
                    pq.push({dist[v], v});
                }
            }
        }

        cout << (dist[t] == INF ? -1 : dist[t]) << '\n';
    }
    return 0;
}
