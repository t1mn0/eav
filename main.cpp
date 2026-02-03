#include <iostream>
#include <string>
#include <vector>

#include <eav/Option.hpp>

using namespace eav;

// clang-format off:
Option<int> get_index(const std::string& name) {
    if (name == "Admin") return make::Some(0);
    if (name == "User") return make::Some(1);
    return make::None();  // Option<PendingType>
}

Option<std::string> get_role_name(int index) {
    static const std::vector<std::string> roles = {std::string("Superuser"), std::string("Regular")};
    if (index >= 0 && index < roles.size()) {
        return make::Some(roles[index]);
    }
    return make::None();
}

int main() {
    {
        auto result = make::Some(std::string("Admin"))
            | combine::option::AndThen([](const std::string& name) { return get_index(name); })
            | combine::option::AndThen([](int idx) { return get_role_name(idx); })
            | combine::option::Map([](const std::string& role) { return "[" + role + "]"; });

        if (result.has_value()) {
            std::cout << "Test 1 Success: " << result.unwrap() << std::endl;  // [Superuser]
        } else {
            std::cerr << "Test 1 Failed: Expected value, got None" << std::endl;
        }
    }

    {
        Option<double> final_opt = make::None()
            | combine::option::Map([](int x) { return x + 1; })
            | combine::option::AndThen([](int x) { return make::Some(double(x)); });

        if (!final_opt.has_value()) {
            std::cout << "Test 3 Success: Lazy None bypassed and converted to Option<double>" << std::endl;
        }
    }

    {
        auto opt = make::None();
        std::string val = std::move(opt).unwrap_or(std::string("Fallback"));

        std::cout << "Test 4 Fallback: " << val << std::endl;
    }

    return 0;
}
