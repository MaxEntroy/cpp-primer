#ifndef CSTR_H_
#define CSTR_H_

#include <string>

namespace cp {

// 1.不检查参数有效性
// 2.返回dynamic char array
char* cstr_cat(const char* left, const char* right);

std::string cppstr_cat(const std::string& l, const std::string& r);

} // namespace cp

#endif // CSTR_H_
