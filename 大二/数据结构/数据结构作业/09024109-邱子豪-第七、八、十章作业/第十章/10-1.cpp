#include <algorithm>
#include <iostream>
#include <vector>

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    explicit Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

int Height(Node* node) { return node ? node->height : 0; }

int BalanceFactor(Node* node) { return node ? Height(node->left) - Height(node->right) : 0; }

void UpdateHeight(Node* node) {
    if (!node) return;
    node->height = 1 + std::max(Height(node->left), Height(node->right));
}

Node* RotateRight(Node* y) {
    Node* x = y->left;
    Node* t2 = x->right;
    x->right = y;
    y->left = t2;
    UpdateHeight(y);
    UpdateHeight(x);
    return x;
}

Node* RotateLeft(Node* x) {
    Node* y = x->right;
    Node* t2 = y->left;
    y->left = x;
    x->right = t2;
    UpdateHeight(x);
    UpdateHeight(y);
    return y;
}

Node* Rebalance(Node* node) {
    if (!node) return nullptr;
    UpdateHeight(node);
    int balance = BalanceFactor(node);

    if (balance > 1 && BalanceFactor(node->left) >= 0) {
        return RotateRight(node);
    }
    if (balance > 1 && BalanceFactor(node->left) < 0) {
        node->left = RotateLeft(node->left);
        return RotateRight(node);
    }
    if (balance < -1 && BalanceFactor(node->right) <= 0) {
        return RotateLeft(node);
    }
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

Node* MaxNode(Node* node) {
    while (node && node->right) node = node->right;
    return node;
}

Node* Delete(Node* root, int key) {
    if (!root) return nullptr;

    if (key < root->key) {
        root->left = Delete(root->left, key);
    } else if (key > root->key) {
        root->right = Delete(root->right, key);
    } else {
        if (!root->left || !root->right) {
            Node* child = root->left ? root->left : root->right;
            delete root;
            return child;
        }

        Node* predecessor = MaxNode(root->left);
        root->key = predecessor->key;
        root->left = Delete(root->left, predecessor->key);
    }

    return Rebalance(root);
}

void InOrder(Node* node) {
    if (!node) return;
    InOrder(node->left);
    std::cout << node->key << "(h=" << node->height << ") ";
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
    std::vector<int> keys{30, 20, 40, 10, 25, 35, 50, 5, 15};
    for (int k : keys) root = Insert(root, k);

    std::cout << "Initial AVL tree (in-order with heights): ";
    InOrder(root);
    std::cout << "\n";

    root = Delete(root, 20);
    std::cout << "After deleting 20: ";
    InOrder(root);
    std::cout << "\n";

    root = Delete(root, 30);
    std::cout << "After deleting 30: ";
    InOrder(root);
    std::cout << "\n";

    root = Delete(root, 10);
    std::cout << "After deleting 10: ";
    InOrder(root);
    std::cout << "\n";

    FreeTree(root);
    return 0;
}
