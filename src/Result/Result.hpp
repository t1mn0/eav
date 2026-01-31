#pragma once

#include <concepts>
#include <string>

namespace eav {

// [TODO] : <typename E> -> <Error E> (Error should be concept)
template <typename T, typename E> requires(!std::is_void_v<T>)
class [[nodiscard]] Result {
  private:  // data members
    bool is_ok_;

    union {
        T ok_val_;
        E err_val_;
    };

  public:  // member functions:
    // Constructors and destructor:
    Result() = delete;  // value of the Result object must be explicitly initialized;
    Result(const Result<T, E>& oth);
    Result(Result<T, E>&& oth);
    ~Result();

    // Operators:
    Result<T, E>& operator=(const Result<T, E>& oth);
    Result<T, E>& operator=(Result<T, E>&& oth);
    operator bool() const noexcept;

    // Observers:
    bool is_ok() const noexcept;
    bool is_err() const noexcept;

    constexpr const T& unwrap_ok(std::string& msg) const&;
    constexpr const T& unwrap_ok(const std::string& msg = "") const&;
    constexpr const T& unwrap_ok(std::string&& msg = "") const&;

    constexpr T& unwrap_ok(std::string& msg) &;
    constexpr T& unwrap_ok(const std::string& msg = "") &;
    constexpr T& unwrap_ok(std::string&& msg = "") &;

    constexpr T unwrap_ok_or(T&& else_val) &;

    constexpr const E& unwrap_err(std::string& msg) const&;
    constexpr const E& unwrap_err(const std::string& msg = "") const&;
    constexpr const E& unwrap_err(std::string&& msg = "") const&;

    constexpr E& unwrap_err(std::string& msg) &;
    constexpr E& unwrap_err(const std::string& msg = "") &;
    constexpr E& unwrap_err(std::string&& msg = "") &;

    constexpr E unwrap_err_or(E&& else_val) &;

    // [TODO]: Option Conversion
    // Option<T> erase_err() &;

  private:  // member functions:
    // Constructors:
    Result(const T& val);
    Result(T&& val);
    Result(const E& val);
    Result(E&& val);
};

}  // namespace eav
