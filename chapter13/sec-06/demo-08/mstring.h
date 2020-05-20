#ifndef MSTRING_H_
#define MSTRING_H_

#include <string.h>
#include <utility>

namespace cp {

class MString {
 public:
  MString();
  explicit MString(const char* str);

  ~MString();
  MString(const MString& rhs);
  MString& operator=(const MString&);
  MString(MString&&) noexcept;
  MString& operator=(MString&&) noexcept;

  int size() const { return sz_ - 1; }
  char* c_str() const { return pstr_; }

  friend void swap(MString&, MString&);

 private:
  char* pstr_;
  int sz_;
};

inline
MString::MString() : pstr_(nullptr), sz_(1) {
  pstr_ = new char[sz_];
  pstr_[sz_ - 1] = '\0';
}

inline
MString::MString(const char* str) : pstr_(nullptr), sz_(strlen(str) + 1) {
  pstr_ = new char[sz_];
  strcpy(pstr_, str);
}

inline
MString::~MString() {
  if (pstr_) {
    delete[] pstr_;
  }
}

inline
MString::MString(const MString& rhs) : pstr_(nullptr), sz_(rhs.sz_) {
  pstr_ = new char[sz_];
  strcpy(pstr_, rhs.pstr_);
}

inline
MString::MString(MString&& rhs) noexcept : pstr_(rhs.pstr_), sz_(rhs.sz_) {
  rhs.pstr_ = nullptr;
  rhs.sz_ = 0;
}

inline
void swap(MString& lhs, MString& rhs) {
  using std::swap;
  swap(lhs.pstr_, rhs.pstr_);
  swap(lhs.sz_, rhs.sz_);
}

} // namespace cp

#endif // MSTRING_H_
