#include "weak_str_blob.h"

#include <stdexcept>

namespace cp {

void WeakStrBlob::incr() {
  check(cur_ + 1, "invalid incr");
  ++cur_;
}

void WeakStrBlob::decr() {
  check(cur_- 1, "invalid decr");
  --cur_;
}

std::string& WeakStrBlob::deref() {
  auto ptr = check(cur_, "invalid deref");
  return (*ptr)[cur_];
}

std::shared_ptr<WeakStrBlob::VecStr> WeakStrBlob::check(size_type i, const std::string& msg) const {
  auto ptr = data_.lock();
  if (!ptr)
    throw std::runtime_error("invalid StrBlob");

  // i is unsigned type, if decr() make i < 0, then i is the biggest number.
  if (i >= ptr->size())
    throw std::out_of_range(msg);

  return ptr;
}

} // namespace cp
