#include <iostream>
#include <queue>
#include <stdexcept>
#include <tuple>
#include <vector>

struct Edge {
    int u;
    int v;
    int w;
};

struct DSU {
    std::vector<int> parent;
    std::vector<int> size;

    explicit DSU(int n) : parent(n), size(n, 1) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) std::swap(a, b);
        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

struct KruskalResult {
    std::vector<Edge> mst;
    int totalWeight = 0;
};

KruskalResult kruskalMST(int V, const std::vector<Edge>& edges) {
    if (V < 1) throw std::invalid_argument("Graph must have at least 1 vertex");

    auto cmp = [](const Edge& a, const Edge& b) { return a.w > b.w; };
    std::priority_queue<Edge, std::vector<Edge>, decltype(cmp)> pq(cmp);
    for (const auto& e : edges) pq.push(e);

    DSU dsu(V);
    KruskalResult res;
    while (!pq.empty() && static_cast<int>(res.mst.size()) < V - 1) {
        Edge e = pq.top();
        pq.pop();
        if (dsu.unite(e.u, e.v)) {
            res.mst.push_back(e);
            res.totalWeight += e.w;
        }
    }
    return res;
}

void printMST(const KruskalResult& r) {
    std::cout << "MST weight: " << r.totalWeight << "\nEdges (u v w):\n";
    for (const auto& e : r.mst) {
        std::cout << e.u << " " << e.v << " " << e.w << "\n";
    }
}

int main() {
    std::vector<Edge> edges = {
        {0, 1, 7}, {0, 3, 5}, {1, 2, 8}, {1, 3, 9}, {1, 4, 7},
        {2, 4, 5}, {3, 4, 15}, {3, 5, 6}, {4, 5, 8}, {4, 6, 9}, {5, 6, 11}
    };
    int V = 7;

    KruskalResult r = kruskalMST(V, edges);
    if (static_cast<int>(r.mst.size()) < V - 1) {
        std::cout << "No spanning tree (graph disconnected)\n";
    } else {
        printMST(r);
    }
    return 0;
}
