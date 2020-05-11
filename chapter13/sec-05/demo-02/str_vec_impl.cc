#include "str_vec_impl.h"

namespace cp {

void StrVecImpl::create(int sz) {
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

void StrVecImpl::alloc_mem(int sz) {
  int cap = sz?2*sz:1;

  first_ = alloc_.allocate(cap);
  last_ = first_ + sz;
  end_ = first_ + cap;
}

void StrVecImpl::realloc_mem() {
  int cap = end_ - first_;
  int new_cap = cap * 2;
  Iter new_first = alloc_.allocate(new_cap);

  std::uninitialized_copy(first_, last_, new_first);

  uncreate();

  first_ = new_first;
  last_ = first_ + cap;
  end_ = first_ + new_cap;
}

} // namespace cp
