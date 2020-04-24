#include "string_util.h"

#include <algorithm>
#include <string>

namespace cp {

static bool is_split_ch(char ch) {
  static const std::string split_ch = ",. ";
  return (find(split_ch.begin(), split_ch.end(), ch) != split_ch.end());
}

static bool not_split_ch(char ch) {
  return !is_split_ch(ch);
}

std::set<std::string> unique_split(const std::string& line) {
  std::set<std::string> ret;

  auto b = line.begin();
  auto e = line.end();

  while(b != e) {
    b = find_if(b, e, not_split_ch);
    auto after = find_if(b, e, is_split_ch);
    if(b != after) {
      ret.insert(std::string(b, after));
      b = after;
    }
  }

  return ret;
}

} // namespace cp
