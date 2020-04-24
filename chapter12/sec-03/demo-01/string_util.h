#ifndef STRING_UTIL_H_
#define STRING_UTIL_H_

#include <set>
#include <string>
#include <vector>

namespace cp {

std::set<std::string> unique_split(const std::string& line);

std::vector<std::string> split(const std::string& line);

} // namespace cp

#endif // STRING_UTIL_H_
