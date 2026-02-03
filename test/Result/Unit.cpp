#include <gtest/gtest.h>

#include "TestUtils.hpp"

TEST(ResultTest, CreationOk) {
    Result<int, ErrorCode> r = make::Ok(42);

    EXPECT_TRUE(r.is_ok());
    EXPECT_FALSE(r.is_err());
    EXPECT_TRUE(static_cast<bool>(r));
    EXPECT_EQ(r.unwrap_ok(), 42);
}

TEST(ResultTest, CreationErr) {
    Result<int, ErrorCode> r = make::Err(ErrorCode{404, "Not Found"});

    EXPECT_FALSE(r.is_ok());
    EXPECT_TRUE(r.is_err());
    EXPECT_FALSE(static_cast<bool>(r));
    EXPECT_EQ(r.unwrap_err().code, 404);
}

TEST(ResultTest, UnwrapOkThrowsOnErr) {
    Result<int, std::string> r = make::Err(std::string("error"));
    EXPECT_THROW(r.unwrap_ok("panic!"), std::runtime_error);

    try {
        r.unwrap_ok("panic!");
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "panic!");
    }
}

TEST(ResultTest, UnwrapErrThrowsOnOk) {
    Result<int, std::string> r = make::Ok(44);
    EXPECT_THROW(r.unwrap_err(), std::runtime_error);
}

TEST(ResultTest, UnwrapOkOr) {
    Result<int, int> r_ok = make::Ok(44);
    Result<int, int> r_err = make::Err(404);

    EXPECT_EQ(r_ok.unwrap_ok_or(0), 44);
    EXPECT_EQ(r_err.unwrap_ok_or(11), 11);
}

TEST(ResultTest, MoveOnlyTypeSupport) {
    auto ptr = std::make_unique<int>(100);
    Result<std::unique_ptr<int>, int> r = make::Ok(std::move(ptr));

    EXPECT_TRUE(r.is_ok());
    auto extracted = std::move(r).unwrap_ok();
    EXPECT_EQ(*extracted, 100);
}

TEST(ResultTest, ResultToOptionAndMapConversion) {
    Result<int, std::string> r = make::Ok(100);
    auto opt = std::move(r).erase_err() | combine::option::Map([](int x) { return x / 2; });
    EXPECT_TRUE(opt.has_value());
    EXPECT_EQ(opt.unwrap(), 50);
}
