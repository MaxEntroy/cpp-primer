#ifndef STR_VEC_H_
#define STR_VEC_H_

#include <initializer_list>
#include <memory>
#include <string>

namespace cp {

class StrVecImpl;

class StrVec {
 public:
  StrVec(int sz = 0);
  StrVec(std::initializer_list<std::string> il);

  ~StrVec();
  StrVec(const StrVec&);
  StrVec& operator=(const StrVec&);

  typedef std::string* Iter;
  typedef const std::string* ConstIter;

 public:
  Iter begin() const;
  Iter end() const;

  size_t size() const;
  size_t capacity() const;

  void push_back(const std::string& val);

 private:
  std::unique_ptr<StrVecImpl> pimpl_;
};

} // namespace cp

#endif // STR_VEC_H_
