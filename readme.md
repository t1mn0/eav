<h1 align="center"> Errors Are Values </h1>

__eav__ is a C++20 header-only library for error handling, inspired by the philosophies of Rust, Go, and functional programming.

The library provides a `Result<T, E>` & `Option<T>` types that help to process errors as values explicitly, without a hidden exception control flow, while maintaining code purity using pipeline syntax in a style typical of functional programming.

## Combinators
eav-lib provides ability to chain complex logic into a single, readable flow. With eav-lib, you compose logic using the `|` (pipe) operator.

### Key Combinators
#### For `Result<T,E>`:
- `MapOk`:	transforms the successful value;
- `AndThen`:	chains another operation that might fail (monadic `bind`);
- `Filter`:	validates a value and converts it to an error if criteria aren't met;
- `MapErr`:	converts error types;
- `OrElse`:	recovers from an error or provides a fallback value;

#### For `Option<T>`:

## Example
```cpp
#include <eav/Result.hpp>
#include <iostream>
#include <string>

using namespace eav;

auto get_data(int id) {
    if (id > 0) return make::Ok(id * 2);
    return make::Err("invalid id");
}

int main() {
    auto res = get_data(10)
        | combine::MapOk([](int n) { return n + 1; })
        | combine::Filter([](int n) { return n < 100; }, "too big")
        | combine::OrElse([](auto err) { return make::Ok(0); });

    if (res) {
        std::cout << "Result: " << res.unwrap_ok() << std::endl;
    }
}
```

## Install
Since the __eav__ is header-only, you can simply copy the eav folder to your project or use CMake's FetchContent:
```cmake
include(FetchContent)
FetchContent_Declare(
  eav
  GIT_REPOSITORY https://github.com/t1mn0/eav.git
  GIT_TAG main
)
FetchContent_MakeAvailable(eav)

target_link_libraries(your_project PRIVATE eav)
```

## Refs
Philosophy and design principles:
- [Joe Duffy](https://joeduffyblog.com/2016/02/07/the-error-model/)
- [Go Proverbs](https://go-proverbs.github.io/)
  
Combinators and functional pipeline ideas:
- [Roman Lipovsky](https://gitlab.com/Lipovsky/concurrency-course/-/tree/master/tasks/future/lazy)
- [Rust Combinators](https://learning-rust.github.io/docs/combinators/)
