#ifndef HAS_PTR_H_
#define HAS_PTR_H_

#include <memory>
#include <string>

namespace cp {

class HasPtr {
 public:
  HasPtr() : val_(0), sptr_(nullptr) {}
  explicit HasPtr(int v) : val_(v), sptr_(nullptr) {}
  explicit HasPtr(const std::string& s) : val_(0), sptr_(std::make_shared<std::string>(s)) {}
  HasPtr(int v, const std::string& s) : val_(v), sptr_(std::make_shared<std::string>(s)) {}

 private:
  int val_;
  std::shared_ptr<std::string> sptr_;
};

} // namespace cp

#endif // HAS_PTR_H_
