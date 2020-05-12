#include "str_vec.h"
#include "str_vec_impl.h"

namespace cp {

StrVec::StrVec(int sz) : pimpl_(new StrVecImpl()) {
  pimpl_->create(sz);
}

StrVec::StrVec(std::initializer_list<std::string> il) : pimpl_(new StrVecImpl()) {
  pimpl_->create(il.begin(), il.end());
}

StrVec::~StrVec() {
  pimpl_->uncreate();
}

StrVec::StrVec(const StrVec& rhs) : pimpl_(new StrVecImpl()) {
  pimpl_->create(rhs.begin(), rhs.end());
}

StrVec& StrVec::operator=(const StrVec& rhs) {
  if (this == &rhs) {
    return *this;
  }

  pimpl_->uncreate();
  pimpl_->create(rhs.begin(), rhs.end());
  return *this;
}

StrVec::Iter StrVec::begin() const {
  return pimpl_->first();
}

StrVec::Iter StrVec::end() const {
  return pimpl_->last();
}

size_t StrVec::size() const {
  return end() - begin();
}

size_t StrVec::capacity() const {
  return pimpl_->end() - begin();
}

void StrVec::push_back(const std::string& val) {
  if (!pimpl_->is_enough_room()) {
    pimpl_->realloc_mem();
  }

  pimpl_->construct_last(val);
}

} // namespace cp
