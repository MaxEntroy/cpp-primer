#ifndef STR_VEC_H_
#define STR_VEC_H_

#include <initializer_list>
#include <memory>
#include <string>

#include <iostream>

namespace cp {

class StrVec {
 public:
  StrVec(int sz = 0) : first_(nullptr), last_(nullptr), end_(nullptr) { create(sz); }
  StrVec(std::initializer_list<std::string> il) : first_(nullptr), last_(nullptr),
    end_(nullptr) { create(il.begin(), il.end()); }

  ~StrVec() { uncreate(); }
  StrVec(const StrVec& rhs) { create(rhs.begin(), rhs.end()); std::cout << "copy ctor called.\n";}
  StrVec& operator=(const StrVec&);
  StrVec(StrVec&&) noexcept;
  StrVec& operator=(StrVec&&) noexcept;


  typedef std::string* Iter;
  typedef const std::string* ConstIter;

 public:
  Iter begin() const { return first_; }
  Iter end() const { return last_; }

  size_t size() const { return last_ - first_; }
  size_t capacity() const { return end_ - first_; }

  void push_back(const std::string& val);
  void push_back(std::string&& val);

 private:
  void create(int sz);
  void create(ConstIter b, ConstIter e);
  void uncreate();

  bool is_enough_room() const { return last_ != end_; }
  void alloc_mem(int sz);
  void realloc_mem();

 private:
  std::allocator<std::string> alloc_;

  Iter first_;
  Iter last_;
  Iter end_;
};

} // namespace cp

#endif // STR_VEC_H_
