#include "has_ptr.h"

namespace cp {

HasPtr::HasPtr(const HasPtr& rhs) {
  val_ = rhs.val_;

  pstr_ = (rhs.pstr_)?new std::string(*rhs.pstr_):nullptr;
}

HasPtr& HasPtr::operator=(const HasPtr& rhs) {
  if (this == &rhs) {
    return *this;
  }

  if (!rhs.pstr_) {
    pstr_ = nullptr;
    val_ = rhs.val_;
    return *this;
  }

  auto ptmp = new std::string(*rhs.pstr_);
  if (!ptmp) {
    return *this;
  }

  pstr_ = ptmp;
  val_ = rhs.val_;
  return *this;
}

} // namespace cp
