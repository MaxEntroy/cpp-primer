#ifndef TEXT_QUERY_IMPL_H_
#define TEXT_QUERY_IMPL_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace cp {

struct QueryResult {
  int count;
  std::vector<int> line_num_arr;

  QueryResult() : count(0) {}

  bool operator==(const QueryResult& rhs) const {
    return count == rhs.count && line_num_arr == rhs.line_num_arr;
  }
};

class TextQueryImpl {
 public:
  void ProcessLine(const std::string& line, int line_num);

  void FormatPrint(const std::string& query, const QueryResult& result) const;

  bool DoQuery(const std::string& query, QueryResult** ppresult );

 private:
  std::vector<std::string> text_;
  std::unordered_map<std::string, QueryResult> result_mapping_;
};

} // namespace cp

#endif // TEXT_QUERY_IMPL_H
