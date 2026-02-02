#pragma once

#include <string_view>
#include <variant>

#include "Result/Concepts/IsError.hpp"
#include "Result/Concepts/IsResult.hpp"
#include "Result/Detail/Tags.hpp"
#include "Result/FwdDecl/Err.hpp"
#include "Result/FwdDecl/Ok.hpp"
#include "Result/FwdDecl/Result.hpp"

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

    // Next constructor
    // NOT SUPPORT:
    //      arg: Result<U=T, R=E>&& <- this constraint doesnt overlap default move constructor
    // SUPPORT:
    // 1.   arg: Result<U=PendingType, R=E>&&           => Result<T,E>
    // 2.   arg: Result<U=T,           R=PendingType>&& => Result<T,E>
    // 3.   arg: Result<U=PendingType, R=PendingType>&& => Result<T,E>
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

    // [TODO]: Option conversion
    // Option<T> erase_err() &;

  private:  // member functions:
    // Private constructors that are called by friend functions Ok(...), Err(...);
    // Argument Tag is used for the compiler to recognize a potentially ambiguous call when E=T (Result<T,T>)
    Result(detail::OkTag, T&& val);
    Result(detail::ErrTag, E&& val);

  private:  // friends declaration:
    template <typename U>
    friend Result<U, detail::PendingType> make::Ok(U&&);

    template <concepts::IsError R>
    friend Result<detail::PendingType, R> make::Err(R&&);
};

}  // namespace eav

// for including 'Result' "module" via '#include <eav/Result.hpp>':
#include "Result/Combinators/AndThen.hpp"
#include "Result/Combinators/Filter.hpp"
#include "Result/Combinators/MapErr.hpp"
#include "Result/Combinators/MapOk.hpp"
#include "Result/Combinators/OrElse.hpp"
#include "Result/Detail/ResultImpl.hpp"
#include "Result/Make.hpp"
#include "Result/Pipe.hpp"
