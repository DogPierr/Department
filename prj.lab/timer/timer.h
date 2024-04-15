#ifndef MIPT2024S_3_TIMER_H
#define MIPT2024S_3_TIMER_H

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>
#include <atomic>
#include <deque>

struct FileTimer {
  FileTimer() : file_name_(""), time_(0), n_(0) {}
  FileTimer(const char* fileName) : file_name_(fileName), time_(0), n_(0) {}
  const char* file_name_;
  unsigned long long time_;
  unsigned long long n_;
};

class Ancor {
 public:
  Ancor(bool flag);
  Ancor(int counter, bool flag);
  Ancor(int count, const char* name, bool flag);

  FileTimer* GetPtr();

  void WriteToFile(const std::string& file_name);

  static FileTimer* data;
 private:
  int count;
  const char* name;
  bool is_measuring_;
  FileTimer* ptr;
};

class Timer {
 public:
  Timer(FileTimer* fileTimer, bool flag);

  ~Timer();

 private:
  FileTimer* file_timer_;
  std::chrono::time_point<std::chrono::high_resolution_clock> timer_;
  bool is_measuring_;
};

#endif  // MIPT2024S_3_TIMER_H
