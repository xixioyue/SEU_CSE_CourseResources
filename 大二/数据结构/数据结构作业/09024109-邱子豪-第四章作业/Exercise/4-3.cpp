// P194:4
template <class T> class Chain;

template <class T>
class ChainNode {
    friend class Chain<T>;
public:
    ChainNode(const T& d, ChainNode<T>* l = nullptr) : data(d), link(l) {}
private:
    T data;
    ChainNode<T>* link;
};

template <class T>
class ChainIterator {
public:
    using Node = ChainNode<T>;

    ChainIterator(Node* ptr = nullptr) : current(ptr) {}

    T& operator*() const {
        return current->data;
    }

    ChainIterator& operator++() {
        if (current) current = current->link;
        return *this;
    }

    bool operator==(const ChainIterator& rhs) const { return current == rhs.current; }
    bool operator!=(const ChainIterator& rhs) const { return current != rhs.current; }

private:
    Node* current;
};

template <class T>
class Chain {
public:
    using Node = ChainNode<T>;
    using iterator = ChainIterator<T>;

    Chain() : first(nullptr) {}
    ~Chain() {
        clear();
    }

    Chain(const Chain&) = delete;
    Chain& operator=(const Chain&) = delete;

    void push_back(const T& x) {
        if (!first) {
            first = new Node(x, nullptr);
            return;
        }
        Node* p = first;
        while (p->link) p = p->link;
        p->link = new Node(x, nullptr);
    }

    iterator begin() { return iterator(first); }
    iterator end() { return iterator(nullptr); }

    iterator begin() const { return iterator(first); }
    iterator end() const { return iterator(nullptr); }

    void clear() {
        Node* p = first;
        while (p) {
            Node* q = p->link;
            delete p;
            p = q;
        }
        first = nullptr;
    }

private:
    Node* first;
};

long long sum_products_offset5(const Chain<int>& chain) {
    auto it1 = chain.begin();
    auto it2 = chain.begin();

    for (int k = 0; k < 5; ++k) {
        if (it2 == chain.end()) return 0;
        ++it2;
    }

    long long sum = 0;
    while (it2 != chain.end()) {
        sum += static_cast<long long>(*it1) * static_cast<long long>(*it2);
        ++it1;
        ++it2;
    }
    return sum;
}