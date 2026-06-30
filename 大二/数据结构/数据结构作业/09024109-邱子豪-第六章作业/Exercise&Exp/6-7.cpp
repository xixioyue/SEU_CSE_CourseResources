#include <iostream>
#include <limits>
#include <vector>

const int INF = std::numeric_limits<int>::max() / 4;
struct Edge { int u, v, w; };

struct Graph {
    int V;
    std::vector<std::vector<int>> w;
    explicit Graph(int n) : V(n), w(n, std::vector<int>(n, INF)) {
        for (int i = 0; i < n; ++i) w[i][i] = 0;
    }
    void addEdge(int u, int v, int weight) {
        if (u < 0 || v < 0 || u >= V || v >= V) throw std::out_of_range("Vertex index out of range");
        if (weight < 0) throw std::invalid_argument("Edge weight must be nonnegative");
        if (weight < w[u][v]) {
            w[u][v] = weight;
            w[v][u] = weight;
        }
    }
};

struct MSTResult {
    std::vector<Edge> edges;
    int totalWeight = 0;
};

MSTResult primMatrixO2(const Graph& g, int start = 0) {
    int n = g.V;
    if (n == 0) throw std::invalid_argument("Graph must have at least one vertex");
    if (start < 0 || start >= n) throw std::out_of_range("Start vertex out of range");

    std::vector<int> key(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<bool> inMST(n, false);

    key[start] = 0;

    for (int iter = 0; iter < n; ++iter) {
        int u = -1, best = INF;
        for (int v = 0; v < n; ++v) {
            if (!inMST[v] && key[v] < best) {
                best = key[v];
                u = v;
            }
        }
        if (u == -1 || best == INF) break;
        inMST[u] = true;
        for (int v = 0; v < n; ++v) {
            int weight = g.w[u][v];
            if (!inMST[v] && weight < key[v]) {
                key[v] = weight;
                parent[v] = u;
            }
        }
    }

    MSTResult res;
    for (int v = 0; v < n; ++v) {
        if (v == start) continue;
        if (parent[v] == -1) {
            res.edges.clear();
            res.totalWeight = 0;
            return res;
        }
        res.edges.push_back({parent[v], v, g.w[parent[v]][v]});
        res.totalWeight += g.w[parent[v]][v];
    }
    return res;
}

void printMST(const MSTResult& r) {
    if (r.edges.empty()) {
        std::cout << "No spanning tree (graph disconnected)\n";
        return;
    }
    std::cout << "MST weight: " << r.totalWeight << "\nEdges (u v w):\n";
    for (const auto& e : r.edges) {
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

    MSTResult r = primMatrixO2(g, 0);
    printMST(r);
    return 0;
}
