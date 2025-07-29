#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../include/Option/Option.hpp"

TEST(OptionArithmeticTest, Addition) {
    fpp::Option<int> a(10);
    fpp::Option<int> b(20);
    auto c = a + b;
    EXPECT_TRUE(c.has_value());
    EXPECT_EQ(c.value(), 30);

    fpp::Option<double> d(1.5);
    fpp::Option<double> e(2.5);
    auto f = d + e;
    EXPECT_TRUE(f.has_value());
    EXPECT_DOUBLE_EQ(f.value(), 4.0);
}

TEST(OptionArithmeticTest, Subtraction) {
    fpp::Option<int> a(30);
    fpp::Option<int> b(10);
    auto c = a - b;
    EXPECT_TRUE(c.has_value());
    EXPECT_EQ(c.value(), 20);
}

TEST(OptionArithmeticTest, Multiplication) {
    fpp::Option<int> a(5);
    fpp::Option<int> b(6);
    auto c = a * b;
    EXPECT_TRUE(c.has_value());
    EXPECT_EQ(c.value(), 30);
}

TEST(OptionArithmeticTest, Division) {
    fpp::Option<int> a(20);
    fpp::Option<int> b(5);
    auto c = a / b;
    EXPECT_TRUE(c.has_value());
    EXPECT_EQ(c.value(), 4);

    fpp::Option<int> zero(0);
    auto d = a / zero;
    EXPECT_FALSE(d.has_value());
}

TEST(OptionArithmeticTest, CompoundOperators) {
    fpp::Option<int> a(10);
    fpp::Option<int> b(5);
    
    a += b;
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 15);
    
    a -= b;
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 10);
    
    a *= b;
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 50);
    
    a /= b;
    EXPECT_TRUE(a.has_value());
    EXPECT_EQ(a.value(), 10);
}

TEST(OptionArithmeticTest, MixedTypes) {
    fpp::Option<int> a(10);
    fpp::Option<double> b(2.5);
    
    auto c = a * b;
    static_assert(std::is_same_v<decltype(c), fpp::Option<double>>);
    EXPECT_TRUE(c.has_value());
    EXPECT_DOUBLE_EQ(c.value(), 25.0);
}

TEST(OptionArithmeticTest, StringConcatenation) {
    fpp::Option<std::string> a("Hello");
    fpp::Option<std::string> b(" World");
    
    auto c = a + b;
    EXPECT_TRUE(c.has_value());
    EXPECT_EQ(c.value(), "Hello World");
}

TEST(OptionArithmeticTest, EmptyOperands) {
    fpp::Option<int> a(10);
    fpp::Option<int> empty;
    
    auto b = a + empty;
    EXPECT_FALSE(b.has_value());
    
    auto c = empty - a;
    EXPECT_FALSE(c.has_value());
    
    a += empty;
    EXPECT_FALSE(a.has_value());
}