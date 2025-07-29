#include <iostream>
#include <string>

#include "../include/Either/Either.hpp"

// these are only simulating examples, which omit many functional subtleties.
// it demonstrate the using of fpp libraries in "everyday" code.

struct EnglishLanguage {
    std::string message;
};

struct RussianLanguage {
    std::string message;
};

using LocalizedString = fpp::Either<EnglishLanguage, RussianLanguage>;

void greet(const LocalizedString& str) {
    if (str.is_left()){
        std::cout << "Hello, " << str.left_value().message << "!\n";
    }
    else{
        std::cout << "Привет, " << str.right_value().message << "!\n";
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