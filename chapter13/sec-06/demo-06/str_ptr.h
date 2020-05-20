#ifndef STR_PTR_H_
#define STR_PTR_H_

#include <iostream>
#include <string>

namespace cp {

class StrPtr {
 public:
  StrPtr() : ptr_(new std::string()) {}
  explicit StrPtr(const std::string& s) : ptr_(new std::string(s)) {}

  ~StrPtr();
  StrPtr(const StrPtr&);
  StrPtr& operator=(const StrPtr&);
  StrPtr(StrPtr&&);
  StrPtr& operator=(StrPtr&&);

  const std::string& operator*() const;
  bool operator<(const StrPtr&) const;

  friend void swap(StrPtr&, StrPtr&);

 private:
  std::string* ptr_;
};

inline
void swap(StrPtr& lhs, StrPtr& rhs) {
  std::cout << "swap is called.\n";
  using std::swap;
  swap(lhs.ptr_, rhs.ptr_);
}

} // namespace cp

#endif // STR_PTR_H_
