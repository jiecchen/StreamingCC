#ifndef __TIMER_H__
#define __TIMER_H__

class Timer {
 public:
  Timer();
  double get_time();
  void reset();
 private:
  unsigned long start_t_;
};

#endif














