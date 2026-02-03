#include <gtest/gtest.h>

#include <string>

#include <eav/Option.hpp>

using namespace eav;

// clang-format off
TEST(OptionCombinatorTest, SomeMapChain) {
    auto res = make::Some(10)
        | combine::option::Map([](int x) { return x * 2; })
        | combine::option::Map([](int x) { return std::to_string(x); });

    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.unwrap(), "20");
}

TEST(OptionCombinatorTest, NoneMapChain) {
    auto res = make::None()
        | combine::option::Map([](int x) { return x * 2; });

    EXPECT_FALSE(res.has_value());
}

TEST(OptionCombinatorTest, SomeAndThenChain) {
    auto res = make::Some(10)
        | combine::option::AndThen([](int x) { return make::Some(std::to_string(x)); });

    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.unwrap(), "10");
}

TEST(OptionCombinatorTest, NoneAndThenChain) {
    auto res = make::None()
        | combine::option::AndThen([](int x) { return make::Some(x); });

    EXPECT_FALSE(res.has_value());
}

TEST(OptionCombinatorTest, SomeOrElseChain) {
    auto res = make::Some(42)
        | combine::option::OrElse([]() { return make::Some(0); });

    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.unwrap(), 42);
}

TEST(OptionCombinatorTest, NoneOrElseChain) {
    auto res = make::None()
        | combine::option::OrElse([]() { return make::Some(77); });

    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.unwrap(), 77);
}

TEST(OptionCombinatorTest, SomeFilterSuccessChain) {
    auto res = make::Some(10)
        | combine::option::Filter([](int x) { return x > 5; });

    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.unwrap(), 10);
}

TEST(OptionCombinatorTest, SomeFilterFailureChain) {
    auto res = make::Some(3)
        | combine::option::Filter([](int x) { return x > 5; });

    EXPECT_FALSE(res.has_value());
}

TEST(OptionCombinatorTest, ComplexOptionChain) {
    using namespace std::string_literals;

    auto res = make::Some(1)
        | combine::option::Map([](int x) { return x + 9; })         // 10
        | combine::option::Filter([](int x) { return x > 5; })      // still 10
        | combine::option::AndThen([](int x) { return make::None(); }) // None
        | combine::option::Map([](int x) { return x + 100; })      // skipped
        | combine::option::OrElse([]() { return make::Some(42); }); // 42

    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.unwrap(), 42);
}

// TEST(OptionCombinatorTest, ResultToOptionConversion) {
//     Result<int, std::string> r = make::Ok(100);

//     auto opt = std::move(r).erase_err()
//         | combine::option::Map([](int x) { return x / 2; });

//     EXPECT_TRUE(opt.has_value());
//     EXPECT_EQ(opt.unwrap(), 50);
// }
