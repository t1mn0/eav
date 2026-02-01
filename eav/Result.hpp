#pragma once

#include <string_view>
#include <type_traits>  // std::is_void_v
#include <variant>

#include "Result/Concepts/IsError.hpp"

namespace eav {

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
class Result;

namespace make {

template <typename T> Result<T, detail::PendingType> Ok(T&& val);
template <concepts::IsError E> Result<detail::PendingType, E> Err(E&& val);

}  // namespace make

}  // namespace eav

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

    template <typename U, typename R>
    requires(
        (std::same_as<U, T> || std::same_as<U, detail::PendingType>) &&
        (std::same_as<R, E> || std::same_as<R, detail::PendingType>) &&
        !(std::same_as<U, T> && std::same_as<R, E>))
    Result(Result<U, R>&& oth);

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

  private:  // member functions:
    Result(detail::OkTag, T&& val);
    Result(detail::ErrTag, E&& val);

  private:  // friends:
    template <typename U>
    friend Result<U, detail::PendingType> make::Ok(U&&);

    template <concepts::IsError R>
    friend Result<detail::PendingType, R> make::Err(R&&);
};

}  // namespace eav

#include "Result/Combinators/AndThen.hpp"
#include "Result/Combinators/MapOk.hpp"
#include "Result/Combinators/OrElse.hpp"
#include "Result/Detail/ResultImpl.hpp"
#include "Result/Make.hpp"
#include "Result/Pipe.hpp"
