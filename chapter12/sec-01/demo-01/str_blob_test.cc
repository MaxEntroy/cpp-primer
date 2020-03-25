#include <stdexcept>
#include <gtest/gtest.h>
#include "str_blob.h"

namespace cp {

class StrBlobTest : public ::testing::Test {
 protected:
  void SetUp() override {
    b_strblob.push_back("hello");
    b_strblob.push_back("world");
  }
  // void TearDown() override {}

  StrBlob a_strblob;
  StrBlob b_strblob;
};

TEST_F(StrBlobTest, HandleSize) {
  EXPECT_EQ(a_strblob.size(), 0);
  EXPECT_EQ(b_strblob.size(), 2);
}

TEST_F(StrBlobTest, HandleEmpty) {
  EXPECT_EQ(a_strblob.empty(), true);
  EXPECT_EQ(b_strblob.empty(), false);
}

TEST_F(StrBlobTest, HandlePushBack) {
  a_strblob.push_back("hello");
  EXPECT_EQ(a_strblob.size(), 1);

  a_strblob.push_back("world");
  EXPECT_EQ(a_strblob.size(), 2);
}

TEST_F(StrBlobTest, HandlePopBack) {
  b_strblob.pop_back();
  EXPECT_EQ(b_strblob.size(), 1);

  b_strblob.pop_back();
  EXPECT_EQ(b_strblob.size(), 0);

  EXPECT_THROW(b_strblob.pop_back(), std::out_of_range);
}

TEST_F(StrBlobTest, HandleFront) {
  EXPECT_THROW(a_strblob.front(), std::out_of_range);

  EXPECT_EQ(b_strblob.front(), "hello");
}

TEST_F(StrBlobTest, HandleBack) {
  EXPECT_THROW(a_strblob.back(), std::out_of_range);

  EXPECT_EQ(b_strblob.back(), "world");
}

TEST_F(StrBlobTest, HandleConstructor) {
  std::initializer_list<std::string> il = {"hello", "world"};
  StrBlob strblob(il);

  EXPECT_EQ(strblob.empty(), false);
  EXPECT_EQ(strblob.size(), 2);
  EXPECT_EQ(strblob.front(), "hello");
  EXPECT_EQ(strblob.back(), "world");
}

TEST_F(StrBlobTest, HandleCopyConstructor) {
  StrBlob c_strblob(b_strblob);

  EXPECT_EQ(c_strblob.front(), "hello");
  EXPECT_EQ(c_strblob.back(), "world");

  b_strblob.pop_back();

  EXPECT_EQ(c_strblob.front(), "hello");
  EXPECT_EQ(c_strblob.back(), "hello");
}

TEST_F(StrBlobTest, HandleAssignment) {
  a_strblob = b_strblob;

  EXPECT_EQ(a_strblob.front(), "hello");
  EXPECT_EQ(a_strblob.back(), "world");

  b_strblob.pop_back();

  EXPECT_EQ(a_strblob.front(), "hello");
  EXPECT_EQ(a_strblob.back(), "hello");
}

TEST_F(StrBlobTest, HandleCheck) {
  EXPECT_NO_THROW(b_strblob.check(0, "out of range"));
  EXPECT_NO_THROW(b_strblob.check(1, "out of range"));
  EXPECT_THROW(b_strblob.check(2, "out of range"), std::out_of_range);
}

}
