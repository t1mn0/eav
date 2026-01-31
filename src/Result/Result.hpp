#pragma once

#include <string_view>
#include <type_traits>  // std::is_void_v
#include <variant>

#include "Concepts/IsError.hpp"
#include "Make.hpp"

namespace eav {

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
class [[nodiscard]] Result {
  public:  // nested types:
    using Alternative = std::variant<T, E>;
    using OkType = T;
    using ErrType = E;

  private:  // data members:
    Alternative value_;

  public:  // member functions:
    // Constructors and destructor:
    Result() = delete;  // value of the Result object must be explicitly initialized;
    Result(const Result<T, E>&) = default;
    Result(Result<T, E>&&) = default;

    template <typename U>
    Result(detail::OkHolder<U>&& ok);

    template <typename U>
    Result(detail::ErrHolder<U>&& err);

    ~Result() = default;

    // Operators:
    Result<T, E>& operator=(const Result<T, E>& oth) = default;
    Result<T, E>& operator=(Result<T, E>&& oth) = default;
    operator bool() const noexcept;

    // Observers:
    bool is_ok() const noexcept;
    bool is_err() const noexcept;

    // Accessors:
    constexpr const T& unwrap_ok(std::string_view msg = "called .unwrap_ok() on Err") const&;
    constexpr T& unwrap_ok(std::string_view msg = "called .unwrap_ok() on Err") &;
    constexpr T unwrap_ok(std::string_view msg = "called .unwrap_ok() on Err") &&;

    template <typename U>
    constexpr T unwrap_ok_or(U&& else_val) const&;

    template <typename U>
    constexpr T unwrap_ok_or(U&& else_val) &&;

    constexpr const E& unwrap_err(std::string_view msg = "called .unwrap_ok() on Ok") const&;
    constexpr E& unwrap_err(std::string_view msg = "called .unwrap_ok() on Ok") &;
    constexpr E unwrap_err(std::string_view msg = "called .unwrap_ok() on Ok") &&;

    // [TODO]: Option Conversion
    // Option<T> erase_err() &;
};

}  // namespace eav

#include "Impl/Result.hpp"
