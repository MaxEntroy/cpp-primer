#include "foo.h"

namespace cp {

int Foo::counter_ = 0;

std::mutex Foo::mtx_;

int Foo::incr() {
  std::lock_guard<std::mutex> lck(mtx_);
  return ++counter_;
}

} // namespace cp
