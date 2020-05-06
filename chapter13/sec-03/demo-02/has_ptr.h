#ifndef HAS_PTR_H_
#define HAS_PTR_H_

#include <iostream>
#include <string>

namespace cp {

class HasPtr {
 public:
  HasPtr() : val_(0), pstr_(nullptr) {}
  explicit HasPtr(int v) : val_(v), pstr_(nullptr) {}
  explicit HasPtr(const std::string& s) : val_(0), pstr_(new std::string(s)) {}
  HasPtr(int v, const std::string& s) : val_(v), pstr_(new std::string(s)) {}

  ~HasPtr();
  HasPtr(const HasPtr&);
  HasPtr& operator=(HasPtr);

 public:
  void PrintStr() const;
  bool operator<(const HasPtr&) const;

  friend void swap(HasPtr&, HasPtr&);

 private:
  int val_;
  std::string* pstr_;
};

inline
void swap(HasPtr& lhs, HasPtr& rhs) {
  std::cout << "Swap of HasPtr called.\n";
  using std::swap;
  swap(lhs.val_, rhs.val_);
  swap(lhs.pstr_, rhs.pstr_);
}

} // namespace cp

#endif // HAS_PTR_H_
