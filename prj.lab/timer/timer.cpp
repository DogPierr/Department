#include "timer.h"

#include <chrono>
#include <fstream>

Timer::Timer(const char* function_name)
    : function_name_(function_name),
      timer_(std::chrono::high_resolution_clock::now()) {}

void Timer::WriteToFile(const std::string& file_name) {
  std::fstream file(file_name, std::fstream::out);
  for (auto& [key, value] : timers_) {
    file << key << ',' << value.time_ << ',' << value.n_ << '\n';
  }
}

Timer::~Timer() {
  auto stop = std::chrono::high_resolution_clock::now();

  if (timers_.find(function_name_) == timers_.end()) {
    timers_[function_name_] = FileTimer();
    timers_[function_name_].file_name_ = function_name_;
  }

  timers_[function_name_].time_ +=
      std::chrono::duration_cast<std::chrono::microseconds>(stop - timer_)
          .count();
  timers_[function_name_].n_ += 1;
}
