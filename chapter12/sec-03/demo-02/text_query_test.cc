#include <gtest/gtest.h>

#include "text_query.h"
#include "text_query_impl.h"

using namespace cp;

class TextQueryTest : public ::testing::Test {
};

TEST_F(TextQueryTest, HandleInitError) {
  TextQuery tq("");
  EXPECT_FALSE(tq.Init());
}

TEST_F(TextQueryTest, HandleInit) {
  TextQuery tq("/Users/kang/workspace/myspace/git-personal/cpp-primer/chapter12/sec-03/demo-02/test.dat");
  EXPECT_TRUE(tq.Init());

  const std::string line1 = "A set element";
  const std::string line2 = "operator element";

  std::vector<std::string> txt;
  txt.push_back(line1);
  txt.push_back(line2);

  EXPECT_EQ(txt, tq.pimpl_->text_);

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

  EXPECT_EQ(result_map, tq.pimpl_->result_mapping_);
}
