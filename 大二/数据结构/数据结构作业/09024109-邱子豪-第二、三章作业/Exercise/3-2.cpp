//P147:3
#include <iostream>
#include <algorithm>
#include <stdexcept>

template <class T>
class Queue {
public:
    Queue<T> split();
private:
    T*  queue;
    int front;
    int rear;
    int capacity;

    void resize(int newCap);
};

template <class T>
void Queue<T>::resize(int newCap) {
    T* newQ = new T[newCap];
    int start = (front + 1) % capacity;
    int sz    = (rear + capacity - start + 1) % capacity;
    if (start <= rear)
        std::copy(queue + start, queue + rear + 1, newQ);
    else {
        std::copy(queue + start, queue + capacity, newQ);
        std::copy(queue, queue + rear + 1, newQ + (capacity - start));
    }
    delete[] queue;
    queue    = newQ;
    front    = newCap - 1;
    rear     = sz - 1;
    capacity = newCap;
}

template <class T>
Queue<T> Queue<T>::split() {
    Queue<T> evenQ;
    evenQ.resize(capacity);
    int sz = (rear + capacity - front) % capacity;
    for (int i = 1; i <= sz; ++i) {
        T val = Front();
        Pop();
        if (i & 1)   Push(val);
        else         evenQ.Push(val);
    }
    return evenQ;
}