#pragma once

#include <eav/Result.hpp>

#include <string>

using namespace eav;

struct ErrorCode {
    int code;
    std::string msg;

    bool operator==(const ErrorCode& oth) const {
        return code == oth.code && msg == oth.msg;
    }
};

static_assert(concepts::IsError<ErrorCode>);
