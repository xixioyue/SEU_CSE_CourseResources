//P267:10、P272:1，2
//共三题
#include <iostream>
#include <stack>
using namespace std;

template<class T> class Tree;
template<class T> class InorderIterator;

template<class T>
class TreeNode {
    friend class Tree<T>;
    friend class InorderIterator<T>;
private:
    T data;
    TreeNode<T>* leftChild;
    TreeNode<T>* rightChild;
    
public:
    TreeNode(const T& value) : data(value), leftChild(0), rightChild(0) {}
};

template <class T>
class Tree {
    friend class InorderIterator<T>;
public:
    // 构造函数
    Tree() { root = 0; }
    
    // 析构函数
    ~Tree() {
        Destroy(root);
    }
    
    // 复制构造函数
    Tree(const Tree<T>& source) {
        root = CopyTree(source.root);
    }

    Tree<T>& operator=(const Tree<T>& source) {
        if (this != &source) {
            Destroy(root);
            root = CopyTree(source.root);
        }
        return *this;
    }

    Tree(const Tree<T>& bt1, const T& item, const Tree<T>& bt2) {
        root = new TreeNode<T>(item);
        root->leftChild = CopyTree(bt1.root);
        root->rightChild = CopyTree(bt2.root);
    }

    void Inorder() { Inorder(root); cout << endl; }
    void Preorder() { Preorder(root); cout << endl; }
    void Postorder() { Postorder(root); cout << endl; }
    void NonrecInorder() { NonrecInorder(); }

    bool IsEmpty() { return root == 0; }
    Tree<T> LeftSubtree() const;
    Tree<T> RightSubtree() const;
    T RootData() const;

    InorderIterator<T> CreateIterator() { return InorderIterator<T>(root); }

    Tree(const T& item) {
        root = new TreeNode<T>(item);
    }

    int CountLeaves() const;
    int CountLeaves(TreeNode<T>* currentNode) const;
    void SwapTree();
    void SwapTree(TreeNode<T>* currentNode);

private:
    TreeNode<T>* root;
    
    void Inorder(TreeNode<T>* currentNode);
    void Preorder(TreeNode<T>* currentNode);
    void Postorder(TreeNode<T>* currentNode);
    void Visit(TreeNode<T>* currentNode);

    void Destroy(TreeNode<T>* node);
    TreeNode<T>* CopyTree(TreeNode<T>* origNode);
};

template <class T>
void Tree<T>::Visit(TreeNode<T>* currentNode) {
    cout << currentNode->data << " ";
}

//前序遍历
template <class T>
void Tree<T>::Preorder(TreeNode<T>* currentNode) {
    if (currentNode) {
        Visit(currentNode);
        Preorder(currentNode->leftChild);
        Preorder(currentNode->rightChild);
    }
}

//中序遍历
template <class T>
void Tree<T>::Inorder(TreeNode<T>* currentNode) {
    if (currentNode) {
        Inorder(currentNode->leftChild);
        Visit(currentNode);
        Inorder(currentNode->rightChild);
    }
}

//后序遍历
template <class T>
void Tree<T>::Postorder(TreeNode<T>* currentNode) {
    if (currentNode) {
        Postorder(currentNode->leftChild);
        Postorder(currentNode->rightChild);
        Visit(currentNode);
    }
}

//非递归中序遍历
template <class T>
void Tree<T>::NonrecInorder() {
    stack<TreeNode<T>*> s;
    TreeNode<T>* currentNode = root;
    
    while (1) {
        while (currentNode) {
            s.push(currentNode);
            currentNode = currentNode->leftChild;
        }
        if (s.empty()) {
            cout << endl;
            return;
        }
        currentNode = s.top();
        s.pop();
        Visit(currentNode);
        currentNode = currentNode->rightChild;
    }
}

template <class T>
void Tree<T>::Destroy(TreeNode<T>* node) {
    if (node) {
        Destroy(node->leftChild);
        Destroy(node->rightChild);
        delete node;
    }
}

template <class T>
TreeNode<T>* Tree<T>::CopyTree(TreeNode<T>* origNode) {
    if (!origNode) return 0;
    TreeNode<T>* newNode = new TreeNode<T>(origNode->data);
    newNode->leftChild = CopyTree(origNode->leftChild);
    newNode->rightChild = CopyTree(origNode->rightChild);
    return newNode;
}

// 左子树
template <class T>
Tree<T> Tree<T>::LeftSubtree() const {
    Tree<T> leftTree;
    if (root) {
        leftTree.root = CopyTree(root->leftChild);
    }
    return leftTree;
}

// 右子树
template <class T>
Tree<T> Tree<T>::RightSubtree() const {
    Tree<T> rightTree;
    if (root) {
        rightTree.root = CopyTree(root->rightChild);
    }
    return rightTree;
}

// 根节点数据
template <class T>
T Tree<T>::RootData() const {
    if (IsEmpty()) {
        throw "Tree is empty";
    }
    return root->data;
}

// 中序遍历迭代器类
template<class T>
class InorderIterator {
public:
    InorderIterator(TreeNode<T>* root) { 
        currentNode = root;
    }
    T* Next();
    
private:
    stack<TreeNode<T>*> s;
    TreeNode<T>* currentNode;
};

template<class T>
T* InorderIterator<T>::Next() {
    while (currentNode) {
        s.push(currentNode);
        currentNode = currentNode->leftChild;
    }
    
    if (s.empty()) return 0;
    
    currentNode = s.top();
    s.pop();
    
    T& temp = currentNode->data;
    currentNode = currentNode->rightChild;
    
    return &temp;
}


//P272:1
template <class T>
int Tree<T>::CountLeaves() const {
    return CountLeaves(root);
}

template <class T>
int Tree<T>::CountLeaves(TreeNode<T>* currentNode) const {
    if (!currentNode) {
        return 0;
    }

    if (!currentNode->leftChild && !currentNode->rightChild) {
        return 1;
    }

    return CountLeaves(currentNode->leftChild) + CountLeaves(currentNode->rightChild);
}

//P272:2
template <class T>
void Tree<T>::SwapTree() {
    SwapTree(root);
}

template <class T>
void Tree<T>::SwapTree(TreeNode<T>* currentNode) {
    if (!currentNode) {
        return;
    }

    TreeNode<T>* temp = currentNode->leftChild;
    currentNode->leftChild = currentNode->rightChild;
    currentNode->rightChild = temp;

    SwapTree(currentNode->leftChild);
    SwapTree(currentNode->rightChild);
}