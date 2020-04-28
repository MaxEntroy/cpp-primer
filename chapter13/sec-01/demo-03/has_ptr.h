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

  HasPtr(const HasPtr& rhs) {
    i_ = rhs.i_;
    ps_ = (rhs.ps_)?(new std::string(*rhs.ps_)):nullptr;
  }

  HasPtr& operator=(const HasPtr& rhs) {
    i_ = rhs.i_;

    if (ps_) {
      delete ps_;
    }

    ps_ = (rhs.ps_)?(new std::string(*rhs.ps_)):nullptr;

    return *this;
  }

 private:
  int i_;
  std::string* ps_;
};

} // namespace cp

#endif // HAS_PTR_H_
