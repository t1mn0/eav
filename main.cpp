#include <string>
#include <iostream>

#include "include/Result/Result.hpp"

using namespace throwless;

Result<int, std::string> safe_divide(int a, int b) {
    if (b == 0) {
        return Result<int, std::string>::Err("Division by zero");
    }
    return Result<int, std::string>::Ok(a / b);
}

Result<int, std::string> parse_int(const std::string& s) {
    try {
        size_t pos;
        int num = std::stoi(s, &pos);
        if (pos != s.size()) {
            return Result<int, std::string>::Err("Invalid characters in string");
        }
        return Result<int, std::string>::Ok(num);
    } catch (const std::exception&) {
        return Result<int, std::string>::Err("Not a valid integer");
    }
}

int main(){
    auto result = safe_divide(10, 0);
    if (result.is_ok()) {
        std::cout << "Result: " << result.unwrap() << "\n";
    } 
    else {
        std::cerr << "Error: " << result.unwrap_err() << "\n";
    }

    auto num = parse_int("42");
    if (num.is_ok()) {
        std::cout << "Parsed: " << num.unwrap() << "\n";
    } 
    else {
        std::cerr << "Parse failed: " << num.unwrap_err() << "\n";
    }

    return 0;
}