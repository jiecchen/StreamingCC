#include "timer.h"

#include <chrono>
#include <ctime>

Timer::Timer() {
  start_t_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now().time_since_epoch())
                 .count();
}

void Timer::reset() {
  start_t_ = std::chrono::duration_cast<std::chrono::milliseconds>(
                 std::chrono::system_clock::now().time_since_epoch())
                 .count();
}

double Timer::get_time() {
  unsigned long end_t = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch())
                            .count();
  auto gap = end_t - start_t_;
  start_t_ = end_t;
  return gap;
}
