#include <gtest/gtest.h>

#include "str_vec_impl.h"

class StrVecImplTest : public ::testing::Test {
 protected:
   cp::StrVecImpl vi;
};


TEST_F(StrVecImplTest, HandleFirst) {
  EXPECT_EQ(vi.first(), nullptr);
}

TEST_F(StrVecImplTest, HandleLast) {
  EXPECT_EQ(vi.last(), nullptr);
}

TEST_F(StrVecImplTest, HandleEnd) {
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

TEST_F(StrVecImplTest, HandleIncrLast) {
  vi.alloc_mem(0);
  vi.incr_last();

  EXPECT_EQ(vi.last(), vi.end());
}

TEST_F(StrVecImplTest, HandleIsEnoughRoom) {
  EXPECT_FALSE(vi.is_enough_room());

  vi.alloc_mem(1);
  EXPECT_TRUE(vi.is_enough_room());
}

TEST_F(StrVecImplTest, HandleConstruct) {
  vi.alloc_mem(1);
  vi.construct(vi.first(), "hello");

  EXPECT_EQ(*(vi.first()), "hello");
}

TEST_F(StrVecImplTest, HandleReallocMem) {
  vi.alloc_mem(1);
  vi.construct(vi.first(), std::string("hello"));

  vi.realloc_mem();
  EXPECT_EQ(vi.last() - vi.first(), 2);
  EXPECT_EQ(vi.end() - vi.first(), 4);

  EXPECT_EQ(*(vi.first()), "hello");
}

TEST_F(StrVecImplTest, HandleCreateWithSzEquals0) {
  vi.create(0);

  EXPECT_EQ(vi.first(), vi.last());
  EXPECT_EQ(vi.end() - vi.last(), 1);
}

TEST_F(StrVecImplTest, HandleCreate) {
  vi.create(1);

  EXPECT_EQ(vi.last() - vi.first(), 1);
  EXPECT_EQ(vi.end() - vi.first(), 2);
}

TEST_F(StrVecImplTest, HandleCreateWIthIter) {
  vi.create(1);
  vi.construct(vi.first(), "hello");

  cp::StrVecImpl vii;
  vii.create(vi.first(), vi.last());

  EXPECT_EQ(vii.last() - vii.first(), 1);
  EXPECT_EQ(vii.end() - vii.first(), 2);
  EXPECT_EQ(*(vii.first()), "hello");
}

TEST_F(StrVecImplTest, HandleUncreate) {
  vi.alloc_mem(3);
  for(cp::StrVecImpl::Iter it = vi.first(); it != vi.last(); it++) {
    vi.construct(it, std::string());
  }

  vi.uncreate();
  EXPECT_EQ(vi.first(), vi.last());
}

TEST_F(StrVecImplTest, HandleCtor) {
  EXPECT_EQ(vi.first(), nullptr);
  EXPECT_EQ(vi.last(), nullptr);
  EXPECT_EQ(vi.end(), nullptr);
}
