#include "text_query.h"

#include <fstream>
#include <iostream>

#include "text_query_impl.h"

namespace cp {

TextQuery::TextQuery(const std::string& path) : file_path_(path), pimpl_(new TextQueryImpl()) {}

TextQuery::~TextQuery() = default;

bool TextQuery::Init() {
  std::ifstream fin(file_path_.c_str());
  if(!fin.is_open()) {
    std::cerr << "Unable to open file!" << std::endl;
    return false;
  }

  int line_num = 1;
  std::string line;
  while(std::getline(fin, line)) {
    pimpl_->ProcessLine(line, line_num);
    ++line_num;
  }

  fin.close();
  return true;
}

void TextQuery::EventLoop() {
  std::string query;
  QueryResult* presult = nullptr;

  while(std::cout << "Plean input a query: ", std::cin >> query) {
    if (!(pimpl_->DoQuery(query, &presult))) {
      std::cerr << "Invalid query!" << std::endl;
      continue;
    }

    pimpl_->FormatPrint(query, *presult);
  }
}

} // namespace cp

