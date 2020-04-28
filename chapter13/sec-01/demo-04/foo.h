#ifndef FOO_H_
#define FOO_H_

#include <iostream>

namespace cp {

struct Foo {
  Foo() {std::cout << "Foo() called.\n";}
  ~Foo() {std::cout << "~Foo() called.\n";}
  Foo(const Foo& rhs) {std::cout << "Foo(const Foo&) called.\n";}
  Foo& operator=(const Foo& rhs) {std::cout << "operator=(const Foo&) called.\n"; return *this;}
};

} // namespace cp

#endif // FOO_H_
