#include <iostream>
#include <string>

#include "Option/Option.hpp"

tmn::err::Option<std::string> find_user_name(int user_id) {
  if (user_id == 1) return std::string("Alice");
  if (user_id == 2) return std::string("Bob");
  return tmn::err::None<std::string>();
}

int main() {
  auto name = find_user_name(3)
    .fmap([](auto s) { return "Hello, " + s + "!"; })
    .or_else([&]() {
      return std::string("User not found");
    });

  std::cout << name.value() << std::endl;
}
