#include "text_query.h"

#include <fstream>
#include <iostream>
#include <set>
#include <string>

#include "string_util.h"

namespace cp {

bool TextQuery::Init() {
  std::string line;
  std::ifstream fin(file_path_.c_str());
  if(!fin.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return false;
  }

  int line_num = 1;
  while(std::getline(fin, line)) {
    ProcessLine(line, line_num);
    ++line_num;
  }

  fin.close();
  return true;
}

void TextQuery::EventLoop() {
  std::string query;
  while(std::cout << "Plean input a query: ", std::cin >> query) {
    if(result_mapping_.find(query) == result_mapping_.end()) {
      std::cerr << "Invalid query!" << std::endl;
      continue;
    }

    const QueryResult& result = result_mapping_[query];
    FormatPrint(query, result);
  }
}

void TextQuery::FormatPrint(const std::string& query, const QueryResult& result) {
  std::string space(4, ' ');

  std::cout << query << " occurs " << result.count << " times" << std::endl;
  for(const auto& ln : result.line_num_arr) {
    const std::string& line_txt = text_[ln - 1];
    std::cout << space << "(line" << ln << ") " << line_txt << std::endl;
  }
}

void TextQuery::ProcessLine(const std::string& line, int line_num) {
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

} // namespace cp
