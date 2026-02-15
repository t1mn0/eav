#pragma once

#include "PartialSpecDecl.hpp"

namespace eav {

template <typename T> requires(!std::is_void_v<T>)
template <typename U> requires std::constructible_from<T*, U*>
Option<T&>::Option(detail::SomeTag, U& val) : storage_(&val) {}

template <typename T> requires(!std::is_void_v<T>)
Option<T&>::Option(detail::NoneTag) : storage_(nullptr) {}

template <typename T> requires(!std::is_void_v<T>)
template <typename U> requires(std::same_as<U, detail::PendingType>)
Option<T&>::Option(Option<U>&&) : storage_(nullptr) {}

template <typename T> requires(!std::is_void_v<T>)
bool Option<T&>::has_value() const noexcept {
    return storage_ != nullptr;
}

template <typename T> requires(!std::is_void_v<T>)
Option<T&>::operator bool() const noexcept {
    return has_value();
}

template <typename T> requires(!std::is_void_v<T>)
constexpr T& Option<T&>::unwrap(std::string_view msg) const {
    if (storage_ == nullptr) throw std::runtime_error(std::string(msg));
    return *storage_;
}

template <typename T> requires(!std::is_void_v<T>)
constexpr T Option<T&>::unwrap_or(T&& else_val) const {
    return (storage_ != nullptr) ? *storage_ : std::forward<T>(else_val);
}

}  // namespace eav
