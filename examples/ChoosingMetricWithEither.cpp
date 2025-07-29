#include <iostream>

#include "../include/Either/Either.hpp"

// these are only simulating examples, which omit many functional subtleties.
// it demonstrate the using of fpp libraries in "everyday" code.

struct Meters {
    double value;
};

struct Feet {
    double value;
};

using Length = fpp::Either<Meters, Feet>;

void printLength(const Length& len) {
    if (len.is_left()){
        std::cout << "Length: " << len.left_value().value << " meters\n";
    }
    else {
        std::cout << "Length: " << len.right_value().value << " feet\n";
    }
}

int main() {
    Length meterLength = fpp::Either<Meters, Feet>::from_left(
        Meters{5.0}
    );

    Length feetLength = fpp::Either<Meters, Feet>::from_right(
        Feet{16.404}
    );

    printLength(meterLength);
    printLength(feetLength);

    return 0;
}