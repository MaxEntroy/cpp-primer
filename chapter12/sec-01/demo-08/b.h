#ifndef B_H_
#define B_H_

#include <iostream>
#include <memory>

namespace cp {

class A;

class B {
 public:
  B() {std::cout << "B() called.\n";}
  ~B() {std::cout << "~B() called.\n";}

  void SetAptr(std::shared_ptr<A> ptr) {a_ptr = ptr;}

 private:
  std::weak_ptr<A> a_ptr;
};

} // namespace cp

#endif // B_H_
