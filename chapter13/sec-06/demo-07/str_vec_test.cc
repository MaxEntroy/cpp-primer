#include <iostream>
#include <gtest/gtest.h>

#include "str_vec.h"

class StrVecTest : public ::testing::Test {
 public:
  cp::StrVec GetStrVec() {
    std::cout << "GetStrVec() called.\n";
    return v;
   }

 protected:
   cp::StrVec v;

};

TEST_F(StrVecTest, HandleDefaultCtor) {
  EXPECT_EQ(v.size(), 0);
  EXPECT_EQ(v.capacity(), 1);

  cp::StrVec v1(3);
  EXPECT_EQ(v1.size(), 3);
  EXPECT_EQ(v1.capacity(), 6);
}

TEST_F(StrVecTest, HandleCopyCtor) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);

  cp::StrVec v2 = v1;
  EXPECT_EQ(v2.size(), 4);
  EXPECT_EQ(v2.capacity(), 8);
}

TEST_F(StrVecTest, HandleAssignment) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  EXPECT_EQ(v1.size(), 4);
  EXPECT_EQ(v1.capacity(), 8);

  v = v1;
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 8);
}

TEST_F(StrVecTest, HandleMoveCtor) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  v = v1;
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 8);

  cp::StrVec v2 = GetStrVec();
  EXPECT_EQ(v2.size(), 4);
  EXPECT_EQ(v2.capacity(), 8);
}

TEST_F(StrVecTest, HandleMoveAssignment) {
  cp::StrVec v1({"hello", "world", "hello", "cpp"});
  v = v1;
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 8);

  cp::StrVec v2;
  v2 = GetStrVec();

  EXPECT_EQ(v2.size(), 4);
  EXPECT_EQ(v2.capacity(), 8);
}

TEST_F(StrVecTest, HandlePushBack) {
  v.push_back("hello");
  EXPECT_EQ(v.size(), 1);
  EXPECT_EQ(v.capacity(), 1);

  v.push_back("world");
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.capacity(), 2);

  v.push_back("hello");
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v.capacity(), 4);

  v.push_back("cpp");
  EXPECT_EQ(v.size(), 4);
  EXPECT_EQ(v.capacity(), 4);

  v.push_back("hello");
  EXPECT_EQ(v.size(), 5);
  EXPECT_EQ(v.capacity(), 8);

  v.push_back("gtest");
  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v.capacity(), 8);
}
