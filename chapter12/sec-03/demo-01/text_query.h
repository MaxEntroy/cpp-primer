#ifndef TEXT_QUERY_H_
#define TEXT_QUERY_H_

#include <string>
#include <unordered_map>
#include <vector>

#ifdef PTEST
#define private public
#endif

namespace cp {

struct QueryResult {
  int count;
  std::vector<int> line_num_arr;

  QueryResult() : count(0) {}

  bool operator==(const QueryResult& rhs) const {
    return count == rhs.count && line_num_arr == rhs.line_num_arr;
  }
};

class TextQuery {
 public:
   TextQuery() {}
   explicit TextQuery(const std::string& path) : file_path_(path) {}

   bool Init();
   void EventLoop();

 private:
   void ProcessLine(const std::string& line, int line_num);
   void FormatPrint(const std::string& query, const QueryResult& result);

 private:
  std::string file_path_;

  std::vector<std::string> text_;
  std::unordered_map<std::string, QueryResult> result_mapping_;
};

} // namespace cp

#endif // TEXT_QUERY_H_
