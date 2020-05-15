#include <iostream>
#include "str_vec.h"

namespace cp {

StrVec& StrVec::operator=(StrVec& rhs) {
  if (this == &rhs) {
    return *this;
  }

  uncreate();
  create(rhs.begin(), rhs.end());
  std::cout << "StrVec& operator= called.\n";
  return *this;
}

StrVec::StrVec(StrVec&& rhs) noexcept :
  first_(rhs.first_), last_(rhs.last_), end_(rhs.end_) {
  rhs.first_ = rhs.last_ = rhs.end_ = nullptr;

  std::cout << "StrVec(StrVec&&) called.\n";
}

// StrVec& StrVec::operator=(StrVec&& rhs) noexcept {
//   if (this == &rhs) {
//     return *this;
//   }
//
//   uncreate();
//
//   first_ = rhs.first_;
//   last_ = rhs.last_;
//   end_ = rhs.end_;
//
//   rhs.first_ = rhs.last_ = rhs.end_ = nullptr;
//   std::cout << "StrVec& operator=(StrVec&&) called.\n";
//   return *this;
// }

void StrVec::create(int sz) {
  alloc_mem(sz);

  for(Iter it = first_; it != last_; it++) {
    alloc_.construct(it, std::string());
  }
}

void StrVec::create(ConstIter b, ConstIter e) {
  alloc_mem(e - b);

  std::uninitialized_copy(b, e, begin());
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
  Iter new_last = new_first;

  for(Iter it = first_; it != last_; ++it) {
    alloc_.construct(new_last++, std::move(*it));
  }
  std::uninitialized_copy(std::);

  uncreate();

  first_ = new_first;
  last_ = new_last;
  end_ = first_ + new_cap;
}

void StrVec::push_back(const std::string& val) {
  if (!is_enough_room()) {
    realloc_mem();
  }

  alloc_.construct(last_++, val);
}

} // namespace cp
