//P296:2
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>

template <class K, class E>
struct Node {
    std::pair<K,E> data;
    Node* left;
    Node* right;
    Node(const std::pair<K,E>& p): data(p), left(nullptr), right(nullptr) {}
};

template <class K, class E>
class SimpleBST {
public:
    SimpleBST(): root(nullptr) {}
    ~SimpleBST() { clear(root); }

    void Insert(const std::pair<K,E>& thePair) {
        if (!root) { root = new Node<K,E>(thePair); return; }
        Node<K,E>* p = root;
        Node<K,E>* parent = nullptr;
        while (p) {
            parent = p;
            if (thePair.first < p->data.first) p = p->left;
            else if (thePair.first > p->data.first) p = p->right;
            else { p->data.second = thePair.second; return; } // update duplicate
        }
        if (thePair.first < parent->data.first) parent->left = new Node<K,E>(thePair);
        else parent->right = new Node<K,E>(thePair);
    }

    int Height() const { return heightRec(root); }

private:
    Node<K,E>* root = nullptr;

    void clear(Node<K,E>* p) {
        if (!p) return;
        clear(p->left);
        clear(p->right);
        delete p;
    }

    int heightRec(Node<K,E>* p) const {
        if (!p) return 0;
        int hl = heightRec(p->left);
        int hr = heightRec(p->right);
        return 1 + std::max(hl, hr);
    }
};

int main() {
    std::vector<int> ns = {100, 500, 1000, 2000, 3000, 10000};
    const int trials = 30;
    std::mt19937 rng(std::random_device{}());

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "n,avg_height,avg_height_div_log2n\n";

    for (int n : ns) {
        double sumH = 0.0, sumR = 0.0;
        std::vector<int> elems(n);
        for (int i = 0; i < n; ++i) elems[i] = i;
        for (int t = 0; t < trials; ++t) {
            std::shuffle(elems.begin(), elems.end(), rng);
            SimpleBST<int,int> tree;
            for (int x : elems) tree.Insert({x,x});
            int h = tree.Height();
            sumH += h;
            sumR += (h / std::log2((double)n));
        }
        double avgH = sumH / trials;
        double avgR = sumR / trials;
        std::cout << n << "," << avgH << "," << avgR << "\n";
    }
    return 0;
}