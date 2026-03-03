#pragma once

#include <concepts>
#include <type_traits>

#include "../../Detail/Pending.hpp"

namespace eav::concepts {

template <typename E>
concept IsError =
    std::same_as<E, detail::PendingType> ||
    (std::move_constructible<E> &&
     std::destructible<E> &&
     !std::is_void_v<E> &&
     !std::is_array_v<E> &&
     !std::is_reference_v<E>);

}  // namespace eav::concepts
