#include "sort.h"

#include "algorithm"

namespace templates {
void merge(void** ppArray, void** tmp, int l, int mid, int r,
           CompareSortType pCompareFunc) {
  for (int k = l; k <= r; k++) tmp[k] = ppArray[k];

  int i = l;
  int j = mid + 1;

  for (int k = l; k <= r; k++) {
    if (i > mid)
      ppArray[k] = tmp[j++];
    else if (j > r)
      ppArray[k] = tmp[i++];
    else if (pCompareFunc(tmp[j], tmp[i]) > 0)  // less
      ppArray[k] = tmp[j++];
    else
      ppArray[k] = tmp[i++];
  }
}

void mergeSortHelper(void** ppArray, int start, int end,
                     CompareSortType pCompareFunc) {
  void** tmp = new void*[end - start + 1];
  int length = end - start + 1;

  for (int sz = 1; sz < length; sz *= 2) {
    for (int left = 0; left < length - sz; left += 2 * sz) {
      merge(ppArray, tmp, left, left + sz - 1,
            std::min(left + 2 * sz - 1, length - 1), pCompareFunc);
    }
  }

  delete[] tmp;
}

void mergeSort(void** ppArray, int length, CompareSortType pCompareFunc) {
  mergeSortHelper(ppArray, 0, length - 1, pCompareFunc);
}

void heapify(void** arr, int n, int i, CompareSortType pCompareFunc) {
  int largest = i;
  int l = 2 * i + 1;
  int r = 2 * i + 2;
  if (l < n && pCompareFunc(arr[largest], arr[l]) > 0)  // arr[l] > arr[largest]
    largest = l;
  if (r < n && pCompareFunc(arr[largest], arr[r]) > 0)  // arr[r] > arr[largest]
    largest = r;
  if (largest != i) {
    std::swap(arr[i], arr[largest]);
    heapify(arr, n, largest, pCompareFunc);
  }
}

void heapSort(void** ppArray, int length, CompareSortType pCompareFunc) {
  for (int i = length / 2 - 1; i >= 0; i--)
    heapify(ppArray, length, i, pCompareFunc);

  for (int i = length - 1; i > 0; i--) {
    std::swap(ppArray[0], ppArray[i]);
    heapify(ppArray, i, 0, pCompareFunc);
  }
}

int partition(void** arr, int low, int high, CompareSortType pCompareFunc) {
  auto pivot = arr[high];
  int i = (low - 1);

  for (int j = low; j <= high - 1; j++) {
    if (pCompareFunc(arr[j], pivot) >= 0) { // arr[j] <= pivot
      i++;
      std::swap(arr[i], arr[j]);
    }
  }
  std::swap(arr[i + 1], arr[high]);
  return (i + 1);
}

int randomPartition(void** arr, int low, int high, CompareSortType pCompareFunc) {
  int random = low + rand() % (high - low);
  std::swap(arr[random], arr[high]);
  return partition(arr, low, high, pCompareFunc);
}

void quickSortHelper(void** arr, int low, int high, CompareSortType pCompareFunc) {
  if (low < high) {
    int pi = randomPartition(arr, low, high, pCompareFunc);
    quickSortHelper(arr, low, pi - 1, pCompareFunc);
    quickSortHelper(arr, pi + 1, high, pCompareFunc);
  }
}

void quickSort(void** ppArray, int length,
                          CompareSortType pCompareFunc) {
  quickSortHelper(ppArray, 0, length - 1, pCompareFunc);
}


}  // namespace templates
