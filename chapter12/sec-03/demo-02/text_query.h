#ifndef TEXT_QUERY_H_
#define TEXT_QUERY_H_

#include <memory>
#include <string>

namespace cp {

class TextQueryImpl;

class TextQuery {
 public:
  explicit TextQuery(const std::string& path);
  ~TextQuery();

  TextQuery(const TextQuery&) = delete;
  TextQuery& operator=(const TextQuery&) = delete;

  TextQuery(TextQuery&&) = default;
  TextQuery& operator=(TextQuery&&) = default;

 public:
  bool Init();

  void EventLoop();

 private:
  std::string file_path_;
  std::unique_ptr<TextQueryImpl> pimpl_;
};

} // namespace cp

#endif // TEXT_QUERY_H
