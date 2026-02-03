#include <gtest/gtest.h>

#include <eav/Result.hpp>

using namespace eav;

// clang-format off
TEST(ResultCombinatorTest, OkMapOkChain) {
    auto res = make::Ok(10)
        | combine::result::MapOk([](int x) { return x * 2; })
        | combine::result::MapOk([](int x) { return std::to_string(x); });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), "20");
}

TEST(ResultCombinatorTest, ErrMapOkChain) {
    auto res = make::Err(10)
        | combine::result::MapOk([](int x) { return x * 2; });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), 10);
}

TEST(ResultCombinatorTest, OkAndThenChain) {
    auto res = make::Ok(10)
        | combine::result::AndThen([](int x) -> Result<std::string, int> { return make::Ok(std::to_string(x)); });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), "10");
}

TEST(ResultCombinatorTest, ErrAndThenChain) {
    auto res = make::Err(10)
        | combine::result::AndThen([](int) -> Result<std::string, int> { return make::Err(500); });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), 10);
}

TEST(ResultCombinatorTest, OkOrElseChain) {
    auto res = make::Ok(42)
        | combine::result::OrElse([](int err) -> Result<int, std::string> {
            if (err == 404) return make::Ok(0);
            return make::Err(std::string("fatal"));
        });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 42);
}

TEST(ResultCombinatorTest, ErrOrElseChain) {
    auto res = make::Err(404)
        | combine::result::OrElse([](int err) -> Result<int, std::string> {
            if (err == 404) return make::Ok(0);
            return make::Err(std::string("fatal"));
        });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 0);
}

TEST(ResultCombinatorTest, OkMapErrChain) {
    auto res = make::Ok(42)
        | combine::result::MapErr([](int code) { return std::to_string(code) + " Not Found"; });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 42);
}

TEST(ResultCombinatorTest, ErrMapErrChain) {
    auto res = make::Err(404)
        | combine::result::MapErr([](int code) { return std::to_string(code) + " Not Found"; });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), "404 Not Found");
}

TEST(ResultCombinatorTest, OkFilterSuccessChain) {
    auto res = make::Ok(10)
        | combine::result::AndThen([](int x) -> Result<int, std::string> { return make::Ok(x * 120); })
        | combine::result::Filter([](int x) -> bool { return x > 5; }, std::string("too small"));

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 1200);
}

TEST(ResultCombinatorTest, OkFilterFailureChain) {
    auto res = make::Ok(3)
        | combine::result::Filter([](int x) { return x > 5; }, std::string("too small"));

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), "too small");
}

TEST(ResultCombinatorTest, ErrFilterChain) {
    auto res = make::Err(std::string("original error"))
        | combine::result::Filter([](int x) { return x > 5; }, std::string("too small"));

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), "original error");
}

TEST(ResultCombinatorTest, SuperComplexChain) {
    using namespace std::string_literals;

    auto res = make::Ok(5)
        | combine::result::MapOk([](int x) { return x * 10; }) // 50
        | combine::result::Filter([](int x) { return x > 40; }, std::string("too small")) // skipped
        | combine::result::MapErr([](std::string s) { return std::string(s) + "!"; }) // skipped
        | combine::result::AndThen([](int x) -> Result<int, std::string> { return make::Ok(x + 5); }) // 55
        | combine::result::Filter([](int x) { return x < 50; }, "Triggered"s) // "Triggered"
        | combine::result::MapErr([](const std::string& s) { return s.length(); }) // 9
        | combine::result::OrElse([](size_t len) { return make::Ok(static_cast<int>(len)); }) // 9
        | combine::result::MapOk([](int x) { return x * 2; }); // 18

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 18);
}
