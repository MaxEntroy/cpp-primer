#ifndef WEAK_STR_BLOB_H_
#define WEAK_STR_BLOB_H_

#include <memory>
#include "str_blob.h"

#ifdef PTEST
#define private public
#endif

namespace cp {

class WeakStrBlob {
 public:
  typedef std::vector<std::string> VecStr;
  typedef VecStr::size_type size_type;

  WeakStrBlob() : cur_(0) {}
  WeakStrBlob(StrBlob& str_blob, size_type cur = 0) : cur_(cur),  data_(str_blob.data_) {}

 public:
  size_type cur() const {return cur_;}
  void incr();
  void decr();
  std::string& deref();

 private:
  std::shared_ptr<VecStr> check(size_type i, const std::string& msg) const;

 private:
  size_type cur_;
  std::weak_ptr<VecStr> data_;
};

} // namespace cp

#endif // WEAK_STR_BLOB_H_
