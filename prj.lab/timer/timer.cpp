#include "timer.h"

#include <chrono>
#include <fstream>
#include <iostream>

Timer::Timer(FileTimer* fileTimer, bool flag)
    : file_timer_(fileTimer),
      is_measuring_(flag),
      timer_(std::chrono::high_resolution_clock::now()) {}

Timer::~Timer() {
  if (is_measuring_) {
    auto stop = std::chrono::high_resolution_clock::now();
    file_timer_->time_ +=
        std::chrono::duration_cast<std::chrono::nanoseconds>(stop - timer_).count();
    file_timer_->n_ += 1;
  }
}

Ancor::Ancor(bool flag) : is_measuring_(flag) {}

Ancor::Ancor(int count, const char* name, bool flag)
    : count(count), name(name), is_measuring_(flag) {
  if (is_measuring_) {
    data[count] = FileTimer(name);
    ptr = &data[count];
  }
}

FileTimer* Ancor::GetPtr() { return ptr; }

void Ancor::WriteToFile(const std::string& file_name) {
  if (is_measuring_) {
    std::fstream file(file_name, std::fstream::out);
    for (int i = 0; i < count; ++i) {
      std::cout << data[i].file_name_ << ',' << data[i].time_ << ','
                << data[i].n_ << '\n';
    }
  }
}

Ancor::Ancor(int counter, bool flag) : count(counter), is_measuring_(flag) {}
