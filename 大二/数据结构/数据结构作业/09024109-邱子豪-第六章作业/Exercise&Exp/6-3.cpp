#include <iostream>
#include <queue>
#include <stdexcept>
#include <vector>

struct Graph {
    int V;
    std::vector<std::vector<int>> adj;

    explicit Graph(int vertices) : V(vertices), adj(vertices) {}

    void addEdge(int u, int v) {
        if (u < 0 || v < 0 || u >= V || v >= V) {
            throw std::out_of_range("Vertex index out of range");
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
};

std::vector<int> breadthFirstSearch(const Graph& g, int start) {
    if (start < 0 || start >= g.V) {
        throw std::out_of_range("Start vertex out of range");
    }
    std::vector<bool> visited(g.V, false);
    std::queue<int> q;
    std::vector<int> order;

    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : g.adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return order;
}

std::vector<int> breadthFirstSearchAll(const Graph& g) {
    std::vector<bool> visited(g.V, false);
    std::queue<int> q;
    std::vector<int> order;

    for (int s = 0; s < g.V; ++s) {
        if (visited[s]) continue;
        visited[s] = true;
        q.push(s);
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);
            for (int v : g.adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }
    return order;
}

void printOrder(const std::vector<int>& order) {
    for (size_t i = 0; i < order.size(); ++i) {
        if (i) std::cout << " ";
        std::cout << order[i];
    }
    std::cout << "\n";
}

int main() {
    Graph g(8);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(4, 5);
    g.addEdge(6, 7);

    std::cout << "BFS from 0 (component containing 0): ";
    printOrder(breadthFirstSearch(g, 0));

    std::cout << "BFS over all components: ";
    printOrder(breadthFirstSearchAll(g));

    return 0;
}
