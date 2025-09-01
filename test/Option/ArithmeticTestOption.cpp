#include <gtest/gtest.h>

#include <random>
#include <string>

#include "../../Option/Option.hpp"

std::mt19937& get_random_engine() {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  return gen;
}

template<typename T>
T generate_random_val(T min, T max) {
  if constexpr (std::is_integral_v<T>) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(get_random_engine());
  }
  else {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(get_random_engine());
  }
}

struct RandomTestData {
  static constexpr int min_int = 1;
  static constexpr int max_int = 100;
  static constexpr double min_double = 0.1;
  static constexpr double max_double = 10.0;

  int random_int_1 = generate_random_val(min_int, max_int);
  int random_int_2 = generate_random_val(min_int, max_int);
  double random_double_1 = generate_random_val(min_double, max_double);
  double random_double_2 = generate_random_val(min_double, max_double);
};

class OptionArithmeticTestFixture : public ::testing::Test {
protected: //* fields:
  RandomTestData test_data;

protected: //* methods:
  void SetUp() override {
    test_data = RandomTestData{};
  }

};

TEST_F(OptionArithmeticTestFixture, IntAdditionWithRandomValues) {
  tmn::err::Option<int> a(test_data.random_int_1);
  tmn::err::Option<int> b(test_data.random_int_2);

  const auto c = a + b;

  ASSERT_TRUE(c.has_value()) << "Sum of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 + test_data.random_int_2);
}

TEST_F(OptionArithmeticTestFixture, DoubleAdditionWithRandomValues) {
  tmn::err::Option<double> a(test_data.random_double_1);
  tmn::err::Option<double> b(test_data.random_double_2);

  const auto c = a + b;

  ASSERT_TRUE(c.has_value()) << "Sum of the Option<double> for two Options with valid values should have the value";
  EXPECT_DOUBLE_EQ(c.value(), test_data.random_double_1 + test_data.random_double_2);
}

TEST_F(OptionArithmeticTestFixture, SubtractionWithRandomValues) {
  tmn::err::Option<int> a(test_data.random_int_1);
  tmn::err::Option<int> b(test_data.random_int_2);

  const auto c = a - b;

  ASSERT_TRUE(c.has_value()) << "Subtraction of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 - test_data.random_int_2);
}

TEST_F(OptionArithmeticTestFixture, MultiplicationWithRandomValues) {
  tmn::err::Option<int> a(test_data.random_int_1);
  tmn::err::Option<int> b(test_data.random_int_2);

  const auto c = a * b;

  ASSERT_TRUE(c.has_value()) << "Multiplication of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 * test_data.random_int_2);
}

TEST_F(OptionArithmeticTestFixture, DivisionWithRandomValues) {
  // Divisor must not be zero:
  const int divisor = generate_random_val(1, RandomTestData::max_int);

  tmn::err::Option<int> dividend(test_data.random_int_1);
  tmn::err::Option<int> divisor_opt(divisor);

  const auto c = dividend / divisor_opt;

  ASSERT_TRUE(c.has_value()) << "Division of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(c.value(), test_data.random_int_1 / divisor);
}

TEST_F(OptionArithmeticTestFixture, DivisionByZero) {
  tmn::err::Option<int> a(test_data.random_int_1);
  tmn::err::Option<int> zero(0);

  const auto c = a / zero;

  EXPECT_FALSE(c.has_value());
}

TEST_F(OptionArithmeticTestFixture, CompoundAssignmentOperators) {
  tmn::err::Option<int> a(test_data.random_int_1);
  // Divisor must not be zero:
  const tmn::err::Option<int> b = generate_random_val(1, RandomTestData::max_int);
  const int original_value = a.value();

  a += b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentSum of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value + test_data.random_int_2);

  a -= b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentSubtraction of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value);

  a *= b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentMultiplication of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value * test_data.random_int_2);

  a = tmn::err::Option<int>(original_value * test_data.random_int_2);
  a /= b;
  ASSERT_TRUE(a.has_value()) << "CompoundAssignmentDivision of the Option<int> for two Options with valid values should have the value";
  EXPECT_EQ(a.value(), original_value);
}

TEST_F(OptionArithmeticTestFixture, MixedTypesArithmetic) {
  tmn::err::Option<int> int_val(test_data.random_int_1);
  tmn::err::Option<double> double_val(test_data.random_double_1);

  const auto res = int_val * double_val;

  static_assert(std::is_same_v<decltype(res), const tmn::err::Option<double>>);
  ASSERT_TRUE(res.has_value()) << "MixedTypesMultiplication of the Option(int&double) for two Options with valid values should have the value";
  EXPECT_DOUBLE_EQ(res.value(), test_data.random_int_1 * test_data.random_double_1);
}

TEST_F(OptionArithmeticTestFixture, StringConcatenation) {
  const std::string prefix = "Hello";
  const std::string suffix = " World";

  tmn::err::Option<std::string> a(prefix);
  tmn::err::Option<std::string> b(suffix);

  const auto c = a + b;

  ASSERT_TRUE(c.has_value()) << "After concatenating valid strings under Option<string>, we should get Option<string> with a valid value";
  EXPECT_EQ(c.value(), prefix + suffix);
}

TEST_F(OptionArithmeticTestFixture, OperationsWithEmptyOperands) {
  tmn::err::Option<int> filled_value(test_data.random_int_1);
  tmn::err::Option<int> empty_value;

  // Binary operations with empty operand:
  const auto addition_result = filled_value + empty_value;
  const auto subtraction_result = empty_value - filled_value;

  EXPECT_FALSE(addition_result.has_value());
  EXPECT_FALSE(subtraction_result.has_value());

  // CompoundAssignment operations with empty operand:
  tmn::err::Option<int> mutable_value(test_data.random_int_1);
  mutable_value += empty_value;

  EXPECT_FALSE(mutable_value.has_value());
}

TEST_F(OptionArithmeticTestFixture, ChainedOperations) {
  const int value1 = test_data.random_int_1;
  const int value2 = test_data.random_int_2;
  const int value3 = generate_random_val(RandomTestData::min_int, RandomTestData::max_int);

  tmn::err::Option<int> a(value1);
  tmn::err::Option<int> b(value2);
  tmn::err::Option<int> c(value3);

  const auto d = (a + b) * c;

  ASSERT_TRUE(d.has_value());
  EXPECT_EQ(d.value(), (value1 + value2) * value3);
}
