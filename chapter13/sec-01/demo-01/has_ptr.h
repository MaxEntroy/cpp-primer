#ifndef HAS_PTR_H_
#define HAS_PTR_H_

#include <string>

namespace cp {

class HasPtr {
 public:
  HasPtr() : i_(0), ps_(nullptr) {}
  HasPtr(int val, const std::string& s) : i_(val), ps_(new std::string(s)) {}

  ~HasPtr() {
    if (ps_) {
      delete ps_;
    }
  }
  HasPtr(const HasPtr&) = delete;
  HasPtr& operator=(const HasPtr&) = delete;
  HasPtr(HasPtr&&) = delete;
  HasPtr& operator=(HasPtr&&) = delete;

 private:
  int i_;
  std::string* ps_;
};

} // namespace cp

#endif // HAS_PTR_H_
