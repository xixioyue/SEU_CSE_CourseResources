// Description - 树上着色问题
// 对一棵树进行着色，每个结点可着黑色或白色，相邻结点不能着相同黑色，但可着相同白色。请设计一种算法对树中尽量多的节点着黑色。

// Input
// 第一行输入T(T<=10)表示有T组数据。每组数据先输入一个正整数N(1<=N<=50000)，表示共有N个结点，接下来输入N-1对(u,v)，表示u与v之间有一条边。

// Output
// 输出T行正整数，第i行表示第i棵树最多能着色几个黑点。

// Sample Input
// 3
// 2
// 1 2
// 4
// 1 2
// 2 3
// 3 4
// 4
// 1 2
// 1 3
// 1 4

// Sample Output
// 1
// 2
// 3


// 树形动态规划
// 对于每个节点，我们计算两种状态的最大值：
// 1. 当前节点u着黑色时，其子树中能着色的黑点数 black[u]
// 2. 当前节点u着白色时，其子树中能着色的黑点数 white[u]
// 转移方程：
// black[u] = 1 + sum(white[v]) for all children v of u
// white[u] = sum(max(white[v], black[v])) for all children v of u

/*
算法说明：动态规划类
①dp含义
black[u]表示以u为根的子树中，u染黑时最多能染黑的节点数。
white[u]表示以u为根的子树中，u染白时最多能染黑的节点数。

②dp转移方程
若u染黑，则所有孩子不能染黑：
black[u] = 1 + sum(white[v])
若u染白，则孩子可黑可白，取较优：
white[u] = sum(max(black[v], white[v]))

③dp初始值
每个节点单独看时，black[u] = 1，white[u] = 0。
按后序遍历从叶子向根转移，答案为max(black[root], white[root])。

④算法时间复杂度
每个节点和每条边只在遍历和转移中处理常数次，时间复杂度为O(n)，空间复杂度为O(n)。
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
        int n;
        cin >> n;
        vector<vector<int>> g(n + 1); // 邻接表表示的树
        for (int i = 0; i < n - 1; ++i) { // 读入边
            int u, v;
            cin >> u >> v;
            g[u].push_back(v);
            g[v].push_back(u);
        }

        vector<int> parent(n + 1, 0), order; // parent[i]表示节点i的父节点，order表示后序遍历的节点顺序
        order.reserve(n);
        vector<int> st{1}; // 栈，用于后序遍历
        parent[1] = -1;
        while (!st.empty()) { // 记录后序遍历节点顺序，便于先计算叶子节点
            int u = st.back();
            st.pop_back();
            order.push_back(u);
            for (int v : g[u]) {
                if (v == parent[u]) continue;
                parent[v] = u;
                st.push_back(v);
            }
        }

        // 动态规划转移
        vector<int> white(n + 1, 0), black(n + 1, 1);
        for (int i = n - 1; i >= 0; --i) {
            int u = order[i];
            for (int v : g[u]) {
                if (parent[v] != u) continue;
                black[u] += white[v];
                white[u] += max(white[v], black[v]);
            }
        }

        cout << max(white[1], black[1]) << '\n';
    }
    return 0;
}