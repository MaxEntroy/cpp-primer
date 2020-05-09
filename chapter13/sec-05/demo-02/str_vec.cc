#include "str_vec.h"
#include "str_vec_impl.h"

namespace cp {

inline StrVec::StrVec(int sz) {
  pimpl_->create(sz);
}

inline StrVec::StrVec(std::initializer_list<std::string> il) {
  pimpl_->create(il.begin(), il.end());
}

inline StrVec::~StrVec() {
  pimpl_->uncreate();
}

inline StrVec::StrVec(const StrVec& rhs) {
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

inline StrVec::Iter StrVec::begin() const {
  return pimpl_->first();
}

inline StrVec::Iter StrVec::end() const {
  return pimpl_->last();
}

inline size_t StrVec::size() const {
  return end() - begin();
}

inline size_t StrVec::capacity() const {
  return pimpl_->end() - begin();
}

void StrVec::push_back(const std::string& val) {
  if (!pimpl_->is_enough_room()) {
    pimpl_->realloc_mem();
  }

  pimpl_->construct(end(), val);
  pimpl_->incr_last();
}

} // namespace cp
