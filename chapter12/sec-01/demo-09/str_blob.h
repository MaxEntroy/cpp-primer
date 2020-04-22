#ifndef STR_BLOB_H_
#define STR_BLOB_H_

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace cp {

class StrBlob {
 public:
  typedef std::vector<std::string> VecStr;
  typedef VecStr::size_type size_type;
  friend class StrBlobTest;
  friend class WeakStrBlob;

 public:
  StrBlob() : data_(std::make_shared<VecStr>()) {}
  explicit StrBlob(std::initializer_list<std::string> il) : data_(std::make_shared<VecStr>(il)) {}

  size_type size() const { return data_->size(); }
  bool empty() const { return data_->empty(); }

 public:
  void push_back(const std::string& item);
  void pop_back();

  std::string& front();
  const std::string& front() const;

  std::string& back();
  const std::string& back() const;

 private:
  // throw msg, if check *data_[i] isn't valid
  void check(size_type i, const std::string& msg) const;
  FRIEND_TEST(StrBlobTest, HandleCheck);

 private:
  std::shared_ptr<VecStr> data_;
};

} // namespace cp

#endif // STR_BLOB_H_
