#include "has_ptr.h"

namespace cp {

std::mutex StrData::mtx;

void StrData::IncrRef() {
  std::lock_guard<std::mutex> lock(mtx);
  ++ref;
}

void StrData::DecrRef() {
  std::lock_guard<std::mutex> lock(mtx);
  --ref;
}

HasPtr::~HasPtr() {
  if (pstr_) {
    pstr_->DecrRef();
    if (pstr_->GetRef() == 0) {
      delete pstr_;
    }
  }
}

HasPtr::HasPtr(const HasPtr& rhs) {
  val_ = rhs.val_;
  pstr_ = rhs.pstr_;

  if (pstr_) {
    pstr_->IncrRef();
  }
}

HasPtr& HasPtr::operator=(const HasPtr& rhs) {
  if (this == &rhs) {
    return *this;
  }

  auto ptmp = rhs.pstr_;
  if (ptmp) {
    ptmp->IncrRef();
  }

  pstr_->DecrRef();
  if (pstr_->GetRef() == 0) {
    delete pstr_;
  }

  pstr_ = ptmp;
  return *this;
}

} // namespace cp
