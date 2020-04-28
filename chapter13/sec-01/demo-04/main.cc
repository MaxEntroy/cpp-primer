#include <vector>
#include "foo.h"
using namespace cp;

void f1(Foo foo) {}

void f2(Foo& foo) {}

int main(void) {

  std::cout << "stack object: " << std::endl;
  Foo foo;
  std::cout << std::endl;

  std::cout << "pass arg with value: " << std::endl;
  f1(foo);
  std::cout << std::endl;

  std::cout << "pass arg with ref: " << std::endl;
  f2(foo);
  std::cout << std::endl;

  std::cout << "dynamic object: " << std::endl;
  Foo* pf = new Foo();
  std::cout << std::endl;

  std::cout << "container insert: " << std::endl;
  std::vector<Foo> foo_vec;
  foo_vec.push_back(*pf);
  std::cout << std::endl;

  std::cout << "release dynamic object: " << std::endl;
  delete pf;
  std::cout << std::endl;

  std::cout << "copy initialization: " << std::endl;
  Foo foo1 = foo;
  std::cout << std::endl;

  std::cout << "copy assignment: " << std::endl;
  foo1 = foo;
  std::cout << std::endl;

  return 0;
}
