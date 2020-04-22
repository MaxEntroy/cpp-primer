#include <gtest/gtest.h>
#include <stdexcept>

#include "str_blob.h"
#include "weak_str_blob.h"

using namespace cp;

class WeakStrBlobTest : public ::testing::Test {
 protected:
  void SetUp() override {
    str_blob.push_back("hello");
    str_blob.push_back("world");
  }

  // void TearDown() override {}

  StrBlob str_blob;
};

TEST_F(WeakStrBlobTest, HandleConstructor) {
  WeakStrBlob a_wstr_blob;
  EXPECT_EQ(a_wstr_blob.cur(), 0);

  WeakStrBlob b_wstr_blob(str_blob, str_blob.size() - 1);
  EXPECT_EQ(b_wstr_blob.cur(), 1);
}

TEST_F(WeakStrBlobTest, HandleIncr) {
  WeakStrBlob a_wstr_blob;
  EXPECT_THROW(a_wstr_blob.incr(), std::runtime_error);

  WeakStrBlob b_wstr_blob(str_blob, str_blob.size() - 1);
  EXPECT_THROW(b_wstr_blob.incr(), std::out_of_range);

  WeakStrBlob c_wstr_blob(str_blob);
  c_wstr_blob.incr();
  EXPECT_EQ(c_wstr_blob.cur(), 1);
}

TEST_F(WeakStrBlobTest, HandleDecr) {
  WeakStrBlob a_wstr_blob;
  EXPECT_THROW(a_wstr_blob.decr(), std::runtime_error);

  WeakStrBlob b_wstr_blob(str_blob);
  EXPECT_THROW(b_wstr_blob.decr(), std::out_of_range);

  WeakStrBlob c_wstr_blob(str_blob, 1);
  c_wstr_blob.decr();
  EXPECT_EQ(c_wstr_blob.cur(), 0);
}

TEST_F(WeakStrBlobTest, HandleDeref) {
  WeakStrBlob a_wstr_blob;
  EXPECT_THROW(a_wstr_blob.deref(), std::runtime_error);

  WeakStrBlob b_wstr_blob(str_blob, str_blob.size());
  EXPECT_THROW(b_wstr_blob.deref(), std::out_of_range);

  WeakStrBlob c_wstr_blob(str_blob);
  EXPECT_EQ(c_wstr_blob.deref(), "hello");

  c_wstr_blob.incr();
  EXPECT_EQ(c_wstr_blob.deref(), "world");
}

TEST_F(WeakStrBlobTest, HandleCheck) {
  WeakStrBlob a_wstr_blob;
  EXPECT_THROW(a_wstr_blob.check(0, "invalid check"), std::runtime_error);

  WeakStrBlob b_wstr_blob(str_blob);
  EXPECT_THROW(b_wstr_blob.check(-1, "invlid check"), std::out_of_range);
  EXPECT_THROW(b_wstr_blob.check(2, "invlid check"), std::out_of_range);
}
