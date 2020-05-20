#include "mstring.h"

namespace cp {

MString& MString::operator=(const MString& rhs) {
  if (this == &rhs) {
    return *this;
  }

  auto p = new char[rhs.sz_];
  if (!p) {
    return *this;
  }

  delete pstr_;

  sz_ = rhs.sz_;
  pstr_ = p;
  strcpy(pstr_, rhs.pstr_);

  return *this;
}

MString& MString::operator=(MString&& rhs) noexcept {
  if (this == &rhs) {
    return *this;
  }

  delete[] pstr_;

  pstr_ = rhs.pstr_;
  sz_ = rhs.sz_;

  rhs.pstr_ = nullptr;
  rhs.sz_ = 0;

  return *this;
}

} // namespace cp
