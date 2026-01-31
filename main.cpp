#include <iostream>
#include <string>

#include "src/Result/Combinators/Map.hpp"
#include "src/Result/Pipe.hpp"
#include "src/Result/Result.hpp"

using namespace eav;

struct AppErr {
    int code;
    std::string msg;
};

Result<int, AppErr> GetValue(bool success) {
    if (success) {
        return make::Ok(42);
    }
    return make::Err(AppErr{404, "Not Found"});
}

int main() {
    // clang-format off
    auto chain1 = GetValue(true)
        | combine::Map([](int v) { return v * 2; })
        | combine::Map([](int v) { return "Result is " + std::to_string(v); });

    if (chain1.is_ok()) {
        std::cout << std::move(chain1).unwrap_ok() << std::endl;
    }

    auto chain2 = GetValue(false)
        | combine::Map([](int v) {
                std::cout << "This will not be printed" << std::endl;
                return v + 1;
            })
        | combine::Map([](int v) {
                return std::to_string(v);
            });

    if (chain2.is_err()) {
        auto err = std::move(chain2).unwrap_err();
        std::cout << "Chain failed correctly with code: " << err.code
                  << " and msg: " << err.msg << std::endl;
    }

    return 0;
}
