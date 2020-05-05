#include "strblob.h"

namespace cp {

StrBlob::StrBlob(const StrBlob& rhs) {
  data_ = std::make_shared<VecStr>(*rhs.data_);
}

StrBlob& StrBlob::operator=(const StrBlob& rhs) {
  if (this == &rhs) {
    return *this;
  }

  auto sp = std::make_shared<VecStr>(*rhs.data_);
  if (!sp) {
    return *this;
  }

  data_ = sp;
  return *this;
}

} // namespace cp
