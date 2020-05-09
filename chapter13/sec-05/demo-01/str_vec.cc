#include <iostream>
#include "str_vec.h"

namespace cp {
StrVec& StrVec::operator=(const StrVec& rhs) {
  if (this == &rhs) {
    return *this;
  }

  uncreate();
  create(rhs.begin(), rhs.end());
  return *this;
}

void StrVec::create(int sz) {
  alloc_mem(sz);

  for(Iter it = first_; it != last_; it++) {
    alloc_.construct(it, std::string());
  }
}

void StrVec::create(ConstIter b, ConstIter e) {
  alloc_mem(e - b);

  std::uninitialized_copy(b, e, first_);
}

void StrVec::uncreate() {
  while(last_ != first_) {
    alloc_.destroy(--last_);
  }

  alloc_.deallocate(first_, capacity());
}

void StrVec::alloc_mem(int sz) {
  int cap = sz?2*sz:1;

  first_ = alloc_.allocate(cap);
  last_ = first_ + sz;
  end_ = first_ + cap;
}

void StrVec::realloc_mem() {
  int cap = capacity();
  int new_cap = cap * 2;
  Iter new_first = alloc_.allocate(new_cap);

  std::uninitialized_copy(first_, last_, new_first);

  uncreate();

  first_ = new_first;
  last_ = first_ + cap;
  end_ = first_ + new_cap;
}

void StrVec::push_back(const std::string& val) {
  if (!is_enough_room()) {
    realloc_mem();
  }

  alloc_.construct(last_++, val);
}

} // namespace cp
