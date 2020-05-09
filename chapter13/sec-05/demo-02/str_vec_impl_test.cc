#include <gtest/gtest.h>

#include "str_vec_impl.h"

class StrVecImplTest : public ::testing::Test {
 protected:
   cp::StrVecImpl vi;
};

TEST_F(StrVecImplTest, HandleCtor) {
  EXPECT_EQ(vi.first(), nullptr);
  EXPECT_EQ(vi.last(), nullptr);
  EXPECT_EQ(vi.end(), nullptr);
}

TEST_F(StrVecImplTest, HandleAllocMem) {
  vi.alloc_mem(0);
  EXPECT_EQ(vi.first(), vi.last());
  EXPECT_EQ(vi.end() - vi.first(), 1);

  vi.alloc_mem(3);
  EXPECT_EQ(vi.last() - vi.first(), 3);
  EXPECT_EQ(vi.end() - vi.first(), 6);
}

TEST_F(StrVecImplTest, HandleUncreateAndConstruct) {
  vi.alloc_mem(3);
  for(cp::StrVecImpl::Iter it = vi.first(); it != vi.last(); it++) {
    vi.construct(it, std::string());
  }
}
