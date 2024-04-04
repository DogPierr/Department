    #include "sort.h"

namespace templates {
void merge(void** ppArray, int start, int mid, int end,
           CompareSortType pCompareFunc) {
  int start2 = mid + 1;

  if (pCompareFunc(ppArray[mid], ppArray[start2]) >= 0) {
    return;
  }

  while (start <= mid && start2 <= end) {
    if (pCompareFunc(ppArray[start], ppArray[start2]) >=
        0) {
      start++;
    } else {
      auto value = ppArray[start2];
      int index = start2;

      while (index != start) {
        ppArray[index] = ppArray[index - 1];
        index--;
      }
      ppArray[start] = value;

      start++;
      mid++;
      start2++;
    }
  }
}

void mergeSortHelper(void** ppArray, int start, int end,
                     CompareSortType pCompareFunc) {
  if (start < end) {
    int m = start + (end - start) / 2;

    mergeSortHelper(ppArray, start, m, pCompareFunc);
    mergeSortHelper(ppArray, m + 1, end, pCompareFunc);

    merge(ppArray, start, m, end, pCompareFunc);
  }
}

void mergeSort(void** ppArray, int length,
                          CompareSortType pCompareFunc) {
  mergeSortHelper(ppArray, 0, length - 1, pCompareFunc);
}
}
