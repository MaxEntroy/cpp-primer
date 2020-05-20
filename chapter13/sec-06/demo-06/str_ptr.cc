#include "str_ptr.h"

namespace cp {
StrPtr::~StrPtr() {
  if (ptr_) {
    delete ptr_;
  }
}

StrPtr::StrPtr(const StrPtr& rhs) {
  std::cout << "copy ctor called.\n";

  ptr_ = new std::string(*rhs.ptr_);
}

StrPtr& StrPtr::operator=(const StrPtr& rhs) {
  std::cout << "copy assignment called.\n";

  if (this == &rhs) {
    return *this;
  }

  auto p = new std::string(*rhs.ptr_);
  if (!p) {
    return *this;
  }

  if (ptr_) {
    delete ptr_;
  }

  ptr_ = p;

  return *this;
}

StrPtr::StrPtr(StrPtr&& rhs) {
  std::cout << "move ctor called.\n";

  ptr_ = rhs.ptr_;
  rhs.ptr_ = nullptr;
}

StrPtr& StrPtr::operator=(StrPtr&& rhs) {
  std::cout << "move assignment called.\n";

  if (this == &rhs) {
    return *this;
  }

  if (ptr_) {
    delete ptr_;
  }

  ptr_ = rhs.ptr_;
  rhs.ptr_ = nullptr;

  return *this;
}

const std::string& StrPtr::operator*() const {
  return *ptr_;
}

bool StrPtr::operator<(const StrPtr& rhs) const {
  if (ptr_ && rhs.ptr_) {
    return *ptr_ < *rhs.ptr_;
  }
  else {
    return true;
  }
}

} // namespace cp
