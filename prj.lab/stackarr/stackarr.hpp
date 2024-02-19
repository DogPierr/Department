#ifndef STACKARR_HPP
#define STACKARR_HPP

#include <algorithm>

template <typename T>
class StackArray {
public:
    StackArray();
    StackArray(const StackArray& other);
    StackArray(StackArray&& other) noexcept;
    StackArray& operator=(const StackArray& other);
    StackArray& operator=(StackArray&& other) noexcept;

    void Push(const T& value);
    void Pop();
    const T &Top() const;
    T &Top();

    bool IsEmpty() const;
    size_t Size() const;

    ~StackArray();

private:
    void Resize(size_t newCapacity);
    void Swap(StackArray& other);

    size_t size_;
    size_t capacity_;
    T* data_;
};

template<typename T>
StackArray<T>::StackArray() : size_(0), capacity_(0), data_(new T[capacity_]) {}

template<typename T>
StackArray<T>::StackArray(const StackArray &other) : size_(other.size_), capacity_(other.capacity_), data_(new T[capacity_]) {
    for (size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

template<typename T>
StackArray<T>::StackArray(StackArray &&other) noexcept : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
    other.size_ = 0;
    other.capacity_ = 0;
    other.data_ = nullptr;
}

template<typename T>
StackArray<T> &StackArray<T>::operator=(const StackArray &other) {
    StackArray<T> tmp(other);
    tmp.Swap(*this);
    return *this;
}

template<typename T>
StackArray<T> &StackArray<T>::operator=(StackArray &&other) noexcept {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    return *this;
}

template<typename T>
void StackArray<T>::Push(const T& value) {
    if (size_ == capacity_) {
        Resize(capacity_ != 0 ? capacity_ * 2 : 1);
    }
    data_[size_++] = value;
}

template<typename T>
void StackArray<T>::Pop() {
    if (size_ == 0) {
        return;
    }
    --size_;
}

template<typename T>
const T &StackArray<T>::Top() const {
    return data_[size_ - 1];
}

template<typename T>
T &StackArray<T>::Top() {
    return data_[size_ - 1];
}

template<typename T>
bool StackArray<T>::IsEmpty() const {
    return (size_ == 0);
}

template<typename T>
size_t StackArray<T>::Size() const {
    return size_;
}

template<typename T>
StackArray<T>::~StackArray() {
    delete[] data_;
}

template<typename T>
void StackArray<T>::Resize(size_t newCapacity) {
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size_; ++i) {
        newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
}

template<typename T>
void StackArray<T>::Swap(StackArray &other) {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

using StackArr = StackArray<float>;
#endif