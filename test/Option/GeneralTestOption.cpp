#include <gtest/gtest.h>
#include <random>
#include <string>

#include "../../Option/Option.hpp"
#include "../../Error/Error.hpp"

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
  } else {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(get_random_engine());
  }
}

struct RandomTestData {
  static constexpr int min_int = 1;
  static constexpr int max_int = 1000;
  static constexpr double min_double = 0.1;
  static constexpr double max_double = 100.0;

  int random_int = generate_random_val(min_int, max_int);
  double random_double = generate_random_val(min_double, max_double);
  std::string random_string = "test_" + std::to_string(random_int);
};

class OptionDefaultConstructorFixture : public ::testing::Test {
protected:
  tmn::err::Option<int> default_opt;
};

TEST_F(OptionDefaultConstructorFixture, DefaultConstructor) {
  EXPECT_FALSE(default_opt.has_value());
  EXPECT_FALSE(static_cast<bool>(default_opt));
}

class OptionValueConstructorFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<int> int_opt{test_data.random_int};
  tmn::err::Option<double> double_opt{test_data.random_double};
  tmn::err::Option<std::string> string_opt{test_data.random_string};
};

TEST_F(OptionValueConstructorFixture, ValueConstructorWithRandomValues) {
  EXPECT_TRUE(int_opt.has_value());
  EXPECT_EQ(int_opt.value(), test_data.random_int);

  EXPECT_TRUE(double_opt.has_value());
  EXPECT_DOUBLE_EQ(double_opt.value(), test_data.random_double);

  EXPECT_TRUE(string_opt.has_value());
  EXPECT_EQ(string_opt.value(), test_data.random_string);
}

class OptionMoveConstructorFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<std::string> original_opt{test_data.random_string};
};

TEST_F(OptionMoveConstructorFixture, MoveConstructorWithRandomValue) {
  tmn::err::Option<std::string> moved_opt(std::move(original_opt));

  EXPECT_TRUE(moved_opt.has_value());
  EXPECT_EQ(moved_opt.value(), test_data.random_string);
  EXPECT_FALSE(original_opt.has_value());
}

class OptionCopyConstructorFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<int> original_opt{test_data.random_int};
};

TEST_F(OptionCopyConstructorFixture, CopyConstructorWithRandomValue) {
  tmn::err::Option<int> copied_opt(original_opt);

  EXPECT_TRUE(copied_opt.has_value());
  EXPECT_EQ(copied_opt.value(), test_data.random_int);
  EXPECT_TRUE(original_opt.has_value());
  EXPECT_EQ(original_opt.value(), test_data.random_int);
}

class OptionAssignmentOperatorsFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<int> source_opt{test_data.random_int};
  tmn::err::Option<int> target_opt;
};

TEST_F(OptionAssignmentOperatorsFixture, CopyAssignmentWithRandomValue) {
  target_opt = source_opt;

  EXPECT_TRUE(target_opt.has_value());
  EXPECT_EQ(target_opt.value(), test_data.random_int);
  EXPECT_TRUE(source_opt.has_value());
  EXPECT_EQ(source_opt.value(), test_data.random_int);
}

TEST_F(OptionAssignmentOperatorsFixture, MoveAssignmentWithRandomValue) {
  const int new_value = generate_random_val(1001, 2000);
  target_opt = tmn::err::Option<int>(new_value);

  EXPECT_TRUE(target_opt.has_value());
  EXPECT_EQ(target_opt.value(), new_value);
}

class OptionSwapFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  const int value1 = test_data.random_int;
  const int value2 = generate_random_val(1001, 2000);
  tmn::err::Option<int> opt1{value1};
  tmn::err::Option<int> opt2{value2};
};

TEST_F(OptionSwapFixture, SwapWithRandomValues) {
  std::swap(opt1, opt2);

  EXPECT_EQ(opt1.value(), value2);
  EXPECT_EQ(opt2.value(), value1);
}

class OptionValueOrFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  const int default_value = generate_random_val(2001, 3000);
  tmn::err::Option<int> filled_opt{test_data.random_int};
  tmn::err::Option<int> empty_opt;
};

TEST_F(OptionValueOrFixture, ValueOrWithRandomValues) {
  EXPECT_EQ(filled_opt.value_or(default_value), test_data.random_int);
  EXPECT_EQ(empty_opt.value_or(default_value), default_value);
}

TEST_F(OptionValueOrFixture, ValueOrDefaultWithRandomValue) {
  EXPECT_EQ(filled_opt.value_or_default(), test_data.random_int);
  EXPECT_EQ(empty_opt.value_or_default(), 0);
}

class OptionValueOrExceptionFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<int> filled_opt{test_data.random_int};
  tmn::err::Option<int> empty_opt;
};

TEST_F(OptionValueOrExceptionFixture, ValueAccessWithRandomValue) {
  EXPECT_EQ(filled_opt.value(), test_data.random_int);
}

TEST_F(OptionValueOrExceptionFixture, ValueAccessThrowsOnEmpty) {
  EXPECT_THROW(empty_opt.value(), std::bad_optional_access);
}

class OptionDestroyValueFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<std::string> string_opt{test_data.random_string};
};

TEST_F(OptionDestroyValueFixture, DestroyValueWithRandomString) {
  EXPECT_TRUE(string_opt.has_value());
  EXPECT_TRUE(string_opt.destroy_value());
  EXPECT_FALSE(string_opt.has_value());
  EXPECT_FALSE(string_opt.destroy_value());
}

class OptionMapFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<int> filled_opt{test_data.random_int};
  tmn::err::Option<int> empty_opt;
};

TEST_F(OptionMapFixture, MapWithRandomValue) {
  const auto doubled = filled_opt.fmap([](int x) { return x * 2; });

  EXPECT_TRUE(doubled.has_value());
  EXPECT_EQ(doubled.value(), test_data.random_int * 2);
}

TEST_F(OptionMapFixture, MapWithEmpty) {
  const auto r = empty_opt.fmap([](int x) { return x * 2; });
  EXPECT_FALSE(r.has_value());
}

class OptionAndThenFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  tmn::err::Option<int> int_opt{test_data.random_int};
  tmn::err::Option<std::string> string_opt{test_data.random_string};
  tmn::err::Option<int> empty_opt;
};

TEST_F(OptionAndThenFixture, AndThenWithRandomInt) {
  const auto r = int_opt.and_then([](int x) { return x * 2; });
  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), test_data.random_int * 2);
}

TEST_F(OptionAndThenFixture, AndThenWithEmpty) {
  const auto r = empty_opt.and_then([](int x) { return x * 2; });
  EXPECT_FALSE(r.has_value());
}

TEST_F(OptionAndThenFixture, AndThenWithStringLength) {
  const auto r = string_opt.and_then([](const std::string& s) { return s.length(); });
  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), test_data.random_string.length());
}

class OptionOrElseFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  const int fallback_value = generate_random_val(3001, 4000);
  tmn::err::Option<int> filled_opt{test_data.random_int};
  tmn::err::Option<int> empty_opt;
};

TEST_F(OptionOrElseFixture, OrElseWithRandomValue) {
  bool callback_called = false;
  const auto r = filled_opt.or_else([&]() {
    callback_called = true;
    return tmn::err::Option<int>(fallback_value);
  });

  EXPECT_TRUE(r.has_value());
  EXPECT_FALSE(callback_called);
}

TEST_F(OptionOrElseFixture, OrElseWithEmpty) {
  bool callback_called = false;
  const auto r = empty_opt.or_else([&]() {
    callback_called = true;
    return tmn::err::Option<int>(fallback_value);
  });

  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), fallback_value);
  EXPECT_TRUE(callback_called);
}

class OptionChainingFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  const int fallback_value = generate_random_val(4001, 5000);
};

TEST_F(OptionChainingFixture, AndThenChainWithRandomValue) {
  const auto r = tmn::err::Option<int>(test_data.random_int)
    .and_then([](int x) { return x * 2; })
    .and_then([](int x) { return x + 5; })
    .and_then([](int x) { return std::to_string(x); });

  const int expected_value = (test_data.random_int * 2) + 5;
  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), std::to_string(expected_value));
}

TEST_F(OptionChainingFixture, OrElseChain) {
  const auto r = tmn::err::Option<int>()
    .or_else([]() { return tmn::err::Option<int>(); })
    .or_else([this]() { return tmn::err::Option<int>(fallback_value); })
    .or_else([]() { return tmn::err::Option<int>(generate_random_val(5001, 6000)); });

  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), fallback_value);
}

TEST_F(OptionChainingFixture, MixedChainWithRandomValue) {
  const int initial_value = test_data.random_int;
  const auto r = tmn::err::Option<int>(initial_value)
    .and_then([](int x) { return x % 2 == 0 ? x : x * 2; })
    .or_else([this]() { return fallback_value; })
    .and_then([](int x) { return x * 3; });

  const int transformed_value = (initial_value % 2 == 0) ? initial_value : initial_value * 2;
  const int expected_value = transformed_value * 3;

  EXPECT_TRUE(r.has_value());
  EXPECT_EQ(r.value(), expected_value);
}

class OptionConversionFixture : public ::testing::Test {
protected:
  RandomTestData test_data;
  const std::string error_message = "error_" + std::to_string(test_data.random_int);
  tmn::err::Option<int> filled_opt{test_data.random_int};
  tmn::err::Option<int> empty_opt;
};

TEST_F(OptionConversionFixture, ToResultWithRandomValue) {
  const auto r = filled_opt.to_result(tmn::err::StrErr(error_message));

  EXPECT_TRUE(r.is_ok());
  EXPECT_EQ(r.value(), test_data.random_int);
}

TEST_F(OptionConversionFixture, ToResultWithEmpty) {
  const auto r = empty_opt.to_result(tmn::err::StrErr(error_message));

  EXPECT_TRUE(r.is_err());
  EXPECT_EQ(r.err().err_msg(), error_message);
}
