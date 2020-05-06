#include <algorithm>
#include <vector>

#include "has_ptr.h"

using namespace cp;

int main(void) {
  HasPtr aa("aa");
  HasPtr bb("bb");
  HasPtr cc("cc");

  std::vector<HasPtr> vec;
  vec.push_back(cc);
  vec.push_back(bb);
  vec.push_back(aa);

  std::sort(vec.begin(), vec.end());
  for (const auto& v : vec) {
    v.PrintStr();
  }

  return 0;
}
