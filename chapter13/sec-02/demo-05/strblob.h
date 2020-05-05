#ifndef STRBLOB_H_
#define STRBLOB_H_

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

namespace cp {

class StrBlob {
 public:
  typedef std::vector<std::string> VecStr;
  explicit StrBlob(std::initializer_list<std::string> str_il) : data_(std::make_shared<VecStr>(str_il)) {}

  StrBlob(const StrBlob&);
  StrBlob& operator=(const StrBlob&);

 private:
  std::shared_ptr<VecStr> data_;
};

} // namespace cp


#endif // STRBLOB_H_
