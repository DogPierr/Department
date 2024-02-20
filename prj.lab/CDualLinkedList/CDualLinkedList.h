#ifndef MIPT2024S_3_CDUALLINKEDLIST_H
#define MIPT2024S_3_CDUALLINKEDLIST_H

#include <stdexcept>

namespace lab618 {
template <class T>
class CDualLinkedList {
 private:
  struct leaf {
    T data;
    leaf *pNext, *pPrev;
    leaf(T& _data, leaf* _pPrev, leaf* _pNext)
        : data(_data), pNext(_pNext), pPrev(_pPrev) {}
  };

 public:
  class CIterator {
   public:
    CIterator() : m_pCurrent(nullptr), m_pBegin(nullptr), m_pEnd(nullptr) {}
    CIterator(leaf* p) : m_pCurrent(p), m_pBegin(nullptr), m_pEnd(nullptr) {}
    CIterator(const CIterator& src)
        : m_pCurrent(src.m_pCurrent),
          m_pBegin(src.m_pBegin),
          m_pEnd(src.m_pEnd) {}
    ~CIterator() {}

    CIterator& operator=(const CIterator& src) {
      if (this != &src) {
        m_pCurrent = src.m_pCurrent;
        m_pBegin = src.m_pBegin;
        m_pEnd = src.m_pEnd;
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

    void operator--() {
      if (m_pCurrent) {
        m_pCurrent = m_pCurrent->pPrev;
      } else if (m_pEnd) {
        m_pCurrent = m_pEnd;
        m_pEnd = nullptr;
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

    // применяется в erase и eraseAndNext
    void setLeaf(leaf* p) { m_pCurrent = p; }

    // применяется в erase и eraseAndNext
    void setLeafPreBegin(leaf* p) { m_pBegin = p; }

    // применяется в erase и eraseAndNext
    void setLeafPostEnd(leaf* p) { m_pEnd = p; }

    bool isValid() { return m_pCurrent != nullptr; }

   private:
    // храним голову списка, если мы находимся перед началом
    leaf* m_pBegin;
    // храним текущее положение
    leaf* m_pCurrent;
    // храним конец списка, если мы находимся после конца
    leaf* m_pEnd;
  };

 public:
  CDualLinkedList() : m_pBegin(nullptr), m_pEnd(nullptr){};

  virtual ~CDualLinkedList() { clear(); };

  void pushBack(T& data) {
    leaf* newLeaf = new leaf(data, m_pEnd, nullptr);
    if (!m_pBegin) {
      m_pBegin = newLeaf;
      m_pEnd = newLeaf;
    } else {
      m_pEnd->pNext = newLeaf;
      m_pEnd = newLeaf;
    }
  }

  T popBack() {
    if (!m_pEnd) {
      throw std::out_of_range("List is empty");
    }
    T data = m_pEnd->data;
    leaf* temp = m_pEnd;
    m_pEnd = m_pEnd->pPrev;
    if (m_pEnd) {
      m_pEnd->pNext = nullptr;
    } else {
      m_pBegin = nullptr;
    }
    delete temp;
    return data;
  }

  void pushFront(T& data) {
    leaf* newLeaf = new leaf(data, nullptr, m_pBegin);
    if (!m_pBegin) {
      m_pBegin = newLeaf;
      m_pEnd = newLeaf;
    } else {
      m_pBegin->pPrev = newLeaf;
      m_pBegin = newLeaf;
    }
  }

  T popFront() {
    if (!m_pBegin) {
      throw std::out_of_range("List is empty");
    }
    T data = m_pBegin->data;
    leaf* temp = m_pBegin;
    m_pBegin = m_pBegin->pNext;
    if (m_pBegin) {
      m_pBegin->pPrev = nullptr;
    } else {
      m_pEnd = nullptr;
    }
    delete temp;
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
      if (m_pBegin) {
        m_pBegin->pPrev = nullptr;
      } else {
        m_pEnd = nullptr;
      }
      it.setLeaf(nullptr);
      it.setLeafPreBegin(m_pBegin);
    } else if (current == m_pEnd) {
      m_pEnd = current->pPrev;
      if (m_pEnd) {
        m_pEnd->pNext = nullptr;
      }
      it.setLeaf(m_pEnd);
    } else {
      it.setLeaf(current->pPrev);
      current->pPrev->pNext = current->pNext;
      current->pNext->pPrev = current->pPrev;
    }
    delete current;
  }

  // изменяет состояние итератора. выставляет следующую позицию.
  void eraseAndNext(CIterator& it) {
    if (!it.isValid()) {
      return;
    }
    leaf* current = it.getLeaf();
    if (current == m_pBegin) {
      m_pBegin = current->pNext;
      if (m_pBegin) {
        m_pBegin->pPrev = nullptr;
      } else {
        m_pEnd = nullptr;
      }
      it.setLeaf(m_pBegin);
    } else if (current == m_pEnd) {
      m_pEnd = current->pPrev;
      if (m_pEnd) {
        m_pEnd->pNext = nullptr;
      }
      it.setLeaf(nullptr);
      it.setLeafPostEnd(m_pEnd);
    } else {
      it.setLeaf(current->pNext);
      current->pPrev->pNext = current->pNext;
      current->pNext->pPrev = current->pPrev;
    }
    delete current;
  }

  int getSize() {
    int size = 0;
    leaf* current = m_pBegin;
    while (current) {
      size++;
      current = current->pNext;
    }
    return size;
  }

  void clear() {
    while (m_pBegin) {
      leaf* next = m_pBegin->pNext;
      next->pPrev = nullptr;
      delete m_pBegin;
      m_pBegin = next;
    }
    m_pEnd = nullptr;
  }

  CIterator begin() { return CIterator(m_pBegin); }

  CIterator end() { return CIterator(m_pEnd); }

 private:
  // храним голову и хвост списка
  leaf *m_pBegin, *m_pEnd;
};
};  // namespace lab618

#endif  // MIPT2024S_3_CDUALLINKEDLIST_H
