#ifndef MIPT2024S_3_MOVETEST_H
#define MIPT2024S_3_MOVETEST_H

#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

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

struct AverageDispersion {
  double average;
  double dispersion;
};

AverageDispersion FindAverageDispersion(const std::vector<long long>& vec1) {
  double sum1 = 0;
  double sum2 = 0;
  for (long long val : vec1) {
    sum1 += double(val);
    sum2 += double(val) * double(val);
  }
  double mean = sum1 / vec1.size();
  double mean_sqr = sum2 / vec1.size();

  double dispersion = std::sqrt(mean_sqr - mean * mean);

  return {mean, dispersion};
}

template <typename T>
std::pair<AverageDispersion, AverageDispersion> TestMoveSemantics(
    const T& obj) {
  if (!std::is_move_constructible<T>::value ||
      !std::is_copy_constructible<T>::value) {
    throw std::runtime_error(
        "T must be move-constructible and copy-constructible");
  }

  std::vector<long long> vec_move;
  std::vector<long long> vec_copy;

  for (int i = 0; i < 10000; ++i) {
    auto start = std::chrono::steady_clock::now();
    T obj_copy(obj);
    auto stop = std::chrono::steady_clock::now();
    auto duration_copy =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count();
    vec_copy.push_back(duration_copy);

    start = std::chrono::steady_clock::now();
    T obj_move(std::move(obj_copy));
    stop = std::chrono::steady_clock::now();
    auto duration_move =
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start)
            .count();
    vec_move.push_back(duration_move);
  }
  return {FindAverageDispersion(vec_move), FindAverageDispersion(vec_copy)};
}
#endif  // MIPT2024S_3_MOVETEST_H
