#ifndef A_H_
#define A_H_

#include <iostream>
#include <memory>

#include "b.h"

namespace cp {

class A {
 public:
  A() {std::cout << "A() called.\n";}
  ~A() {std::cout << "~A() called.\n";}

  void SetBptr(std::shared_ptr<B> ptr) { b_ptr = ptr;}

 private:
  std::shared_ptr<B> b_ptr;
};

} // namespace cp

#endif // A_H_
