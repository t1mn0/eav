#ifndef FPP_OPTION_HPP
#error "Include Option.hpp instead of CoproductOperation.tpp"
#endif

#include "Option.hpp"

#include <utility> // for: std::declval;
#include <type_traits> // for: is_integral_v;

namespace fpp {

// arithmetic operations (free funcs) that support coproduct for the Option<T> monad
template <typename T, typename U>
requires fpp::Addable<T, U>
auto operator+(const Option<T>& lhs, const Option<U>& rhs)  
    -> Option<decltype(std::declval<T>() + std::declval<U>())>
{
    if (lhs.has_value() && rhs.has_value()) {
        return Option<decltype(std::declval<T>() + std::declval<U>())>(
            lhs.value_or_exception() + rhs.value_or_exception()
        );
    }
    return Option<decltype(std::declval<T>() + std::declval<U>())>();
}

template <typename T, typename U>
requires fpp::Subtractable<T, U>
auto operator-(const Option<T>& lhs, const Option<U>& rhs)
    -> Option<decltype(std::declval<T>() - std::declval<U>())>
{
    if (lhs.has_value() && rhs.has_value()) {
        return Option<decltype(std::declval<T>() - std::declval<U>())>(
            lhs.value_or_exception() - rhs.value_or_exception()
        );
    }
    return Option<decltype(std::declval<T>() - std::declval<U>())>();
}

template <typename T, typename U>
requires fpp::Multipliable<T, U>
auto operator*(const Option<T>& lhs, const Option<U>& rhs)
    -> Option<decltype(std::declval<T>() * std::declval<U>())>
{
    if (lhs.has_value() && rhs.has_value()) {
        return Option<decltype(std::declval<T>() * std::declval<U>())>(
            lhs.value_or_exception() * rhs.value_or_exception()
        );
    }
    return Option<decltype(std::declval<T>() * std::declval<U>())>();
}

template <typename T, typename U>
requires fpp::Dividable<T, U>
auto operator/(const Option<T>& lhs, const Option<U>& rhs)
    -> Option<decltype(std::declval<T>() / std::declval<U>())>
{
    if (lhs.has_value() && rhs.has_value()) {
        if constexpr (std::is_integral_v<T> && std::is_integral_v<U>) {
            if (rhs.value_or_exception() == 0) return Option<decltype(std::declval<T>() / std::declval<U>())>();
        }
        return Option<decltype(std::declval<T>() / std::declval<U>())>(
            lhs.value_or_exception() / rhs.value_or_exception()
        );
    }
    return Option<decltype(std::declval<T>() / std::declval<U>())>();
}

}