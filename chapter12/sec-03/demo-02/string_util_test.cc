#include <gtest/gtest.h>
#include "string_util.h"

using namespace cp;

TEST(unique_split_test, handle_empty_string) {
  const std::string line;
  std::set<std::string> empty_set;

  EXPECT_EQ(unique_split(line), empty_set);
}

TEST(unique_split_test, handle_normal_string) {
  const std::string line = "hello,world hello.world";
  std::set<std::string> ret;

  ret.insert("hello");
  ret.insert("world");

  EXPECT_EQ(unique_split(line), ret);
}
