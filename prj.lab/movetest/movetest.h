#ifndef MIPT2024S_3_MOVETEST_H
#define MIPT2024S_3_MOVETEST_H

#include "algorithm"
#include "chrono"

template <typename T, bool IsTest>
class MoveTester {
 public:
  MoveTester() = default;
  bool Test(const T& obj);

 private:
};

template <typename T, bool IsTest>
bool MoveTester<T, IsTest>::Test(const T& obj) {
  if (IsTest) {
    if (!std::is_move_constructible<T>::value ||
        !std::is_copy_constructible<T>::value) {
      return false;
    }

    auto start = std::chrono::high_resolution_clock::now();
    T obj_copy(obj);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration_copy =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count();

    start = std::chrono::high_resolution_clock::now();
    T obj_move(std::move(obj_copy));
    stop = std::chrono::high_resolution_clock::now();
    auto duration_move =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count();

    if (duration_copy < duration_move) {
      return false;
    }
    return true;
  }
  return false;
}

#endif  // MIPT2024S_3_MOVETEST_H
