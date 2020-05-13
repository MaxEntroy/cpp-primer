#include "str_vec.h"
#include "str_vec_impl.h"

namespace cp {

StrVec::StrVec(size_t sz) : pimpl_(new StrVecImpl()) {
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

void StrVec::reserve(size_t n) {
  if (capacity() < n) {
    pimpl_->realloc_mem(n);
  }
}

void StrVec::resize(size_t n, std::string val) {
  size_t sz = size();
  size_t cap = capacity();

  if (n <= sz) {
    size_t dif = sz - n;
    for(size_t i = 0; i < dif; ++i) {
      pimpl_->destroy_last();
    }
    return;
  }

  if ( sz < n && n <= cap ) {
    size_t dif = n - sz;
    for(size_t i = 0; i < dif; ++i) {
      pimpl_->construct_last(val);
    }
    return;
  }

  pimpl_->realloc_mem(n);
  size_t dif = n - sz;
  for(size_t i = 0; i < dif; ++i) {
    pimpl_->construct_last(val);
  }
}

void StrVec::push_back(const std::string& val) {
  if (!pimpl_->is_enough_room()) {
    pimpl_->realloc_mem(2 * capacity());
  }

  pimpl_->construct_last(val);
}

} // namespace cp
