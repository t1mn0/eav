#pragma once

#include <new>  // placement new
#include <stdexcept>

#include "../../Option.hpp"

namespace eav {

// --- Constructors ---

template <typename T> requires(!std::is_void_v<T>)
template <typename U> requires std::constructible_from<T, U>
Option<T>::Option(detail::SomeTag, U&& val) : has_value_(true) {
    new (storage_) T(std::forward<U>(val));
}

template <typename T> requires(!std::is_void_v<T>)
Option<T>::Option(detail::NoneTag) : has_value_(false) {}

template <typename T> requires(!std::is_void_v<T>)
Option<T>::~Option() {
    if (has_value_) {
        ptr()->~T();
    }
}

template <typename T> requires(!std::is_void_v<T>)
Option<T>::Option(const Option& oth) : has_value_(oth.has_value_) {
    if (has_value_) {
        new (storage_) T(*oth.ptr());
    }
}

template <typename T> requires(!std::is_void_v<T>)
Option<T>::Option(Option&& oth) noexcept(std::is_nothrow_move_constructible_v<T>)
    : has_value_(oth.has_value_) {
    if (has_value_) {
        new (storage_) T(std::move(*reinterpret_cast<T*>(oth.storage_)));
    }
}

template <typename T> requires(!std::is_void_v<T>)
template <typename U> requires(std::same_as<U, detail::PendingType>)
Option<T>::Option(Option<U>&& oth) : has_value_(oth.has_value()) {
    if (has_value_) {
        new (storage_) T(std::move(*reinterpret_cast<T*>(oth.storage_)));
    }
}

// --- Operators ---

template <typename T> requires(!std::is_void_v<T>)
Option<T>::operator bool() const noexcept {
    return has_value_;
}

// --- Observers ---

template <typename T> requires(!std::is_void_v<T>)
bool Option<T>::has_value() const noexcept {
    return has_value_;
}

// --- Accessors: unwrap ---

template <typename T> requires(!std::is_void_v<T>)
constexpr const T& Option<T>::unwrap(std::string_view msg) const& {
    if (!has_value_) throw std::runtime_error(std::string(msg));
    return *ptr();
}

template <typename T> requires(!std::is_void_v<T>)
constexpr T& Option<T>::unwrap(std::string_view msg) & {
    if (!has_value_) throw std::runtime_error(std::string(msg));
    return *ptr();
}

template <typename T> requires(!std::is_void_v<T>)
constexpr T Option<T>::unwrap(std::string_view msg) && {
    if (!has_value_) throw std::runtime_error(std::string(msg));
    return *ptr();
}

// --- Accessors: unwrap_or ---

template <typename T> requires(!std::is_void_v<T>)
constexpr T Option<T>::unwrap_or(T&& else_val) const& {
    return has_value_ ? *ptr() : else_val;
}

template <typename T> requires(!std::is_void_v<T>)
template <typename U> requires std::same_as<T, detail::PendingType>
constexpr U Option<T>::unwrap_or(U&& else_val) const& {
    return else_val;
}

// --- Accessors: ptr ---

template <typename T> requires(!std::is_void_v<T>)
const T* Option<T>::ptr() const {
    return reinterpret_cast<const T*>(storage_);
}

template <typename T> requires(!std::is_void_v<T>)
T* Option<T>::ptr() {
    return reinterpret_cast<T*>(storage_);
}

}  // namespace eav
