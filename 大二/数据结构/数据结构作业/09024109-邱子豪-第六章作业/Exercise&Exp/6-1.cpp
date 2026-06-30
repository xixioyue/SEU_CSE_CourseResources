#include <iostream>
#include <stdexcept>
#include <vector>

struct AdjNode {
    int v;
    AdjNode* next;

    AdjNode(int vertex, AdjNode* nxt = nullptr) : v(vertex), next(nxt) {}
};

struct Graph {
    int V;
    int E;
    std::vector<AdjNode*> adj;
    explicit Graph(int vertices) : V(vertices), E(0), adj(vertices, nullptr) {}

    ~Graph() {
        for (AdjNode* head : adj) {
            while (head) {
                AdjNode* tmp = head;
                head = head->next;
                delete tmp;
            }
        }
    }
};

void addEdge(Graph& g, int u, int v) {
    if (u < 0 || v < 0 || u >= g.V || v >= g.V) {
        throw std::out_of_range("Vertex index out of range");
    }
    g.adj[u] = new AdjNode(v, g.adj[u]);
    g.adj[v] = new AdjNode(u, g.adj[v]);
    g.E++;
}

Graph readUndirectedGraph() {
    int vertices, edges;
    if (!(std::cin >> vertices >> edges)) {
        throw std::runtime_error("Failed to read V and E");
    }

    Graph g(vertices);
    for (int i = 0; i < edges; ++i) {
        int u, v;
        if (!(std::cin >> u >> v)) {
            throw std::runtime_error("Failed to read an edge");
        }
        addEdge(g, u, v);
    }
    return g;
}

void printAdjList(const Graph& g) {
    for (int u = 0; u < g.V; ++u) {
        std::cout << u << ":";
        for (AdjNode* cur = g.adj[u]; cur; cur = cur->next) {
            std::cout << " " << cur->v;
        }
        std::cout << "\n";
    }
}

int main() {
    try {
        Graph g = readUndirectedGraph();
        printAdjList(g);
    } catch (const std::exception& ex) {
        std::cerr << ex.what() << "\n";
        return 1;
    }
    return 0;
}
