#include "str_blob.h"

#include <stdexcept>

namespace cp {

void StrBlob::push_back(const std::string& item) {
  data_->push_back(item);
}
void StrBlob::pop_back() {
  check(0, "pop_back on empty strblob");
  data_->pop_back();
}

std::string& StrBlob::front() {
  check(0, "front on empty strblob");
  return data_->front();
}

const std::string& StrBlob::front() const {
  check(0, "front on empty strblob");
  return data_->front();
}

std::string& StrBlob::back() {
  check(0, "back on empty strblob");
  return data_->back();
}

const std::string& StrBlob::back() const {
  check(0, "back on empty strblob");
  return data_->back();
}

void StrBlob::check(size_type i, const std::string& msg) const {
  if (i >= size()) {
    throw std::out_of_range(msg);
  }
}

} // namespace cp
