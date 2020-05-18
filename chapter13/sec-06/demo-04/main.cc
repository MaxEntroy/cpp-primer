#include "has_ptr.h"

using namespace cp;

int main(void) {
  HasPtr aa("aa");
  HasPtr bb("bb");

  aa.PrintStr();
  bb.PrintStr();

  swap(aa, bb);

  aa.PrintStr();
  bb.PrintStr();

  return 0;
}
