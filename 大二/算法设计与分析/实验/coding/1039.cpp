// Description
// 输入一张有向图, 输出拓扑排序后的的结果.

// Input
// 输入第一行是一个int型整数t,表示有t组测试数据。接下来每组测试数据第一行为两个整数n,m(n<=300,0<=m<=n*(n-1)/2),表示后面有n个节点（编号为1，2，...，n ),m条有向边. 
// 后面的m行中每行有两个int型整数X和Y，表示X号点到Y号点有一条有向边，表示Y号点必须出现在X号点之后.

// Output
// 对于每组数据:若没有可行的拓扑方案,输出0.若可行方案存在，输出任意一个可行方案(n个节点编号,编号间使用一个空格隔开）.每组数据的输出以一个回车结尾.

// Sample Input
// 2
// 2 2
// 1 2
// 2 1
// 3 3
// 3 1
// 2 3
// 2 1

// Sample Output
// 0
// 2 3 1

/*
算法说明：图算法
①算法思想
使用Kahn拓扑排序。先统计每个点的入度，把所有入度为0的点加入队列。
每次取出一个入度为0的点加入答案，并删除它的出边，使相邻点入度减1。
若最后输出点数不足n，说明图中存在环，无法拓扑排序。

②伪代码
TopologicalSort(G):
    统计所有点入度
    将入度为0的点入队
    while 队列非空:
        u出队并加入答案
        for u的每条出边u -> v:
            indeg[v] = indeg[v] - 1
            if indeg[v] == 0:
                v入队
    if 答案点数不足n:
        return 0
    return 答案序列

③时间复杂度
每个点入队出队一次，每条边被处理一次。
最好、最坏时间复杂度均为O(n+m)，空间复杂度为O(n+m)。
*/

#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    if (!(cin >> T)) return 0;
    while (T--) {
        int n, m;
        cin >> n >> m;
        vector<vector<int>> g(n + 1); // 邻接表表示有向图
        vector<int> indeg(n + 1, 0); // 入度
        for (int i = 0; i < m; ++i) {
            int x, y;
            cin >> x >> y;
            g[x].push_back(y);
            ++indeg[y];
        }

        queue<int> q; // 拓扑排序队列
        for (int i = 1; i <= n; ++i) {
            if (indeg[i] == 0) q.push(i);
        }

        vector<int> ans; // 存储拓扑排序结果
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            ans.push_back(u);
            for (int v : g[u]) {
                if (--indeg[v] == 0) q.push(v);
            }
        }

        if ((int)ans.size() != n) {
            cout << 0 << '\n';
        } else {
            for (int i = 0; i < n; ++i) {
                if (i) cout << ' ';
                cout << ans[i];
            }
            cout << '\n';
        }
    }
    return 0;
}
