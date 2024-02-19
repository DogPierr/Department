// 2024 by Polevoi Dmitry under Unlicense
//
#ifndef STACKLST_HPP
#define STACKLST_HPP

#include <algorithm>

template<typename T>
class Stack {
public:
    Stack() = default;
    Stack(const Stack &other);
    Stack(Stack &&other) noexcept;
    Stack &operator=(const Stack &other);
    Stack &operator=(Stack &&other) noexcept;

    void Push(const T &value);
    void Pop();
    bool IsEmpty() const;
    const T &Top() const;
    T &Top();

    size_t Size() const;
    ~Stack();
private:
    struct Node {
        T data;
        Node *next;

        Node(const T &value);
        Node(T &&value);
    };

    void Swap(Stack& other) noexcept;

    Node *top_;
    size_t size_;
};

template<typename T>
Stack<T>::Stack(const Stack &other) {
    if (other.top_ == nullptr) {
        top_ = nullptr;
        return;
    }

    size_ = other.size_;
    top_ = new Node(other.top_->data);
    Node *current = top_;
    Node *next = other.top_->next;

    while (next != nullptr) {
        current->next = new Node(next->data);
        current = current->next;
        next = next->next;
    }
}

template<typename T>
Stack<T>::Stack(Stack &&other) noexcept {
    top_ = other.top_;
    size_ = other.size_;
    other.top_ = nullptr;
    other.size_ = 0;
}

template<typename T>
Stack<T> &Stack<T>::operator=(const Stack &other) {
    Stack<T> tmp(other);
    tmp.Swap(*this);
    return *this;
}

template<typename T>
Stack<T> &Stack<T>::operator=(Stack &&other) noexcept {
    std::swap(top_, other.top_);
    std::swap(size_, other.size_);
    return *this;
}

template<typename T>
void Stack<T>::Push(const T &value) {
    Node* new_node = new Node(value);
    new_node->next = top_;
    top_ = new_node;
    ++size_;
}

template<typename T>
void Stack<T>::Pop() {
    if (size_ == 0) {
        return;
    }

    Node* temp = top_;
    top_ = top_->next;
    delete temp;
    --size_;
}

template<typename T>
bool Stack<T>::IsEmpty() const {
    return (size_ == 0);
}

template<typename T>
const T &Stack<T>::Top() const {
    return top_->data;
}

template<typename T>
T &Stack<T>::Top() {
    return top_->data;
}

template<typename T>
size_t Stack<T>::Size() const {
    return size_;
}

template<typename T>
Stack<T>::Node::Node(const T &value) : data(value), next(nullptr) {}

template<typename T>
Stack<T>::Node::Node(T &&value) : data(std::move(value)), next(nullptr) {}

template<typename T>
void Stack<T>::Swap(Stack& other) noexcept {
    std::swap(top_, other.top_);
}

template<typename T>
Stack<T>::~Stack() {
    Node* current = top_;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

using StackList = Stack<float>;
#endif