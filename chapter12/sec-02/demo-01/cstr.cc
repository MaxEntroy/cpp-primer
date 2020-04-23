#include "cstr.h"

#include <string.h>

namespace cp {

char* cstr_cat(const char* left, const char* right) {
  size_t sz = strlen(left) + strlen(right) + 1;

  char* s = new char[sz]();

  strcat(s, left);
  strcat(s, right);
  s[sz] = '\0';

  return s;
}

std::string cppstr_cat(const std::string& l, const std::string& r) {
  return l + r;
}

} // namespace cp
