#ifndef FOO_H_
#define FOO_H_

#include <mutex>

namespace cp {

class Foo {
 public:
  Foo() : uid_(0) {uid_ = incr();}

  Foo(const Foo&) {uid_ = incr();}
  Foo& operator=(const Foo&) {uid_ = incr(); return *this;}

  int uid() const {return uid_;}

 private:
  static int incr();

 private:
  int uid_;

  static int counter_;
  static std::mutex mtx_;
};


} // namespace cp

#endif // FOO_H_
