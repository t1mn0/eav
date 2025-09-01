#include <gtest/gtest.h>

#include "../../Error/Error.hpp"

class ExceptionErrFixture : public ::testing::Test {
protected: //* structs and fields:
  // Creating an internal hidden struct and fields for testing the ExceptionErr module:
  struct TestException : std::exception {
    const char* what() const noexcept override { return "Test exception"; }
  };

  tmn::err::ExceptionErr excerr1;
  tmn::err::ExceptionErr excerr2;
  tmn::err::ExceptionErr excerr3;
  tmn::err::ExceptionErr excerr4;

protected: //* methods:
  void SetUp() override {
    TestException exc;
    excerr1 = tmn::err::ExceptionErr(exc);
    excerr2 = tmn::err::ExceptionErr(); // default constructor;

    TestException exc2;
    excerr3 = tmn::err::ExceptionErr(exc2);
    excerr4 = tmn::err::ExceptionErr(std::runtime_error("Different error"));
  }
};

TEST_F(ExceptionErrFixture, BasicFunctionality) {
  EXPECT_TRUE(excerr1.err_msg().find("TestException") != std::string::npos);
  EXPECT_TRUE(excerr1.err_msg().find("Test exception") != std::string::npos);
  EXPECT_EQ(excerr2.err_msg(), "[Unknown type]: Unknown error");

  EXPECT_EQ(excerr1, excerr3) << "ExceptionErrors must be equal, because they are created from the same exceptions";
  EXPECT_NE(excerr1, excerr4) << "They should not be equal, because they are created from different exceptions";
}
