    //P183:2;P184:5;
class ChainNode{
friend class Chain;
public:
    ChainNode(int element=0,ChainNode* next=0)
    {data=element;link=next;}
private:
    int data;
    ChainNode *link;
};

class Chain{
public:
    Chain(){first=0;}
    void Delete(ChainNode*);
    Chain Merge(Chain&,Chain&);
private:
    ChainNode* first;
};

//P183:2;时间复杂度为O(n)
void Chain::Delete(ChainNode *x)
{
    if(x==first)
    {
        first=first->link;
        delete x;
        return;
    }

    ChainNode *p = first;
    while (p->link != nullptr && p->link != x) {
        p = p->link;
    }
    if (p->link == x) {
        p->link = x->link;
        delete x;
    }
}

// P184:5;时间复杂度为O(n+m)
Chain Chain::Merge(Chain &x,Chain &y)
{
    Chain temp;
    ChainNode* px=x.first;
    ChainNode* py=y.first;

    x.first=nullptr;
    y.first=nullptr;

    ChainNode* tail=nullptr;

    if(px->data<py->data){
        temp.first=px;
        tail=px;
    }
    else{
        temp.first=py;
        tail=py;
    }

    while(px->link!=nullptr&&py->link!=nullptr)
    {
        if(px->data<py->data){
            tail->link=px;
            tail=px;
            px=px->link;
        }
        else{
            tail->link=py;
            tail=py;
            py=py->link;
        }
    }
    if(px->link!=nullptr)
    {
        tail->link=px;
        while(px->link) px=px->link;
        tail=px;
    }
    if(py->link!=nullptr)
    {
        tail->link=py;
        while(py->link) py=py->link;
        tail=py;
    }
}