#include <iostream>
#include "employee.h"

int main(void) {
  cp::Employee a;
  cp::Employee b;

  std::cout << a.uid() << std::endl;
  std::cout << b.uid() << std::endl;

  a = b;
  std::cout << a.uid() << std::endl;
  std::cout << b.uid() << std::endl;

  return 0;
}
