// Description
// 给定n(n<=500)个顶点,以及E(E<=20000)条边，计算最小生成树的权值.

// Input
// 第一行输入T表示有T组数据。每组数据第一行输入n、E，分别表示顶点数和边数. 
// 接下来输入E行每行三个正整数u(1<=u<=n)、v(1<=v<=n)、w，表示顶点u到顶点v之间无向边长度w（可能有重边）。

// Output
// 输出T行正整数，第i行表示第i组数据的最小生成树权值, 若不能构建最小生成树输出-1。

// Sample Input
// 3
// 2 2
// 1 2 1
// 1 2 2
// 3 1
// 2 3 1
// 3 3
// 1 2 2
// 1 2 3
// 2 3 1

// Sample Output
// 1
// -1
// 3

/*
算法说明：图算法
①算法思想
使用Kruskal算法。先把所有边按权值从小到大排序，
依次尝试加入当前最短的边；若这条边连接了两个不同连通块，就加入最小生成树，
否则跳过以避免形成环。

②伪代码
Kruskal(G):
    按边权从小到大排序所有边
    初始化并查集
    ans = 0, used = 0
    for 每条边(u, v, w):
        if u和v不在同一集合:
            合并u和v
            ans = ans + w
            used = used + 1
    if used == n - 1:
        return ans
    return -1

③如何根据子问题最优确定总问题最优
在任意割中，跨越该割的最小权边一定可以属于某棵最小生成树。
Kruskal每次选择当前不会成环的最小边，相当于不断把若干连通块用安全边合并。
每一步都保留存在最优解的可能，直到选出n-1条边得到总问题最优。
*/

#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

struct Edge {
    int u, v, w;
};

struct DSU { // 并查集
    vector<int> parent, rankv;

    explicit DSU(int n) : parent(n + 1), rankv(n + 1, 0) {
        for (int i = 1; i <= n; ++i) parent[i] = i;
    }

    int find(int x) { // 查找
        if (parent[x] == x) return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int a, int b) { // 合并
        a = find(a);
        b = find(b);
        // 如果两个节点已经在同一个集合中，则不需要合并
        if (a == b) return false;
        // 将秩小的树合并到秩大的树下
        if (rankv[a] < rankv[b]) swap(a, b);
        parent[b] = a;
        // 如果两棵树的秩相同，则合并后的树的秩加1
        if (rankv[a] == rankv[b]) ++rankv[a];
        return true;
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, e;
        cin >> n >> e;
        vector<Edge> edges(e);
        for (auto& edge : edges) cin >> edge.u >> edge.v >> edge.w;
        sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { // 按照边的权重从小到大排序
            return a.w < b.w;
        });

        DSU dsu(n);
        long long ans = 0;
        int used = 0;
        for (const auto& edge : edges) {
            if (dsu.unite(edge.u, edge.v)) {
                ans += edge.w;
                ++used;
                if (used == n - 1) break;
            }
        }

        cout << (used == n - 1 ? ans : -1) << '\n';
    }
    return 0;
}
