#pragma once

#include <utility>  // std::forward

#include "../Detail/Pending.hpp"
#include "Concepts/IsError.hpp"
#include "Detail/Tags.hpp"

namespace eav {

template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
class Result;

namespace make {

// Ok(T) => Result<DecayedT, PendingType>
template <typename T>
Result<std::decay_t<T>, detail::PendingType> Ok(T&& val) {
    using DecayedT = std::decay_t<T>;
    return Result<DecayedT, detail::PendingType>(detail::OkTag{}, std::forward<T>(val));
}

// Err(E) => Result<PendingType, DecayedE>
template <concepts::IsError E>
Result<detail::PendingType, std::decay_t<E>> Err(E&& val) {
    using DecayedE = std::decay_t<E>;
    return Result<detail::PendingType, DecayedE>(detail::ErrTag{}, std::forward<E>(val));
}

}  // namespace make

}  // namespace eav
