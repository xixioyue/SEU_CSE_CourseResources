#include <algorithm>
#include <iostream>
#include <vector>

struct Node {
    int key;
    int height;
    int size;
    int lsize;
    Node* left;
    Node* right;
    explicit Node(int k) : key(k), height(1), size(1), lsize(0), left(nullptr), right(nullptr) {}
};

int Height(Node* node) { return node ? node->height : 0; }
int Size(Node* node) { return node ? node->size : 0; }

void Update(Node* node) {
    if (!node) return;
    node->lsize = Size(node->left);
    node->size = node->lsize + 1 + Size(node->right);
    node->height = 1 + std::max(Height(node->left), Height(node->right));
}

int BalanceFactor(Node* node) { return node ? Height(node->left) - Height(node->right) : 0; }

Node* RotateRight(Node* y) {
    Node* x = y->left;
    Node* t2 = x->right;
    x->right = y;
    y->left = t2;
    Update(y);
    Update(x);
    return x;
}

Node* RotateLeft(Node* x) {
    Node* y = x->right;
    Node* t2 = y->left;
    y->left = x;
    x->right = t2;
    Update(x);
    Update(y);
    return y;
}

Node* Rebalance(Node* node) {
    if (!node) return nullptr;
    Update(node);
    int balance = BalanceFactor(node);
    if (balance > 1 && BalanceFactor(node->left) >= 0) return RotateRight(node);
    if (balance > 1 && BalanceFactor(node->left) < 0) {
        node->left = RotateLeft(node->left);
        return RotateRight(node);
    }
    if (balance < -1 && BalanceFactor(node->right) <= 0) return RotateLeft(node);
    if (balance < -1 && BalanceFactor(node->right) > 0) {
        node->right = RotateRight(node->right);
        return RotateLeft(node);
    }
    return node;
}

Node* Insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->key) {
        root->left = Insert(root->left, key);
    } else if (key > root->key) {
        root->right = Insert(root->right, key);
    } else {
        return root;
    }
    return Rebalance(root);
}

Node* DeleteKth(Node* root, int k) {
    if (!root) return nullptr;
    Update(root);
    int left_count = root->lsize;
    if (k < left_count + 1) {
        root->left = DeleteKth(root->left, k);
    } else if (k > left_count + 1) {
        root->right = DeleteKth(root->right, k - left_count - 1);
    } else {
        if (!root->left || !root->right) {
            Node* child = root->left ? root->left : root->right;
            delete root;
            return child;
        }
        Node* pred = root->left;
        while (pred->right) pred = pred->right;
        root->key = pred->key;
        root->left = DeleteKth(root->left, Size(root->left));
    }
    return Rebalance(root);
}

void InOrder(Node* node) {
    if (!node) return;
    InOrder(node->left);
    std::cout << node->key << "(sz=" << node->size << ",l=" << node->lsize << ") ";
    InOrder(node->right);
}

void FreeTree(Node* node) {
    if (!node) return;
    FreeTree(node->left);
    FreeTree(node->right);
    delete node;
}

int main() {
    Node* root = nullptr;
    std::vector<int> keys{30, 20, 40, 10, 25, 35, 50, 5, 15, 27, 45};
    for (int k : keys) root = Insert(root, k);

    std::cout << "Initial tree in-order: ";
    InOrder(root);
    std::cout << "\n";

    root = DeleteKth(root, 3);
    std::cout << "After deleting 3rd smallest: ";
    InOrder(root);
    std::cout << "\n";

    root = DeleteKth(root, 5);
    std::cout << "After deleting 5th smallest: ";
    InOrder(root);
    std::cout << "\n";

    root = DeleteKth(root, 1);
    std::cout << "After deleting 1st smallest: ";
    InOrder(root);
    std::cout << "\n";

    FreeTree(root);
    return 0;
}
