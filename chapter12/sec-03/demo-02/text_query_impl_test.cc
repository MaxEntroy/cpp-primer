#include <gtest/gtest.h>

#include "text_query_impl.h"

using namespace cp;

class TextQueryImplTest : public ::testing::Test {
 protected:
  TextQueryImpl tq_impl;
};

TEST_F(TextQueryImplTest, HandleProcessLine) {
  const std::string line1 = "A set element";
  const std::string line2 = "operator element";

  tq_impl.ProcessLine(line1, 1);
  tq_impl.ProcessLine(line2, 2);

  std::vector<std::string> txt;
  txt.push_back(line1);
  txt.push_back(line2);

  EXPECT_EQ(tq_impl.text_, txt);

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

  EXPECT_EQ(tq_impl.result_mapping_, result_map);
}

TEST_F(TextQueryImplTest, HandleFormatPrint) {
  const std::string line1 = "A set element contains only a key.";
  const std::string line2 = "operator creates a new element";

  tq_impl.text_.push_back(line1);
  tq_impl.text_.push_back(line2);

  const std::string query = "element";
  QueryResult result;
  result.count = 2;
  result.line_num_arr.push_back(1);
  result.line_num_arr.push_back(2);

  tq_impl.FormatPrint(query, result);
}

TEST_F(TextQueryImplTest, HandleDoQuery) {
  const std::string line1 = "A set element";
  const std::string line2 = "operator element";

  tq_impl.ProcessLine(line1, 1);
  tq_impl.ProcessLine(line2, 2);

  QueryResult* presult = nullptr;
  EXPECT_FALSE(tq_impl.DoQuery("hello", &presult));
  EXPECT_EQ(presult, nullptr);

  EXPECT_TRUE(tq_impl.DoQuery("A", &presult));
  QueryResult res;
  res.count = 1;
  res.line_num_arr.push_back(1);
  EXPECT_EQ(res, *presult);

  EXPECT_TRUE(tq_impl.DoQuery("set", &presult));
  EXPECT_EQ(res, *presult);

  EXPECT_TRUE(tq_impl.DoQuery("element", &presult));
  res.count = 2;
  res.line_num_arr.push_back(2);
  EXPECT_EQ(res, *presult);

  EXPECT_TRUE(tq_impl.DoQuery("operator", &presult));
  res.count = 1;
  res.line_num_arr.clear();
  res.line_num_arr.push_back(2);
  EXPECT_EQ(res, *presult);
}
