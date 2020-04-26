#include "text_query_impl.h"

#include <iostream>
#include <set>

#include "string_util.h"

namespace cp {

void TextQueryImpl::ProcessLine(const std::string& line, int line_num) {
  // append to text
  text_.push_back(line);

  // update result_mapping
  std::set<std::string> word_set = unique_split(line);
  for(const auto& word : word_set) {
    if (result_mapping_.find(word) == result_mapping_.end()) {
      QueryResult result;
      result_mapping_[word] = result;
    }
    QueryResult& result = result_mapping_[word];
    result.count++;
    result.line_num_arr.push_back(line_num);
  }
}

void TextQueryImpl::FormatPrint(const std::string& query, const QueryResult& result) const {
  std::string space(4, ' ');

  std::cout << query << " occurs " << result.count << " times" << std::endl;
  for(const auto& ln : result.line_num_arr) {
    const std::string& line_txt = text_[ln - 1];
    std::cout << space << "(line" << ln << ") " << line_txt << std::endl;
  }
}

bool TextQueryImpl::DoQuery(const std::string& query, QueryResult** ppresult) {
  if (result_mapping_.find(query) == result_mapping_.end()) {
    return false;
  }

  *ppresult = &result_mapping_[query];
  return true;
}

} // namespace cp
