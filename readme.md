<h1 align="center"> Errors Are Values </h1>

<p align="center">
  <img src="https://img.shields.io/github/actions/workflow/status/t1mn0/eav/ci.yml?branch=main&style=flat-square" alt="CI">
  <img src="https://img.shields.io/badge/C%2B%2B-20-blue.svg?style=flat-square&logo=cplusplus" alt="C++20">
  <img src="https://img.shields.io/github/license/t1mn0/eav?style=flat-square" alt="License">
</p>

__eav__ is a C++20 header-only library for error handling, inspired by the philosophies of Rust, Go, and functional programming.

The library provides a `Result<T, E>` & `Option<T>` types that help to process errors as values explicitly, without a hidden exception control flow, while maintaining code purity using pipeline syntax in a style typical of functional programming.

## Combinators
eav-lib provides ability to chain complex logic into a single, readable flow. With eav-lib, you compose logic using the `|` (pipe) operator.

### Key Combinators
#### For `Result<T,E>`:
- `MapOk`: transforms the successful value;
- `AndThen`: chains another operation that might fail (monadic `bind`);
- `Filter`:	validates a value and converts it to an error if criteria aren't met;
- `MapErr`:	converts error types;
- `OrElse`:	recovers from an error or provides a fallback value;

#### For `Option<T>`:
- `Map`: transforms the value if present;
- `AndThen`: chains another operation returning an `Option`;
- `Filter`: keeps the value only if it satisfies a predicate;
- `OrElse`: provides a fallback `Option` if the current one is `None`;

## Examples
- [Result tests](test/Result/Func.cpp)
- [Option tests](test/Option/Func.cpp)

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
- [Joe Duffy - The Error Model](https://joeduffyblog.com/2016/02/07/the-error-model/)
- [Go Proverbs - Erros are values](https://go-proverbs.github.io/)
  
Combinators and functional pipeline ideas:
- [Roman Lipovsky - Lazy Futures](https://gitlab.com/Lipovsky/concurrency-course/-/tree/master/tasks/future/lazy)
- [Rust Combinators Guide](https://learning-rust.github.io/docs/combinators/)
