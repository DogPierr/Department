#ifndef MIPT2024S_3_MOVETEST_H
#define MIPT2024S_3_MOVETEST_H

#include "algorithm"
#include "chrono"

template <bool IsTest>
class MoveTester {
 public:
  MoveTester() = default;
  MoveTester(MoveTester&& other);
  MoveTester(const MoveTester& other);
  ~MoveTester() = default;

 private:
  void CreateFakeValue();
  const int kMax = 10'000;
};
template <bool IsTest>
MoveTester<IsTest>::MoveTester(const MoveTester& other) {
  if (IsTest) {
    CreateFakeValue();
  }
}

template <bool IsTest>
MoveTester<IsTest>::MoveTester(MoveTester&& other) {
  if (!IsTest) {
    CreateFakeValue();
  }
}

template <bool IsTest>
void MoveTester<IsTest>::CreateFakeValue() {
  std::vector<int> vec;
  for (int i = 0; i < kMax; ++i) {
    vec.push_back(1);
  }
}

template <typename T>
bool TestMoveSemantics(const T& obj) {
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
#endif  // MIPT2024S_3_MOVETEST_H
