#pragma once

#include <utility>  // std::forward

#include "Concepts/IsError.hpp"

namespace eav::detail {

template <typename T>
struct OkHolder {
    T val_;
};

template <concepts::IsError E>
struct ErrHolder {
    E val_;
};

}  // namespace eav::detail

namespace eav::make::result {

template <typename T>
detail::OkHolder<T> Ok(T&& val) {
    return detail::OkHolder<T>{std::forward<T>(val)};
}

template <typename T, typename... Args> requires std::constructible_from<T, Args...>
detail::OkHolder<T> Ok(Args&&... args) {
    return T{std::forward<Args>(args)...};
}

template <concepts::IsError E>
detail::ErrHolder<E> Err(E&& val) {
    return detail::ErrHolder<E>{std::forward<E>(val)};
}

template <concepts::IsError E, typename... Args> requires std::constructible_from<E, Args...>
detail::ErrHolder<E> Err(Args&&... args) {
    return E{std::forward<Args>(args)...};
}

}  // namespace eav::make::result
