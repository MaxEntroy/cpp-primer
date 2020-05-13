#include "str_vec_impl.h"

#include <algorithm>

namespace cp {

void StrVecImpl::create(size_t sz) {
  alloc_mem(sz);

  for(Iter it = first_; it != last_; it++) {
    alloc_.construct(it, std::string());
  }
}

void StrVecImpl::create(ConstIter b, ConstIter e) {
  alloc_mem(e - b);

  std::uninitialized_copy(b, e, first());
}

void StrVecImpl::uncreate() {
  while(last_ != first_) {
    alloc_.destroy(--last_);
  }

  alloc_.deallocate(first_, end_ - first_);
}

void StrVecImpl::realloc_mem(size_t n) {
  Iter new_first = alloc_.allocate(n);
  Iter new_last = new_first;

  for(Iter it = first_; it != last_; it++) {
    alloc_.construct(new_last++, std::move(*it));
  }

  uncreate();

  first_ = new_first;
  last_ = new_last;
  end_ = first_ + n;
}

void StrVecImpl::alloc_mem(size_t sz) {
  size_t cap = sz?2*sz:1;

  first_ = alloc_.allocate(cap);
  last_ = first_ + sz;
  end_ = first_ + cap;
}

} // namespace cp
