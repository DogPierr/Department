#ifndef MEMORY_MANAGER_HEAD_H_2024_03_07
#define MEMORY_MANAGER_HEAD_H_2024_03_07

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
};  // namespace lab618

#endif  // #define MEMORY_MANAGER_HEAD_H_2024_03_07