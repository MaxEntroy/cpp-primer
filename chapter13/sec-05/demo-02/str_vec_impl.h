#ifndef STR_VEC_IMPL_H_
#define STR_VEC_IMPL_H_

#include <memory>
#include <string>

namespace cp {

class StrVecImpl {
 public:
  StrVecImpl() : first_(nullptr), last_(nullptr), end_(nullptr) {}

  StrVecImpl(const StrVecImpl&) = delete;
  StrVecImpl& operator=(const StrVecImpl&) = delete;

  typedef std::string* Iter;
  typedef const std::string* ConstIter;

 public:
  void create(int sz);
  void create(ConstIter b, ConstIter e);
  void uncreate();

  bool is_enough_room() const { return last_ != end_; }
  void realloc_mem();
  void construct_last(const std::string& val) { alloc_.construct(last_++, val); }

  Iter first() const { return first_; }
  Iter last() const { return last_; }
  Iter end() const { return end_; }

 private:
  void alloc_mem(int sz);

 private:
  std::allocator<std::string> alloc_;

  Iter first_;
  Iter last_;
  Iter end_;
};

} // namespace cp

#endif // STR_VEC_IMPL_H_
