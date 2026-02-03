#pragma once

#include <type_traits>  // std::decay_t
#include <utility>      // std::forward

#include "../Detail/Pending.hpp"
#include "Detail/Tags.hpp"
#include "FwdDecl/Option.hpp"

namespace eav::make {

// Some(T) => Option<T>
template <typename T>
Option<std::decay_t<T>> Some(T&& val) {
    return Option<std::decay_t<T>>(detail::SomeTag{}, std::forward<T>(val));
}

// None() => Option<?>
inline Option<detail::PendingType> None() {
    return Option<detail::PendingType>(detail::NoneTag{});
}

}  // namespace eav::make
