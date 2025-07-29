#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include "../../include/Error/Error.hpp"

TEST(ErrorConceptTest, ConceptSatisfaction) {
    static_assert(fpp::Error<fpp::StringError>, "StringError should satisfy Error concept");
    static_assert(fpp::Error<fpp::CodeError>, "CodeError should satisfy Error concept");
    static_assert(fpp::Error<fpp::ExceptionError>, "ExceptionError should satisfy Error concept");
}

TEST(ExceptionErrorTest, BasicFunctionality) {
    struct TestException : std::exception {
        const char* what() const noexcept override { return "Test exception"; }
    };
    
    TestException ex;
    fpp::ExceptionError err1(ex);
    
    EXPECT_TRUE(err1.err_message().find("TestException") != std::string::npos);
    EXPECT_TRUE(err1.err_message().find("Test exception") != std::string::npos);
    
    fpp::ExceptionError err2;
    EXPECT_EQ(err2.err_message(), "[unknown]: Unknown exception");
    
    TestException ex2;
    fpp::ExceptionError err3(ex2);
    EXPECT_EQ(err1, err3);
    EXPECT_NE(err1, err2);
}

TEST(TryOrConvertTest, BasicFunctionality) {
    auto success_fn = []() -> int { return 42; };
    auto success_result = fpp::try_or_convert(success_fn);
    EXPECT_TRUE(success_result.is_ok());
    EXPECT_EQ(success_result.unwrap_val(), 42);
    
    auto throwing_fn = []() -> int { throw std::runtime_error("Test error"); };
    auto error_result = fpp::try_or_convert(throwing_fn);
    EXPECT_TRUE(error_result.is_err());
    EXPECT_TRUE(error_result.unwrap_err().err_message().find("runtime_error") != std::string::npos);
    EXPECT_TRUE(error_result.unwrap_err().err_message().find("Test error") != std::string::npos);
    
    auto unknown_throwing_fn = []() -> int { throw 42; };
    auto unknown_error_result = fpp::try_or_convert(unknown_throwing_fn);
    EXPECT_TRUE(unknown_error_result.is_err());
    EXPECT_EQ(unknown_error_result.unwrap_err().err_message(), "[unknown]: Unknown exception");
}