#ifndef TEXT_QUERY_H_
#define TEXT_QUERY_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace cp {

struct QueryResult {
  int count;
  std::vector<int> line_num_arr;
};

class TextQuery {
 public:
   void Init();
   void EventLoop();

 private:
   void ProcessLine(const std::string& line);
   void FormatPrint(const std::string& query, const QueryResult& result);

 private:
  std::string file_path_;

  std::vector<std::string> text_;
  std::unordered_map<std::string, QueryResult> result_mapping_;
};

} // namespace cp

#endif // TEXT_QUERY_H_
