#include <iostream>
#include "cstr.h"


int main(void) {
  const char* s1 = "hello,";
  const char* s2 = "world!";

  char* s = cp::cstr_cat(s1, s2);
  std::cout << s << std::endl;

  std::string ss1 = "hello,";
  std::string ss2 = "world!";
  std::string ss = cp::cppstr_cat(ss1, ss2);
  std::cout << ss << std::endl;

  delete [] s;
  return 0;
}
