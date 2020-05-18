#include "has_ptr.h"

#include <iostream>
#include <utility>

namespace cp {

HasPtr::HasPtr(const HasPtr& rhs) {
  val_ = rhs.val_;

  pstr_ = (rhs.pstr_)?new std::string(*rhs.pstr_):nullptr;
}

HasPtr& HasPtr::operator=(const HasPtr& rhs) {
  if (this == &rhs) {
    return*this;
  }

  val_ = rhs.val_;
  auto new_ptr = (rhs.pstr_)?new std::string(*rhs.pstr_):nullptr;

  if (pstr_) {
    delete pstr_;
  }

  pstr_ = new_ptr;
  return *this;
}

HasPtr::HasPtr(HasPtr&& rhs) noexcept {
  val_ = rhs.val_;
  pstr_ = rhs.pstr_;

  rhs.pstr_ = nullptr;
}

HasPtr& HasPtr::operator=(HasPtr&& rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }

  if (pstr_) {
    delete pstr_;
  }

  val_ = rhs.val_;
  pstr_ = rhs.pstr_;

  rhs.pstr_ = nullptr;

  return *this;
}

void HasPtr::PrintStr() const {
  std::cout << *pstr_ << std::endl;
}

} // namespace cp
