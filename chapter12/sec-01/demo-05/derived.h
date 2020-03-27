#ifndef DERIVED_H_
#define DERIVED_H_

#include "base.h"

namespace test {

class Derived : public Base {
 public:
  Derived() : Base() {std::cout << "Derived Constructor called.\n";}
  ~Derived () {std::cout << "Derived Destructor called.\n";}

  void print() override {
    std::cout << "Derived::print() called.\n";
  }
};

} // namespace test

#endif // DERIVED_H_
