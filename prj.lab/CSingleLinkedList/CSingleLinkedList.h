#ifndef LIST_H
#define LIST_H

#include <stdexcept>

namespace lab618 {
template <class T>
class CSingleLinkedList {
 private:
  struct leaf {
    T data;
    leaf* pNext;

    leaf(const T& _data, leaf* _pNext) : data(_data), pNext(_pNext) {}
  };

 public:
  class CIterator {
   public:
    CIterator() : m_pCurrent(nullptr), m_pBegin(nullptr) {}

    CIterator(leaf* p) : m_pCurrent(p), m_pBegin(nullptr) {}

    CIterator(const CIterator& src)
        : m_pBegin(src.m_pBegin), m_pCurrent(src.m_pCurrent) {}

    ~CIterator() {}

    CIterator& operator=(const CIterator& src) {
      if (this != &src) {
        m_pCurrent = src.m_pCurrent;
        m_pBegin = src.m_pBegin;
      }
      return *this;
    }

    bool operator!=(const CIterator& it) const {
      return m_pCurrent != it.m_pCurrent;
    }

    void operator++() {
      if (m_pCurrent) {
        m_pCurrent = m_pCurrent->pNext;
      } else if (m_pBegin) {
        m_pCurrent = m_pBegin;
        m_pBegin = nullptr;
      }
    }

    T& getData() {
      if (m_pCurrent) {
        return m_pCurrent->data;
      }
      throw std::out_of_range("Iterator is not valid");
    }

    T& operator*() { return getData(); }

    leaf* getLeaf() { return m_pCurrent; }

    void setLeaf(leaf* p) { m_pCurrent = p; }

    void setLeafPreBegin(leaf* p) { m_pBegin = p; }

    bool isValid() { return m_pCurrent != nullptr; }

   private:
    leaf* m_pBegin;
    leaf* m_pCurrent;
  };

 public:
  CSingleLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr) {}

  virtual ~CSingleLinkedList() { clear(); }

  void pushBack(const T& data) {
    leaf* newLeaf = new leaf(data, nullptr);
    if (!m_pBegin) {
      m_pBegin = newLeaf;
      m_pEnd = newLeaf;
    } else {
      m_pEnd->pNext = newLeaf;
      m_pEnd = newLeaf;
    }
  }

  void pushFront(const T& data) {
    leaf* newLeaf = new leaf(data, m_pBegin);
    m_pBegin = newLeaf;
    if (!m_pEnd) {
      m_pEnd = newLeaf;
    }
  }

  T popFront() {
    if (!m_pBegin) {
      throw std::out_of_range("List is empty");
    }
    T data = m_pBegin->data;
    leaf* temp = m_pBegin;
    m_pBegin = m_pBegin->pNext;
    delete temp;
    if (!m_pBegin) {
      m_pEnd = nullptr;
    }
    return data;
  }

  // изменяет состояние итератора. выставляет предыдущую позицию.
  void erase(CIterator& it) {
    if (!it.isValid()) {
      return;
    }
    leaf* current = it.getLeaf();
    if (current == m_pBegin) {
      m_pBegin = current->pNext;
      if (!m_pBegin) {
        m_pEnd = nullptr;
      }
      it.setLeaf(nullptr);
      it.setLeafPreBegin(m_pBegin);
    } else {
      leaf* prev = m_pBegin;
      while (prev && prev->pNext != current) {
        prev = prev->pNext;
      }
      if (prev) {
        prev->pNext = current->pNext;
      }
      if (m_pEnd == current) {
         m_pEnd = prev;
      }
      it.setLeaf(prev);
    }
    delete current;
  }

  int getSize() {
    int size = 0;
    leaf* current = m_pBegin;
    while (current) {
      ++size;
      current = current->pNext;
    }
    return size;
  }

  void clear() {
    leaf* cp_begin = m_pBegin;
    while (cp_begin) {
      leaf* temp = cp_begin;
      cp_begin = cp_begin->pNext;
      delete temp;
    }
    m_pBegin = cp_begin;
    m_pEnd = nullptr;
  }

  CIterator begin() { return CIterator(m_pBegin); }

 private:
  leaf *m_pBegin, *m_pEnd;
};
};  // namespace lab618

#endif
