#ifndef HAS_PTR_H_
#define HAS_PTR_H_

#include <string>

namespace cp {

class HasPtr {
 public:
  HasPtr() : val_(0), pstr_(nullptr) {}
  explicit HasPtr(int v) : val_(v), pstr_(nullptr) {}
  explicit HasPtr(const std::string& s) : val_(0), pstr_(new std::string(s)) {}
  HasPtr(int v, const std::string& s) : val_(v), pstr_(new std::string(s)) {}

  ~HasPtr() {
    if(pstr_) {
      delete pstr_;
    }
  }

  HasPtr(const HasPtr&);
  HasPtr& operator=(const HasPtr&);

 private:
  int val_;
  std::string* pstr_;
};

} // namespace cp

#endif // HAS_PTR_H_
