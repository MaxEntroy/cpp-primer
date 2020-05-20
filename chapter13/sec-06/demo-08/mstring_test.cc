#include <iostream>

#include <gtest/gtest.h>

#include "mstring.h"

using namespace cp;

class MStringTest : public ::testing::Test {
 protected:
  MString str;
};

TEST_F(MStringTest, HandleSize) {
  EXPECT_EQ(str.size(), 0);

  MString str("hello");
  EXPECT_EQ(str.size(), 5);
}

TEST_F(MStringTest, HandleCStr) {
  MString str("hello");
  char* s =  str.c_str();

  EXPECT_EQ(s[0], 'h');
  EXPECT_EQ(s[4], 'o');
}


TEST_F(MStringTest, HandleCopyCtor) {
  MString str1("hello");
  MString str2(str1);
  MString str3 = str1;

  EXPECT_EQ(str1.size(), 5);
  EXPECT_EQ(str2.size(), 5);
  EXPECT_EQ(str3.size(), 5);
}

TEST_F(MStringTest, HandleCopyAssignment) {
  MString str1("hello");
  MString str2("hello");
  str1 = str2;
}

TEST_F(MStringTest, HandleMoveCopyAndAssign) {
  MString str1("hello");
  MString str2("world");
  MString str3 = std::move(str1);
  str3 = std::move(str2);
}

TEST_F(MStringTest, HandleSwap) {
  MString str1("hello");
  MString str2("world");
  using std::swap;

  swap(str1, str2);

  char* s1 = str1.c_str();
  char* s2 = str2.c_str();

  EXPECT_EQ(*s1, 'w');
  EXPECT_EQ(*s2, 'h');
}

