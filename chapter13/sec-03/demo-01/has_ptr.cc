#include "has_ptr.h"

#include <iostream>
#include <utility>

namespace cp {

HasPtr::HasPtr(const HasPtr& rhs) {
  val_ = rhs.val_;

  pstr_ = (rhs.pstr_)?new std::string(*rhs.pstr_):nullptr;
}

HasPtr& HasPtr::operator=(HasPtr rhs) {
  swap(*this, rhs);
  return *this;
}

void HasPtr::PrintStr() const {
  std::cout << *pstr_ << std::endl;
}

} // namespace cp
