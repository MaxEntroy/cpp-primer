#ifndef DERIVED_H_
#define DERIVED_H_

#include <iostream>

#include "base.h"

class Derived : public Base {
 public:
  Derived() : val_(0) {}

  Derived(const Derived&) = delete;
  Derived& operator=(const Derived&) = delete;

  void Print() override {
    std::cout << "Derived::Print() called." << std::endl;
  }

 private:
  int val_;
};

#endif
