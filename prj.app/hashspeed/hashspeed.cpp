#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

#include "Hashmap/hash.h"
#include "avltree/avltree.h"
#include "sort/sort.h"
// #include "mm/mm.h"

#define LOGFILE "../prj.app/hashspeed/hashspeed.csv"
#define BLOCKSIZE 100'000
#define HASHTABLESIZE 1'000'000

const auto hash_func = std::hash<std::string>{};

std::string generateRandomString(int minLength, int maxLength) {
  static const char charset[] =
      "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(minLength, maxLength);

  int length = distribution(gen);
  std::string randomString;

  for (int i = 0; i < length; ++i) {
    randomString += charset[gen() % (sizeof(charset) - 1)];
  }

  return randomString;
}

class TimerProfiler {
 public:
  TimerProfiler(std::fstream& file, const char* function_name, size_t amount)
      : file_(file),
        start_(std::chrono::high_resolution_clock::now()),
        function_name_(function_name),
        amount_(amount) {}

  ~TimerProfiler() {
    auto end = std::chrono::high_resolution_clock::now();
    long duration =
        std::chrono::duration_cast<std::chrono::microseconds>(end - start_)
            .count();

    file_ << function_name_ << ',' << duration << ',' << amount_ << '\n';
  }

 private:
  std::fstream& file_;
  std::chrono::high_resolution_clock::time_point start_;
  const char* function_name_;
  size_t amount_;
};

std::fstream fileLog(LOGFILE, std::fstream::out | std::fstream::trunc);

struct Object {
  Object(const std::string& key1, const std::string& key2)
      : key1(key1), key2(key2) {}
  std::string key1;
  std::string key2;
};

int ObjectComparator(const Object* a, const Object* b) {
  if (a->key1 < b->key1) {
    return 1;
  } else if (a->key1 == b->key1) {
    return 0;
  }
  return -1;
}

unsigned int ObjectHash(const Object* pElement) {
  return hash_func(pElement->key1);
}

class SortedArray {
 public:
  SortedArray(Object** objects, size_t size) {
    data_ = objects;
    size_ = size;
    TimerProfiler t(fileLog, __FUNCTION__, size_);
    templates::quickSort(data_, size_, ObjectComparator);
  }

  bool search(const Object* object, const char* stringName) {
    //    TimerProfiler t(fileLog, stringName, size_);
    auto it =
        std::binary_search(data_, data_ + size_, object, ObjectComparator);
    return it;
  }

  void searchSameSort() {
    TimerProfiler t(fileLog, __FUNCTION__, size_);
    for (size_t i = 0; i < size_; ++i) {
      search(data_[i], __FUNCTION__);
    }
  }

  void searchRandomSort(Object** randomData, size_t randomSize) {
    TimerProfiler t(fileLog, __FUNCTION__, randomSize);
    for (size_t i = 0; i < randomSize; ++i) {
      search(randomData[i], __FUNCTION__);
    }
  }

 private:
  Object** data_;
  size_t size_;
};

class AvlTree {
 public:
  AvlTree(Object** objects, size_t size) : tree_(BLOCKSIZE) {
    data_ = objects;
    size_ = size;
    TimerProfiler t(fileLog, "AVLTreeBuild", size_);
    for (size_t i = 0; i < size_; ++i) {
      tree_.add(data_[i]);
    }
  }

  Object* search(const Object& object, const char* stringName) {
    //    TimerProfiler t(fileLog, stringName, size_);
    return tree_.find(object);
  }

  void searchSameAVL() {
    TimerProfiler t(fileLog, __FUNCTION__, size_);
    for (size_t i = 0; i < size_; ++i) {
      if (search(*data_[i], __FUNCTION__) != data_[i]) std::cout << "err\n";
    }
  }

  void searchRandomAVL(Object** randomData, size_t randomSize) {
    TimerProfiler t(fileLog, __FUNCTION__, randomSize);
    for (size_t i = 0; i < randomSize; ++i) {
      search(*randomData[i], __FUNCTION__);
    }
  }

  void remove(const Object& object, const char* stringName) {
    //    TimerProfiler t(fileLog, stringName, size_);
    tree_.remove(object);
  }

  void removeSameAVL() {
    TimerProfiler t(fileLog, __FUNCTION__, size_);
    for (size_t i = 0; i < size_; ++i) {
      remove(*data_[i], __FUNCTION__);
    }
  }

 private:
  Object** data_;
  size_t size_;
  lab618::CAVLTree<Object, &ObjectComparator> tree_;
};

class HashMap {
 public:
  HashMap(Object** objects, size_t size) : map_(HASHTABLESIZE, BLOCKSIZE) {
    data_ = objects;
    size_ = size;
    TimerProfiler t(fileLog, "HashMapBuild", size_);
    for (size_t i = 0; i < size_; ++i) {
      map_.add(data_[i]);
    }
  }

  void search(const Object& object, const char* stringName) {
    //    TimerProfiler t(fileLog, stringName, size_);
    map_.find(object);
  }

  void searchSameHash() {
    TimerProfiler t(fileLog, __FUNCTION__, size_);
    for (size_t i = 0; i < size_; ++i) {
      search(*data_[i], __FUNCTION__);
    }
  }

  void searchRandomHash(Object** randomData, size_t randomSize) {
    TimerProfiler t(fileLog, __FUNCTION__, randomSize);
    for (size_t i = 0; i < randomSize; ++i) {
      search(*randomData[i], __FUNCTION__);
    }
  }

  void remove(const Object& object, const char* stringName) {
    //    TimerProfiler t(fileLog, stringName, size_);
    map_.remove(object);
  }

  void removeSameHash() {
    TimerProfiler t(fileLog, __FUNCTION__, size_);
    for (size_t i = 0; i < size_; ++i) {
      remove(*data_[i], __FUNCTION__);
    }
  }

 private:
  Object** data_;
  size_t size_;
  lab618::CHash<Object, &ObjectHash, &ObjectComparator> map_;
};

void Test() {
  size_t N_min = 10000;
  size_t N_max = 1000000;

  size_t step = static_cast<size_t>(
      std::round(static_cast<double>(N_max - N_min) / 10.0));

  for (size_t N = N_min; N <= N_max; N += step) {
    std::cout << "N = " << N << std::endl;
    Object** data = new Object*[N];
    for (size_t i = 0; i < N; ++i) {
      data[i] =
          new Object(generateRandomString(7, 20), generateRandomString(7, 20));
    }
    Object** dataRandom = new Object*[2 * N];
    for (size_t i = 0; i < 2 * N; ++i) {
      dataRandom[i] =
          new Object(generateRandomString(7, 20), generateRandomString(7, 20));
    }
    {
      SortedArray sortedArray(data, N);
      sortedArray.searchSameSort();
      sortedArray.searchRandomSort(dataRandom, 2 * N);
    }
    //    {
    //      SortedArray* sortedArray = new SortedArray(data, N);
    //      TimerProfiler t(fileLog, "SortedArrayClear", N);
    //      sortedArray->~SortedArray();
    //    }

    {
      AvlTree avlTree(data, N);
      avlTree.searchSameAVL();
      avlTree.searchRandomAVL(dataRandom, 2 * N);
      avlTree.removeSameAVL();
    }
    //    {
    //      AvlTree* avlTree = new AvlTree(data, N);
    //      TimerProfiler t(fileLog, "AvlTreeClear", N);
    //      avlTree->~AvlTree();
    //    }

    {
      HashMap hashMap(data, N);
      hashMap.searchSameHash();
      hashMap.searchRandomHash(dataRandom, 2 * N);
      hashMap.removeSameHash();
    }
    //    {
    //      HashMap* hashMap = new HashMap(data, N);
    //      TimerProfiler t(fileLog, "HashMapClear", N);
    //      hashMap->~HashMap();
    //    }
  }
}

int main() {
  fileLog << "Function,Duration,Amount\n";
  Test();
}