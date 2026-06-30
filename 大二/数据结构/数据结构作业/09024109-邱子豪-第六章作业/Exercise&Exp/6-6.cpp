#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

struct Edge {
    int to;
    int w;
};

struct Tree {
    int V;
    std::vector<std::vector<Edge>> adj;  // adjacency list

    explicit Tree(int vertices) : V(vertices), adj(vertices) {}

    void addEdge(int u, int v, int w) {
        if (u < 0 || v < 0 || u >= V || v >= V) {
            throw std::out_of_range("Vertex index out of range");
        }
        if (w < 0) {
            throw std::invalid_argument("Edge weight must be nonnegative");
        }
        adj[u].push_back({v, w});
        adj[v].push_back({u, w});
    }
};

std::vector<int> shortestFromRoot(const Tree& t, int root) {
    if (root < 0 || root >= t.V) throw std::out_of_range("Root index out of range");
    std::vector<int> dist(t.V, -1);
    std::queue<int> q;
    dist[root] = 0;
    q.push(root);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (const auto& e : t.adj[u]) {
            if (dist[e.to] == -1) {
                dist[e.to] = dist[u] + e.w;
                q.push(e.to);
            }
        }
    }
    return dist;
}

int main() {
    Tree t(7);
    t.addEdge(0, 1, 3);
    t.addEdge(0, 2, 2);
    t.addEdge(1, 3, 4);
    t.addEdge(1, 4, 6);
    t.addEdge(2, 5, 5);
    t.addEdge(5, 6, 1);

    int root = 0;
    auto dist = shortestFromRoot(t, root);
    std::cout << "Distances from root " << root << ":\n";
    for (size_t i = 0; i < dist.size(); ++i) {
        std::cout << i << ": " << dist[i] << "\n";
    }
    return 0;
}
