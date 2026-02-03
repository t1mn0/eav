#pragma once

#include <string_view>
#include <type_traits>

#include "Detail/Pending.hpp"
#include "Option/Detail/Tags.hpp"
#include "Option/FwdDecl/None.hpp"
#include "Option/FwdDecl/Some.hpp"

namespace eav {

template <typename T> requires(!std::is_void_v<T>)
class [[nodiscard]] Option {
  public:  // nested types:
    using OkType = T;
    using ErrType = void;

  private:  // data members:
    alignas(T) char storage_[sizeof(T)];
    bool has_value_ = false;

  public:  // member functions:
    // Constructors and destructor:
    Option() = delete;
    Option(const Option& oth);
    Option(Option&& oth) noexcept(std::is_nothrow_move_constructible_v<T>);

    // Next constructor for Option without inferenced type (None)
    template <typename U> requires(std::same_as<U, detail::PendingType>)
    Option(Option<U>&& oth);

    ~Option();

    // Operators:
    Option& operator=(const Option& oth);
    Option& operator=(Option&& oth) noexcept(std::is_nothrow_move_assignable_v<T>);

    // Observers:
    bool has_value() const noexcept;
    operator bool() const noexcept;

    // Accessors:
    const T* ptr() const;
    T* ptr();

    constexpr const T& unwrap(std::string_view msg = "called .unwrap() on None") const&;
    constexpr T& unwrap(std::string_view msg = "called .unwrap() on None") &;
    constexpr T unwrap(std::string_view msg = "called .unwrap() on None") &&;

    constexpr T unwrap_or(T&& else_val) const&;

    template <typename U> requires std::same_as<T, detail::PendingType>
    constexpr U unwrap_or(U&& else_val) const&;

  private:  // member functions:
    // Private constructors that are called by friend functions Some(...), None();
    template <typename U> requires std::constructible_from<T, U>
    Option(detail::SomeTag, U&& val);

    Option(detail::NoneTag);

  private:  // friends declaration:
    template <typename U>
    friend Option<std::decay_t<U>> make::Some(U&&);

    friend inline Option<detail::PendingType> make::None();

    template <typename U> requires(!std::is_void_v<U>)
    friend class Option;
};

}  // namespace eav

// for including 'Option' "module" via '#include <eav/Option.hpp>':
#include "Detail/Pipe.hpp"
#include "Option/Combinators/AndThen.hpp"
#include "Option/Combinators/Filter.hpp"
#include "Option/Combinators/Map.hpp"
#include "Option/Combinators/OrElse.hpp"
#include "Option/Detail/OptionImpl.hpp"
#include "Option/Make.hpp"
