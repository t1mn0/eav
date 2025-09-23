#include <gtest/gtest.h>

#include "../../SmartPtr/UniquePtr/UniquePtr.hpp"
#include "../Utils.hpp"

class UniquePtrArrayFixture : public ::testing::Test {
protected:
void SetUp() override {
  tmn::test_utils::TestObject::reset_counts();
}

void TearDown() override {
  tmn::test_utils::TestObject::reset_counts();
}
};

TEST_F(UniquePtrArrayFixture, ArrayDefaultConstructor) {
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr;

  EXPECT_FALSE(ptr.has_resource());
  EXPECT_FALSE(static_cast<bool>(ptr));
}

TEST_F(UniquePtrArrayFixture, ArrayConstructorWithPointer) {
  auto* raw_array = new tmn::test_utils::TestObject[3]{tmn::test_utils::TestObject(1), tmn::test_utils::TestObject(2), tmn::test_utils::TestObject(3)};
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr(raw_array);

  EXPECT_TRUE(ptr.has_resource());
}

TEST_F(UniquePtrArrayFixture, ArrayMoveOperations) {
  auto* raw_array = new tmn::test_utils::TestObject[2]{tmn::test_utils::TestObject(10), tmn::test_utils::TestObject(20)};
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr1(raw_array);
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr2(std::move(ptr1));

  EXPECT_FALSE(ptr1.has_resource());
  EXPECT_TRUE(ptr2.has_resource());
}

TEST_F(UniquePtrArrayFixture, ArrayIndexOperator) {
  auto* raw_array = new tmn::test_utils::TestObject[3]{tmn::test_utils::TestObject(1), tmn::test_utils::TestObject(2), tmn::test_utils::TestObject(3)};
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr(raw_array);

  EXPECT_EQ(ptr[0].value, 1);
  EXPECT_EQ(ptr[1].value, 2);
  EXPECT_EQ(ptr[2].value, 3);

  ptr[1].value = 99;
  EXPECT_EQ(ptr[1].value, 99);
}

TEST_F(UniquePtrArrayFixture, ArrayIndexOperatorThrowsWhenEmpty) {
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr;

  EXPECT_THROW(
    { ptr[0]; },
    std::runtime_error
  );
}

TEST_F(UniquePtrArrayFixture, ArrayTryGetValAt) {
  auto* raw_array = new tmn::test_utils::TestObject[2]{tmn::test_utils::TestObject(10), tmn::test_utils::TestObject(20)};
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr(raw_array);

  auto result1 = ptr.try_get_val_at(0);
  auto result2 = ptr.try_get_val_at(1);

  EXPECT_TRUE(result1.has_value());
  EXPECT_EQ(result1.value().value, 10);

  EXPECT_TRUE(result2.has_value());
  EXPECT_EQ(result2.value().value, 20);
}

TEST_F(UniquePtrArrayFixture, ArrayReset) {
auto* raw_array1 = new tmn::test_utils::TestObject[2]{tmn::test_utils::TestObject(1), tmn::test_utils::TestObject(2)};
auto* raw_array2 = new tmn::test_utils::TestObject[1]{tmn::test_utils::TestObject(3)};

tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr(raw_array1);
  ptr.reset(raw_array2);

  EXPECT_TRUE(ptr.has_resource());
  EXPECT_EQ(ptr[0].value, 3);
  EXPECT_EQ(tmn::test_utils::TestObject::destructor_count, 2);
}

TEST_F(UniquePtrArrayFixture, ArrayDestructorDestroysAllElements) {
tmn::test_utils::TestObject::reset_counts();

  {
    auto* raw_array = new tmn::test_utils::TestObject[5];
    tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr(raw_array);
    EXPECT_EQ(tmn::test_utils::TestObject::constructor_count, 5);
  }

  EXPECT_EQ(tmn::test_utils::TestObject::destructor_count, 5);
}

TEST_F(UniquePtrArrayFixture, ArraySwap) {
  auto* raw_array1 = new tmn::test_utils::TestObject[2]{tmn::test_utils::TestObject(1), tmn::test_utils::TestObject(2)};
  auto* raw_array2 = new tmn::test_utils::TestObject[3]{tmn::test_utils::TestObject(3), tmn::test_utils::TestObject(4), tmn::test_utils::TestObject(5)};

  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr1(raw_array1);
  tmn::UniquePtr<tmn::test_utils::TestObject[]> ptr2(raw_array2);

  swap(ptr1, ptr2);

  EXPECT_EQ(ptr1[0].value, 3);
  EXPECT_EQ(ptr1[1].value, 4);
  EXPECT_EQ(ptr2[0].value, 1);
  EXPECT_EQ(ptr2[1].value, 2);
}
