#pragma once

#include <stdexcept>
#include <utility>

#include "../Result.hpp"

namespace eav {

// --- Constructors ---

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
template <typename U>
Result<T, E>::Result(detail::OkHolder<U>&& ok) : value_(std::in_place_index<0>, std::forward<U>(ok.val_)) {}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
template <typename U>
Result<T, E>::Result(detail::ErrHolder<U>&& err) : value_(std::in_place_index<1>, std::forward<U>(err.val_)) {}

// --- Operators ---

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
Result<T, E>::operator bool() const noexcept {
    return is_ok();
}

// --- Observers ---

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
bool Result<T, E>::is_ok() const noexcept {
    return value_.index() == 0;
}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
bool Result<T, E>::is_err() const noexcept {
    return value_.index() == 1;
}

// --- Accessors: unwrap_ok ---

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
constexpr const T& Result<T, E>::unwrap_ok(std::string_view msg) const& {
    if (is_err()) throw std::runtime_error(std::string(msg));
    return std::get<0>(value_);
}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
constexpr T& Result<T, E>::unwrap_ok(std::string_view msg) & {
    if (is_err()) throw std::runtime_error(std::string(msg));
    return std::get<0>(value_);
}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
constexpr T Result<T, E>::unwrap_ok(std::string_view msg) && {
    if (is_err()) throw std::runtime_error(std::string(msg));
    return std::get<0>(std::move(value_));
}

// --- Accessors: unwrap_ok_or ---

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
template <typename U>
constexpr T Result<T, E>::unwrap_ok_or(U&& else_val) const& {
    if (is_ok()) return std::get<0>(value_);
    return static_cast<T>(std::forward<U>(else_val));
}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
template <typename U>
constexpr T Result<T, E>::unwrap_ok_or(U&& else_val) && {
    if (is_ok()) return std::get<0>(std::move(value_));
    return static_cast<T>(std::forward<U>(else_val));
}

// --- Accessors: unwrap_err ---

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
constexpr const E& Result<T, E>::unwrap_err(std::string_view msg) const& {
    if (is_ok()) throw std::runtime_error(std::string(msg));
    return std::get<1>(value_);
}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
constexpr E& Result<T, E>::unwrap_err(std::string_view msg) & {
    if (is_ok()) throw std::runtime_error(std::string(msg));
    return std::get<1>(value_);
}

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
constexpr E Result<T, E>::unwrap_err(std::string_view msg) && {
    if (is_ok()) throw std::runtime_error(std::string(msg));
    return std::get<1>(std::move(value_));
}

}  // namespace eav
