#include "a.h"

using namespace cp;

int main (void) {
  std::shared_ptr<A> a_ptr = std::make_shared<A>();
  std::shared_ptr<B> b_ptr = std::make_shared<B>();

  a_ptr->SetBptr(b_ptr);
  b_ptr->SetAptr(a_ptr);

  return 0;
}
