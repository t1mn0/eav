#include <gtest/gtest.h>

#include <eav/Option.hpp>

using namespace eav;

TEST(OptionTest, CreationSome) {
    Option<int> o = make::Some(42);

    EXPECT_TRUE(o.has_value());
    EXPECT_TRUE(static_cast<bool>(o));
    EXPECT_EQ(o.unwrap(), 42);
}

TEST(OptionTest, CreationNone) {
    Option<int> o = make::None();

    EXPECT_FALSE(o.has_value());
    EXPECT_FALSE(static_cast<bool>(o));
}

TEST(OptionTest, UnwrapThrowsOnNone) {
    Option<int> o = make::None();
    EXPECT_THROW(o.unwrap("panic!"), std::runtime_error);

    try {
        o.unwrap("panic!!");
    } catch (const std::runtime_error& e) {
        EXPECT_STREQ(e.what(), "panic!!");
    }
}

TEST(OptionTest, UnwrapOr) {
    Option<int> o_some = make::Some(44);
    Option<int> o_none = make::None();

    EXPECT_EQ(o_some.unwrap_or(0), 44);
    EXPECT_EQ(o_none.unwrap_or(11), 11);
}

TEST(OptionTest, MoveOnlyTypeSupport) {
    auto ptr = std::make_unique<int>(100);
    Option<std::unique_ptr<int>> o = make::Some(std::move(ptr));

    EXPECT_TRUE(o.has_value());
    auto extracted = std::move(o.unwrap());
    EXPECT_EQ(*extracted, 100);
}
