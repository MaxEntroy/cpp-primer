#include <algorithm>
#include <vector>

#include <gtest/gtest.h>

#include "str_ptr.h"

class StrPtrTest : public ::testing::Test {
 protected:
  cp::StrPtr ps;
};

TEST_F(StrPtrTest, HandleOperator) {
  EXPECT_EQ(*ps, "");

  cp::StrPtr ps1("hello");
  EXPECT_EQ(*ps1, "hello");
}

TEST_F(StrPtrTest, HandleCopyCtor) {
  cp::StrPtr p1("hello");
  cp::StrPtr p2 = p1;

  EXPECT_EQ(*p2, "hello");
}

TEST_F(StrPtrTest, HandleCopyAssignment) {
  cp::StrPtr p1("hello");
  cp::StrPtr p2("world");

  EXPECT_EQ(*p1, "hello");
  EXPECT_EQ(*p2, "world");

  p1 = p2;
  EXPECT_EQ(*p1, "world");

  p1 = p1;
  EXPECT_EQ(*p1, "world");
}

TEST_F(StrPtrTest, HandleMoveCtor) {
  cp::StrPtr p1("hello");
  cp::StrPtr p2 = std::move(p1); // p1 is destructible and should not be used anymore


  EXPECT_EQ(*p2, "hello");
}

TEST_F(StrPtrTest, HandleMoveAssignment) {
  cp::StrPtr p1("hello");
  cp::StrPtr p2("world");

  p2 = std::move(p1); // p1 is destructible.
  EXPECT_EQ(*p2, "hello");

  p2 = std::move(p2); // p2 is not be destroyed
  EXPECT_EQ(*p2, "hello");
}

TEST_F(StrPtrTest, HandleSwap) {
  cp::StrPtr p1("hello");
  cp::StrPtr p2("world");

  EXPECT_EQ(*p1, "hello");
  EXPECT_EQ(*p2, "world");

  using std::swap;
  swap(p1, p2);
  EXPECT_EQ(*p2, "hello");
  EXPECT_EQ(*p1, "world");
}

TEST_F(StrPtrTest, HandleLeq) {
  cp::StrPtr p1("world");
  cp::StrPtr p2("hello");

  std::vector<cp::StrPtr> v;
  v.push_back(p1);
  v.push_back(p2);

  std::sort(v.begin(), v.end());

  EXPECT_EQ(*v[0], "hello");
  EXPECT_EQ(*v[1], "world");
}
