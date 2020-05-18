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

  HasPtr(HasPtr&&) noexcept;
  HasPtr& operator=(HasPtr&&) noexcept;

  void PrintStr() const;

  friend void swap(HasPtr&, HasPtr&);

 private:
  int val_;
  std::string* pstr_;
};

inline
void swap(HasPtr& lhs, HasPtr& rhs) {
  using std::swap;
  swap(lhs.val_, rhs.val_);
  swap(lhs.pstr_, rhs.pstr_);
}

} // namespace cp

#endif // HAS_PTR_H_
