#include <gtest/gtest.h>

#include <eav/Result.hpp>

using namespace eav;

// clang-format off
TEST(CombinatorTest, OkMapOkChain) {
    auto res = make::Ok(10)
        | combine::MapOk([](int x) { return x * 2; })
        | combine::MapOk([](int x) { return std::to_string(x); });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), "20");
}

TEST(CombinatorTest, ErrMapOkChain) {
    auto res = make::Err(10)
        | combine::MapOk([](int x) { return x * 2; });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), 10);
}

TEST(CombinatorTest, OkAndThenChain) {
    auto res = make::Ok(10)
        | combine::AndThen([](int x) -> Result<std::string, int> { return make::Ok(std::to_string(x)); });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), "10");
}

TEST(CombinatorTest, ErrAndThenChain) {
    auto res = make::Err(10)
        | combine::AndThen([](int) -> Result<std::string, int> { return make::Err(500); });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), 10);
}

TEST(CombinatorTest, OkOrElseChain) {
    auto res = make::Ok(42)
        | combine::OrElse([](int err) -> Result<int, std::string> {
            if (err == 404) return make::Ok(0);
            return make::Err(std::string("fatal"));
        });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 42);
}

TEST(CombinatorTest, ErrOrElseChain) {
    auto res = make::Err(404)
        | combine::OrElse([](int err) -> Result<int, std::string> {
            if (err == 404) return make::Ok(0);
            return make::Err(std::string("fatal"));
        });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 0);
}

TEST(ResultFunc, OkMapErrChain) {
    auto res = make::Ok(42)
        | combine::MapErr([](int code) { return std::to_string(code) + " Not Found"; });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 42);
}

TEST(ResultFunc, ErrMapErrChain) {
    auto res = make::Err(404)
        | combine::MapErr([](int code) { return std::to_string(code) + " Not Found"; });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), "404 Not Found");
}

TEST(ResultFunc, OkFilterSuccessChain) {
    auto res = make::Ok(10)
        | combine::AndThen([](int x) -> Result<int, std::string> { return make::Ok(x * 120); })
        | combine::Filter([](int x) -> bool { return x > 5; }, std::string("too small"));

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 1200);
}

TEST(ResultFunc, OkFilterFailureChain) {
    auto res = make::Ok(3)
        | combine::Filter([](int x) { return x > 5; }, std::string("too small"));

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), "too small");
}

TEST(ResultFunc, ErrFilterChain) {
    auto res = make::Err(std::string("original error"))
        | combine::Filter([](int x) { return x > 5; }, std::string("too small"));

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), "original error");
}

TEST(CombinatorTest, SuperComplexChain) {
    using namespace std::string_literals;

    auto res = make::Ok(5)
        | combine::MapOk([](int x) { return x * 10; }) // 50
        | combine::Filter([](int x) { return x > 40; }, std::string("too small")) // skipped
        | combine::MapErr([](std::string s) { return std::string(s) + "!"; }) // skipped
        | combine::AndThen([](int x) -> Result<int, std::string> { return make::Ok(x + 5); }) // 55
        | combine::Filter([](int x) { return x < 50; }, "Triggered"s) // "Triggered"
        | combine::MapErr([](const std::string& s) { return s.length(); }) // 9
        | combine::OrElse([](size_t len) { return make::Ok(static_cast<int>(len)); }) // 9
        | combine::MapOk([](int x) { return x * 2; }); // 18

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 18);
}
