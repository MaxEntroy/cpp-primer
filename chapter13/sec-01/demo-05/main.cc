#include <iostream>

#include "foo.h"

using namespace cp;

int main(void) {
  Foo a;
  std::cout << a.uid() << std::endl;

  Foo b;
  std::cout << b.uid() << std::endl;

  Foo c = a;
  std::cout << c.uid() << std::endl;

  c = b;
  std::cout << c.uid() << std::endl;

  return 0;
}
