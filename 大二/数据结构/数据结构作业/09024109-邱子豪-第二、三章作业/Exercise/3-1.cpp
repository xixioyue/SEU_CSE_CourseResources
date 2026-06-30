//p138:1
#include <iostream>
#include <stdexcept>

template <class T>
class Stack {
public:
    Stack<T> Split();
    void Combine(Stack<T>& rhs);

    private:
    T* stack;
    int top;
    int capacity;

    void ChangeSize1D(T*& a, int oldCap, int newCap);
};

template <class T>
void Stack<T>::ChangeSize1D(T*& a, int oldCap, int newCap) {
    if (newCap < oldCap) return;
    T* temp = new T[newCap];
    for (int i = 0; i < oldCap; ++i) temp[i] = a[i];
    delete[] a;
    a = temp;
}

template <class T>
Stack<T> Stack<T>::Split() {
    int n = top + 1;
    int half = (n + 1) >> 1;

    Stack<T> upper;
    upper.capacity = capacity;
    upper.stack = new T[capacity];
    upper.top = n - half - 1;

    for (int i = 0; i <= upper.top; ++i) upper.stack[i] = stack[half + i];

    top = half - 1;
    return upper;
}

template <class T>
void Stack<T>::Combine(Stack<T>& rhs) {
    if (rhs.IsEmpty()) return;

    Stack<T> tmp;
    while (!rhs.IsEmpty()) { tmp.Push(rhs.Top()); rhs.Pop(); }
    while (!tmp.IsEmpty()) { Push(tmp.Top()); tmp.Pop(); }
}