#include <vector>

#include <gtest/gtest.h>

#include "str_vec.h"

class StrVecTest : public ::testing::Test {
 protected:
  // void SetUp() override;
  // void TearDown() override;
   cp::StrVec v;
};

TEST_F(StrVecTest, HandleDefaultCtor) {
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 1);

  cp::StrVec v1(3);
  EXPECT_EQ(v1.size(), 3);
  EXPECT_EQ(v1.capacity(), 6);
}

TEST_F(StrVecTest, HandleDefaultCtorWithIl) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);
}

TEST_F(StrVecTest, HandleCopyCtor) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  cp::StrVec v2(v1);

  EXPECT_EQ(v2.size(), 4);
  EXPECT_EQ(v2.capacity(), 8);
}

TEST_F(StrVecTest, HandleAssignment) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});

  v = v1;
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 8);
}

TEST_F(StrVecTest, HandleIterator) {
  std::initializer_list<std::string> il = {"hello", "world", "hello", "cpp"};
  std::vector<std::string> input(il);

  cp::StrVec v1(il);
  std::vector<std::string> output;
  for(auto it = v1.begin(); it != v1.end(); ++it) {
    output.push_back(*it);
  }

  EXPECT_EQ(input, output);
}

TEST_F(StrVecTest, HandleReserve) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);

  v1.reserve(7);
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);

  v1.reserve(8);
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);
}

TEST_F(StrVecTest, HandleResize) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);

  v1.resize(4);
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);

  v1.resize(3);
  EXPECT_EQ(v1.size(), 3);
  EXPECT_EQ(v1.capacity(), 8);

  v1.resize(5);
  EXPECT_EQ(v1.size(), 5);
  EXPECT_EQ(v1.capacity(), 8);

  v1.resize(8);
  EXPECT_EQ(v1.size(), 8);
  EXPECT_EQ(v1.capacity(), 8);

  v1.resize(10);
  EXPECT_EQ(v1.size(), 10);
  EXPECT_EQ(v1.capacity(), 10);
}

TEST_F(StrVecTest, HandlePushBack) {
  std::vector<std::string> in({"hello","world","hello","cap","hello","gtest"});

  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 1);

  v.push_back(in[0]);
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v.capacity(), 1);

  v.push_back(in[1]);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.capacity(), 2);

  v.push_back(in[2]);
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.capacity(), 4);

  v.push_back(in[3]);
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 4);

  v.push_back(in[4]);
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.capacity(), 8);

  v.push_back(in[5]);
  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v.capacity(), 8);

  std::vector<std::string> out;
  for(auto it = v.begin(); it != v.end(); ++it) {
    out.push_back(*it);
  }

  EXPECT_EQ(in, out);
}
