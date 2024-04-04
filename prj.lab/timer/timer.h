#ifndef MIPT2024S_3_TIMER_H
#define MIPT2024S_3_TIMER_H

#include <string>
#include <chrono>
#include <unordered_map>
#include <vector>

class Timer {
 public:
  Timer(const std::string& function_name);

  void WriteToFile(const std::string& file_name);

  ~Timer();

 private:
  struct FileTimer {
    std::string file_name_;
    long time_;
    long n_;
  };

  std::string function_name_;
  inline static std::unordered_map<std::string, FileTimer> timers_;
  std::chrono::time_point<std::chrono::high_resolution_clock> timer_;
};

#endif  // MIPT2024S_3_TIMER_H
