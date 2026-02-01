#include <gtest/gtest.h>

#include <eav/Result.hpp>

using namespace eav;

// clang-format off
TEST(CombinatorTest, MapOkChain) {
    auto res = make::Ok(10)
        | combine::MapOk([](int x) { return x * 2; })
        | combine::MapOk([](int x) { return std::to_string(x); });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), "20");
}

TEST(CombinatorTest, AndThenSuccess) {
    auto res = make::Ok(10) | combine::AndThen([](int x) -> Result<std::string, int> { return make::Ok(std::to_string(x)); });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), "10");
}

TEST(CombinatorTest, AndThenFailure) {
    auto res = make::Ok(10)
        | combine::AndThen([](int) -> Result<std::string, int> { return make::Err(500); })
        | combine::MapOk([](const std::string&) { return 0; });

    EXPECT_TRUE(res.is_err());
    EXPECT_EQ(res.unwrap_err(), 500);
}

TEST(CombinatorTest, OrElseRecovery) {
    auto res = make::Err(404) | combine::OrElse([](int err) -> Result<int, std::string> {
            if (err == 404) return make::Ok(0);
            return make::Err(std::string("fatal"));
        });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 0);
}

TEST(CombinatorTest, ComplexChain) {
    auto res = make::Ok(1) | combine::MapOk([](int x) { return x + 9; })  // 10
               | combine::AndThen([](int x) { return make::Ok(x * 2); })  // 20
               | combine::AndThen([](int) {                               // err: "stop"
                     return Result<int, std::string>(make::Err(std::string("stop")));
                 })
               | combine::MapOk([](int x) { return x + 100; })            // skipped, reset err
               | combine::OrElse([](std::string s) {                      // work with err
                     return make::Ok(static_cast<int>(s.length()));       // res.val = 4 = len("stop");
                 });

    EXPECT_TRUE(res.is_ok());
    EXPECT_EQ(res.unwrap_ok(), 4);
}
