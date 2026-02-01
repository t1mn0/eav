#pragma once

#include <utility>  // std::forward

#include "Concepts/IsError.hpp"
#include "Detail/Pending.hpp"

namespace eav {

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
class Result;

namespace make {
template <typename T>
Result<T, detail::PendingType> Ok(T&& val) {
    return Result<T, detail::PendingType>(detail::OkTag{}, std::forward<T>(val));
}

template <concepts::IsError E>
Result<detail::PendingType, E> Err(E&& val) {
    return Result<detail::PendingType, E>(detail::ErrTag{}, std::forward<E>(val));
}

}  // namespace make

}  // namespace eav
