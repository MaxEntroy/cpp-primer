#ifndef BASE_H_
#define BASE_H_

#include <iostream>

namespace test {

class Base {
 public:
  Base() {std::cout << "Base Constructor called.\n";}
  virtual ~Base() {std::cout << "Base Destructor called.\n";}

  virtual void print() {std::cout << "Base::print() called.\n";}
};

} // namespace test

#endif // BASE_H_
