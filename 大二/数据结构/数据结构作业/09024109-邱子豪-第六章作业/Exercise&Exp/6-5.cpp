#include <iostream>
#include <limits>
#include <vector>

const int INF = std::numeric_limits<int>::max() / 4;

struct Edge {
    int u;
    int v;
    int w;
};

struct Graph {
    int V;
    std::vector<std::vector<int>> adj;

    explicit Graph(int vertices) : V(vertices), adj(vertices, std::vector<int>(vertices, INF)) {
        for (int i = 0; i < V; ++i) adj[i][i] = 0;
    }

    void addEdge(int u, int v, int w) {
        if (u < 0 || v < 0 || u >= V || v >= V) {
            throw std::out_of_range("Vertex index out of range");
        }
        if (w < 0) throw std::invalid_argument("Edge weight must be nonnegative");
        if (w < adj[u][v]) {
            adj[u][v] = w;
            adj[v][u] = w;
        }
    }
};

struct PrimResult {
    std::vector<Edge> mst;
    int totalWeight = 0;
};

PrimResult primMST(const Graph& g, int start = 0) {
    int n = g.V;
    if (n == 0) throw std::invalid_argument("Graph must have at least 1 vertex");
    if (start < 0 || start >= n) throw std::out_of_range("Start vertex out of range");

    std::vector<int> key(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<bool> inMST(n, false);

    key[start] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1;
        int best = INF;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < best) {
                best = key[v];
                u = v;
            }
        }
        if (u == -1 || best == INF) break;

        inMST[u] = true;
        for (int v = 0; v < n; ++v) {
            int w = g.adj[u][v];
            if (!inMST[v] && w < key[v]) {
                key[v] = w;
                parent[v] = u;
            }
        }
    }

    PrimResult res;
    for (int v = 0; v < n; ++v) {
        if (v == start) continue;
        if (parent[v] == -1) {
            res.mst.clear();
            res.totalWeight = 0;
            return res;
        }
        res.mst.push_back({parent[v], v, g.adj[parent[v]][v]});
        res.totalWeight += g.adj[parent[v]][v];
    }
    return res;
}

void printMST(const PrimResult& r) {
    if (r.mst.empty()) {
        std::cout << "No spanning tree (graph disconnected)\n";
        return;
    }
    std::cout << "MST weight: " << r.totalWeight << "\nEdges (u v w):\n";
    for (const auto& e : r.mst) {
        std::cout << e.u << " " << e.v << " " << e.w << "\n";
    }
}

int main() {
    Graph g(7);
    g.addEdge(0, 1, 7);
    g.addEdge(0, 3, 5);
    g.addEdge(1, 2, 8);
    g.addEdge(1, 3, 9);
    g.addEdge(1, 4, 7);
    g.addEdge(2, 4, 5);
    g.addEdge(3, 4, 15);
    g.addEdge(3, 5, 6);
    g.addEdge(4, 5, 8);
    g.addEdge(4, 6, 9);
    g.addEdge(5, 6, 11);

    PrimResult r = primMST(g, 0);
    printMST(r);
    return 0;
}
