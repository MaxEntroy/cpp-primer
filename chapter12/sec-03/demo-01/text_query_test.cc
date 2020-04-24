#include <fstream>

#include <gtest/gtest.h>

#include "text_query.h"

using namespace cp;

class TextQueryTest : public ::testing::Test {
 protected:
  // void SetUp() override {}
  // void TearDown() override {}

  TextQuery tq;
  std::string test_file_path;
};

TEST_F(TextQueryTest, HandleInitError) {
  tq.file_path_ = "";
  EXPECT_EQ(tq.Init(), false);
}

TEST_F(TextQueryTest, HandleInit) {
  tq.file_path_ = "/home/kang/workspace/myspace/git-personal/cpp-primer/chapter12/sec-03/demo-01/test.dat";
  EXPECT_EQ(tq.Init(), true);

  const std::string line1 = "A set element";
  const std::string line2 = "operator element";

  std::vector<std::string> txt;
  txt.push_back(line1);
  txt.push_back(line2);

  EXPECT_EQ(tq.text_, txt);

  std::unordered_map<std::string, QueryResult> result_map;
  QueryResult result;

  result.count = 1;
  result.line_num_arr.push_back(1);
  result_map["A"] = result;
  result_map["set"] = result;

  result.count = 2;
  result.line_num_arr.push_back(2);
  result_map["element"] = result;

  result.count = 1;
  result.line_num_arr.clear();
  result.line_num_arr.push_back(2);
  result_map["operator"] = result;

  EXPECT_EQ(tq.result_mapping_, result_map);
}

TEST_F(TextQueryTest, HandleFormatPrint) {
  const std::string line1 = "A set element contains only a key.";
  const std::string line2 = "operator creates a new element";

  tq.text_.push_back(line1);
  tq.text_.push_back(line2);

  const std::string query = "element";
  QueryResult result;
  result.count = 2;
  result.line_num_arr.push_back(1);
  result.line_num_arr.push_back(2);

  tq.FormatPrint(query, result);
}

TEST_F(TextQueryTest, HandleProcessLine) {
  const std::string line1 = "A set element";
  const std::string line2 = "operator element";

  tq.ProcessLine(line1, 1);
  tq.ProcessLine(line2, 2);

  std::vector<std::string> txt;
  txt.push_back(line1);
  txt.push_back(line2);

  EXPECT_EQ(tq.text_, txt);

  std::unordered_map<std::string, QueryResult> result_map;
  QueryResult result;

  result.count = 1;
  result.line_num_arr.push_back(1);
  result_map["A"] = result;
  result_map["set"] = result;

  result.count = 2;
  result.line_num_arr.push_back(2);
  result_map["element"] = result;

  result.count = 1;
  result.line_num_arr.clear();
  result.line_num_arr.push_back(2);
  result_map["operator"] = result;

  EXPECT_EQ(tq.result_mapping_, result_map);
}
