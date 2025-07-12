#include <iostream>
#include <string>

#include "../include/Either/Either.hpp"

struct EnglishLanguage {
    std::string message;
};

struct RussianLanguage {
    std::string message;
};

using LocalizedString = fpp::Either<EnglishLanguage, RussianLanguage>;

void greet(const LocalizedString& str) {
    if (str.is_left()){
        std::cout << "Hello, " << str.left_value_or_exception().message << "!\n";
    }
    else{
        std::cout << "Привет, " << str.right_value_or_exception().message << "!\n";
    }
}

int main() {
    LocalizedString enMsg = fpp::Either<EnglishLanguage, RussianLanguage>::from_left(
        EnglishLanguage{"User"}
    );

    LocalizedString ruMsg = fpp::Either<EnglishLanguage, RussianLanguage>::from_right(
        RussianLanguage{"Пользователь"}
    );

    greet(enMsg);
    greet(ruMsg);

    return 0;
}