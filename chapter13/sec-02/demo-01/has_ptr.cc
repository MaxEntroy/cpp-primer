#include "has_ptr.h"

namespace cp {

HasPtr::HasPtr(const HasPtr& rhs) {
  val_ = rhs.val_;

  pstr_ = (rhs.pstr_)?new std::string(*rhs.pstr_):nullptr;
}

HasPtr& HasPtr::operator=(const HasPtr& rhs) {
  val_ = rhs.val_;

  if (pstr_) {
    delete pstr_;
  }

  pstr_ = (rhs.pstr_)?new std::string(*rhs.pstr_):nullptr;

  return *this;
}

} // namespace cp
