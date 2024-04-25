#ifndef AVL_HEAD_H_2024_04_11
#define AVL_HEAD_H_2024_04_11

namespace lab618 {

template <class T>
class CMemoryManager {
 private:
  struct block {
    // Массив данных блока
    T* pdata;
    // Адрес следующего блока
    block* pnext;
    // Первая свободная ячейка
    int firstFreeIndex;
    // Число заполненных ячеек
    int usedCount;
  };

 public:
  class CException {
   public:
    CException() {}
  };

 public:
  /**
    _default_block_size - количество элементов в блоке данных
    isDeleteElementsOnDestruct - уничтожать элементы в деструкторе менеджера или
    проверять на наличие неосвобожденных функцией deleteObject элементов.
  */
  CMemoryManager(int _default_block_size,
                 bool isDeleteElementsOnDestruct = false)
      : m_blkSize(_default_block_size),
        m_pBlocks(nullptr),
        m_pCurrentBlk(nullptr),
        m_isDeleteElementsOnDestruct(isDeleteElementsOnDestruct) {}

  virtual ~CMemoryManager() { clear(); }

  // Получить адрес нового элемента из менеджера
  T* newObject() {
    block* p = m_pBlocks;
    T* res;
    while (p != nullptr) {
      if (p->firstFreeIndex < m_blkSize) {
        res = p->pdata + p->firstFreeIndex;
        p->firstFreeIndex = *reinterpret_cast<int*>(res);
        ++p->usedCount;
        new (res) T();
        return res;
      }
      p = p->pnext;
    }
    p = newBlock();
    res = p->pdata + p->firstFreeIndex;
    p->firstFreeIndex = *reinterpret_cast<int*>(res);
    ++p->usedCount;
    new (res) T();
    return res;
  }

  // Освободить элемент в менеджере
  bool deleteObject(T* p) {
    block* pblk = m_pBlocks;
    while (pblk != nullptr) {
      if (p - pblk->pdata < m_blkSize && p - pblk->pdata >= 0) {
        p->~T();
        int* ptr_next = reinterpret_cast<int*>(p);
        *ptr_next = pblk->firstFreeIndex;
        pblk->firstFreeIndex = static_cast<int>(p - pblk->pdata);
        --pblk->usedCount;
        return true;
      }
      pblk = pblk->pnext;
    }
    return false;
  }

  // Очистка данных, зависит от m_isDeleteElementsOnDestruct
  void clear() {
    block* p = m_pBlocks;
    m_pBlocks = nullptr;
    m_pCurrentBlk = nullptr;
    bool* is_int = nullptr;
    if (m_isDeleteElementsOnDestruct) {
      is_int = new bool[m_blkSize];
    }
    while (p != nullptr) {
      block* pnext = p->pnext;
      deleteBlock(p, is_int);
      p = pnext;
    }
    delete[] is_int;
  }

 private:
  // Создать новый блок данных. применяется в newObject
  block* newBlock() {
    block* p = new block;
    p->pdata = reinterpret_cast<T*>(new char[m_blkSize * sizeof(T)]);
    for (int i = 0; i < m_blkSize; i++) {
      int* next = reinterpret_cast<int*>(p->pdata + i);
      *next = i + 1;
    }
    p->pnext = nullptr;
    p->firstFreeIndex = 0;
    p->usedCount = 0;
    if (m_pBlocks == nullptr) {
      m_pBlocks = p;
      m_pCurrentBlk = p;
    } else {
      m_pCurrentBlk->pnext = p;
      m_pCurrentBlk = p;
    }
    return p;
  }

  // Освободить память блока данных. Применяется в clear
  void deleteBlock(block* p, bool* is_int) {
    if (m_isDeleteElementsOnDestruct) {
      for (int i = 0; i < m_blkSize; i++) {
        is_int[i] = false;
      }
      while (p->firstFreeIndex < m_blkSize) {
        int next = *reinterpret_cast<int*>(p->pdata + p->firstFreeIndex);
        is_int[p->firstFreeIndex] = true;
        p->firstFreeIndex = next;
      }
      for (int i = 0; i < m_blkSize; i++) {
        if (!is_int[i]) {
          p->pdata[i].~T();
        }
      }
      delete[] reinterpret_cast<char*>(p->pdata);
    } else if (p->usedCount > 0) {
      throw CException();
    }

    delete p;
  }

  // Размер блока
  int m_blkSize;
  // Начало списка блоков
  block* m_pBlocks;
  // Текущий блок
  block* m_pCurrentBlk;
  // Удалять ли элементы при освобождении
  bool m_isDeleteElementsOnDestruct;
};

template <class T, int (*Compare)(const T* pElement, const T* pElement2)>
class CAVLTree {
 private:
  struct leaf {
    T* pData;
    leaf* pLeft;
    leaf* pRight;
    int balanceFactor;
  };

 public:
  class CException {
   public:
    CException();
  };

 public:
  CAVLTree(int defaultBlockSize)
      : m_pRoot(nullptr), m_Memory(defaultBlockSize) {}

  virtual ~CAVLTree() { clear(); }

  bool add(T* pElement);

  bool update(T* pElement);

  T* find(const T& pElement);

  bool remove(const T& element);

  void clear();

 private:
  leaf* m_pRoot;
  CMemoryManager<leaf> m_Memory;

  struct LeafAdded {
    leaf* pleaf;
    bool is_added;
  };

  LeafAdded addPrivate(leaf* node, T* element);
  LeafAdded updatePrivate(leaf* node, T* element);
  T* findPrivate(leaf* node, const T& element);
  LeafAdded removePrivate(leaf* node, const T& element);
  void clearPrivate(leaf* node);

  leaf* RightSmallRotate(leaf* parent);
  leaf* LeftSmallRotate(leaf* parent);
  leaf* Balance(leaf* parent);

  leaf* GetMin(leaf* node);
  leaf* DeleteMin(leaf* node);

  int Height(leaf* node) { return (node != nullptr ? node->balanceFactor : 0); }

  int BalanceValue(leaf* node) {
    return (Height(node->pRight) - Height(node->pLeft));
  }

  void NewHeight(leaf* node) {
    node->balanceFactor =
        (Height(node->pLeft) > Height(node->pRight) ? Height(node->pLeft)
                                                    : Height(node->pRight)) +
        1;
  }
};

template <class T, int (*Compare)(const T*, const T*)>
bool CAVLTree<T, Compare>::update(T* pElement) {
  auto res = updatePrivate(m_pRoot, pElement);
  m_pRoot = res.pleaf;
  return res.is_added;
}

template <class T, int (*Compare)(const T*, const T*)>
bool CAVLTree<T, Compare>::remove(const T& element) {
  auto res = removePrivate(m_pRoot, element);
  m_pRoot = res.pleaf;
  return res.is_added;
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::leaf* CAVLTree<T, Compare>::DeleteMin(
    CAVLTree::leaf* node) {
  if (node->pLeft == nullptr) {
    return node->pRight;
  }
  node->pLeft = DeleteMin(node->pLeft);
  return Balance(node);
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::leaf* CAVLTree<T, Compare>::GetMin(
    CAVLTree::leaf* node) {
  while (node->pLeft != nullptr) {
    node = node->pLeft;
  }
  return node;
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::LeafAdded CAVLTree<T, Compare>::removePrivate(
    CAVLTree::leaf* node, const T& value) {
  if (node == nullptr) {
    return {nullptr, false};
  }
  bool flag = false;
  if (Compare(&value, node->pData) > 0) {  // value < node->pData
    auto res = removePrivate(node->pLeft, value);
    flag = (flag || res.is_added);
    node->pLeft = res.pleaf;
  } else if (Compare(&value, node->pData) < 0) {  // value > node->pData
    auto res = removePrivate(node->pRight, value);
    flag = (flag || res.is_added);
    node->pRight = res.pleaf;
  } else {
    leaf* left = node->pLeft;
    leaf* right = node->pRight;
    m_Memory.deleteObject(node);
    if (right == nullptr) {
      return {left, true};
    }
    leaf* min = GetMin(right);
    min->pRight = DeleteMin(right);
    min->pLeft = left;
    return {Balance(min), true};
  }
  return {Balance(node), flag};
}

template <class T, int (*Compare)(const T*, const T*)>
void CAVLTree<T, Compare>::clearPrivate(CAVLTree::leaf* node) {
  if (node == nullptr) {
    return;
  }

  clearPrivate(node->pLeft);
  clearPrivate(node->pRight);

  m_Memory.deleteObject(node);
}

template <class T, int (*Compare)(const T*, const T*)>
void CAVLTree<T, Compare>::clear() {
  clearPrivate(m_pRoot);
}

template <class T, int (*Compare)(const T*, const T*)>
bool CAVLTree<T, Compare>::add(T* pElement) {
  auto res = addPrivate(m_pRoot, pElement);
  m_pRoot = res.pleaf;
  return res.is_added;
}

template <class T, int (*Compare)(const T*, const T*)>
T* CAVLTree<T, Compare>::find(const T& pElement) {
  return findPrivate(m_pRoot, pElement);
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::LeafAdded CAVLTree<T, Compare>::updatePrivate(
    CAVLTree::leaf* node, T* element) {
  if (node == nullptr) {
    auto new_node = m_Memory.newObject();
    new_node->pData = element;
    new_node->balanceFactor = 1;
    new_node->pRight = nullptr;
    new_node->pLeft = nullptr;
    return LeafAdded{new_node, false};
  }
  bool flag = false;
  if (Compare(element, node->pData) > 0) {  // element < node->pData
    auto res = updatePrivate(node->pLeft, element);
    flag = (flag || res.is_added);
    node->pLeft = res.pleaf;
  } else if (Compare(element, node->pData) < 0) {  // element > node->pData
    auto res = updatePrivate(node->pRight, element);
    flag = (flag || res.is_added);
    node->pRight = res.pleaf;
  } else {
    node->pData = element;
    flag = true;
  }
  return {Balance(node), flag};
}

template <class T, int (*Compare)(const T*, const T*)>
T* CAVLTree<T, Compare>::findPrivate(CAVLTree::leaf* node, const T& element) {
  while (node != nullptr &&
         Compare(&element, node->pData) != 0) {  // element != node->pData
    node = (Compare(&element, node->pData) < 0)
               ? node->pRight
               : node->pLeft;  // element > node->pData
  }
  if (node != nullptr) {
    return node->pData;
  }
  return nullptr;
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::leaf* CAVLTree<T, Compare>::Balance(
    CAVLTree::leaf* parent) {
  NewHeight(parent);
  if (BalanceValue(parent) == 2) {
    if (BalanceValue(parent->pRight) < 0) {
      parent->pRight = RightSmallRotate(parent->pRight);
    }
    return LeftSmallRotate(parent);
  }
  if (BalanceValue(parent) == -2) {
    if (BalanceValue(parent->pLeft) > 0) {
      parent->pLeft = LeftSmallRotate(parent->pLeft);
    }
    return RightSmallRotate(parent);
  }
  return parent;
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::leaf* CAVLTree<T, Compare>::RightSmallRotate(
    CAVLTree::leaf* parent) {
  leaf* left_child = parent->pLeft;
  parent->pLeft = left_child->pRight;
  left_child->pRight = parent;
  NewHeight(parent);
  NewHeight(left_child);
  return left_child;
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::leaf* CAVLTree<T, Compare>::LeftSmallRotate(
    CAVLTree::leaf* parent) {
  leaf* right_child = parent->pRight;
  parent->pRight = right_child->pLeft;
  right_child->pLeft = parent;
  NewHeight(parent);
  NewHeight(right_child);
  return right_child;
}

template <class T, int (*Compare)(const T*, const T*)>
typename CAVLTree<T, Compare>::LeafAdded CAVLTree<T, Compare>::addPrivate(
    CAVLTree::leaf* node, T* element) {
  if (node == nullptr) {
    auto new_node = m_Memory.newObject();
    new_node->pData = element;
    new_node->balanceFactor = 1;
    new_node->pRight = nullptr;
    new_node->pLeft = nullptr;
    return LeafAdded{new_node, true};
  }
  bool flag = false;
  if (Compare(element, node->pData) > 0) {  // element < node->pData
    auto res = addPrivate(node->pLeft, element);
    flag = (flag || res.is_added);
    node->pLeft = res.pleaf;
  } else if (Compare(element, node->pData) < 0) {  // element > node->pData
    auto res = addPrivate(node->pRight, element);
    flag = (flag || res.is_added);
    node->pRight = res.pleaf;
  }
  return {Balance(node), flag};
}

};  // namespace lab618

#endif  // #define AVL_HEAD_H_2024_04_11
