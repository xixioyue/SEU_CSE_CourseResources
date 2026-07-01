//P184:6
class ChainNode {
    friend class Chain;
public:
    ChainNode(int element = 0, ChainNode* next = nullptr)
        : data(element), link(next) {}
private:
    int data;
    ChainNode* link;
};

class Chain {
public:
    Chain() : first(nullptr), l(nullptr), r(nullptr) {}

//(a)
    void moveRight(int n) {
        for (int i = 0; i < n; ++i) {
            if (r == nullptr) {
                break;
            }
            if (r->link == nullptr) {
                l = r;
                r = nullptr;
                break;
            } else {
                ChainNode* next = r->link;
                r->link = l;
                l = r;
                r = next;
            }
        }
    }

//(b)
    void moveLeft(int n) {
        for (int i = 0; i < n; ++i) {
            if (l == nullptr) {
                break;
            } else {
                ChainNode* new_r = l;
                ChainNode* new_l = l->link;
                new_r->link = r;
                r = new_r;
                l = new_l;
            }
        }
    }

private:
    ChainNode* first;
    ChainNode* l;
    ChainNode* r;
};