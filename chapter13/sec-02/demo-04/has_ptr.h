#ifndef HAS_PTR_H_
#define HAS_PTR_H_

#include <mutex>
#include <string>

namespace cp {

class StrData {
 public:
  explicit StrData(const std::string& s) : ref(1), data(s) {}

  void IncrRef();
  void DecrRef();
  int GetRef() const {return ref;}

 private:
  int ref;
  std::string data;
  static std::mutex mtx;
};

class HasPtr {
 public:
  HasPtr() : val_(0), pstr_(nullptr) {}
  explicit HasPtr(int v) : val_(v), pstr_(nullptr) {}
  explicit HasPtr(const std::string& s) : val_(0), pstr_(new StrData(s)) {}
  HasPtr(int v, const std::string& s) : val_(v), pstr_(new StrData(s)) {}

  ~HasPtr();
  HasPtr(const HasPtr&);
  HasPtr& operator=(const HasPtr&);

 private:
   int val_;
   StrData* pstr_;
};

} // namespace cp

#endif // HAS_PTR_H_
