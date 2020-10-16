#ifndef BASE_H_
#define BASE_H_

#include <iostream>

class Base {
 public:
  Base() = default;

  Base(const Base&) = delete;
  Base& operator=(const Base&) = delete;

  virtual ~Base() = default;

  virtual void Print() = 0;

};

#endif
