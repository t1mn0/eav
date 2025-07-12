#include <iostream>
#include <regex>

#include "../include/Error/Error.hpp"
#include "../include/Result/Result.hpp"

// these are only simulating examples, which omit many functional subtleties.
// it demonstrate the using of fpp libraries in "everyday" code.

struct ValidationError {
    std::string field;
    std::string reason;
    
    std::string err_message() const noexcept {
        return "Validation failed for '" + field + "': " + reason;
    }
    
    bool operator==(const ValidationError& oth) const noexcept {
        return field == oth.field && reason == oth.reason;
    }
};

fpp::Result<std::string, ValidationError> validate_email(const std::string& email) {
    static const std::regex pattern(R"(\w+@\w+\.\w+)");
    if (!std::regex_match(email, pattern)) {
        return fpp::Result<std::string, ValidationError>::Err(ValidationError{"email", "invalid format"});
    }
    return fpp::Result<std::string, ValidationError>::Ok(email);
}

fpp::Result<int, ValidationError> validate_age(int age) {
    if (age < 18) {
        return fpp::Result<int, ValidationError>::Err(ValidationError{"age", "must be at least 18"});
    }
    return fpp::Result<int, ValidationError>::Ok(age);
}

fpp::Result<bool, ValidationError> register_user(const std::string& email, int age) {
    const auto valid_email = validate_email(email);
    if (valid_email.is_err()) return fpp::Result<bool, ValidationError>::Err(valid_email.unwrap_err_or_exception());
    
    const auto valid_age = validate_age(age);
    if (valid_age.is_err()) return fpp::Result<bool, ValidationError>::Err(valid_age.unwrap_err_or_exception());
    
    const auto reg_result = fpp::try_or_convert([&]() {
        if (email == "admin@example.com") {
            throw std::runtime_error("Admin user already exists");
        }
        return true;
    });
    
    if (reg_result.is_err()) return fpp::Result<bool, ValidationError>::Err(
        ValidationError{"email", reg_result.unwrap_err_or_exception().err_message()});
    
    std::cout << "User registered successfully!\n";
    return fpp::Result<bool, ValidationError>::Ok(true);
}

int main() {
    const auto result = register_user("test@example.com", 16); // try to change some field and check output
    
    if (result.is_err()) {
        std::cerr << "Registration failed:\n" << result.unwrap_err_or_exception().err_message() << "\n";
        return 1;
    }
    
    return 0;
}