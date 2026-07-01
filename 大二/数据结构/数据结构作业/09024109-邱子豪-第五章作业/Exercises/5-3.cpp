//P296:1

template <class K, class E>
struct TreeNode {
    std::pair<K,E> data;
    TreeNode* left;
    TreeNode* right;
    TreeNode(const std::pair<K,E>& thePair)
        : data(thePair), left(nullptr), right(nullptr) {}
};

template <class K, class E>
class BST {
public:
    BST(): root(nullptr) {}
    ~BST() { clear(root); }

    bool Remove(const K& k) {
        removedFlag = false;
        root = removeRec(root, k);
        return removedFlag;
    }

private:
    TreeNode<K,E>* root = nullptr;
    bool removedFlag = false;

    void clear(TreeNode<K,E>* p) {
        if (!p) return;
        clear(p->left);
        clear(p->right);
        delete p;
    }

    TreeNode<K,E>* removeRec(TreeNode<K,E>* p, const K& k) {
        if (!p) { removedFlag = false; return nullptr; }
        if (k < p->data.first) {
            p->left = removeRec(p->left, k);
            return p;
        } else if (k > p->data.first) {
            p->right = removeRec(p->right, k);
            return p;
        } else {
            removedFlag = true;
            if (!p->left) {
                TreeNode<K,E>* r = p->right;
                delete p;
                return r;
            } else if (!p->right) {
                TreeNode<K,E>* l = p->left;
                delete p;
                return l;
            } else {
                TreeNode<K,E>* succParent = p;
                TreeNode<K,E>* succ = p->right;
                while (succ->left) {
                    succParent = succ;
                    succ = succ->left;
                }
                p->data = succ->data;
                if (succParent->left == succ) succParent->left = removeRec(succ, succ->data.first);
                else succParent->right = removeRec(succ, succ->data.first);
                return p;
            }
        }
    }
};
