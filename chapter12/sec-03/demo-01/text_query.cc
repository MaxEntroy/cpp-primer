#include "text_query.h"

#include <fstream>
#include <iostream>
#include <string>

namespace cp {

void TextQuery::Init() {
  std::string line;
  std::ifstream fin(file_path_.c_str());
  if(!fin.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return;
  }

  while(std::getline(fin, line)) {
    ProcessLine(line);
  }

  fin.close();
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

} // namespace cp
