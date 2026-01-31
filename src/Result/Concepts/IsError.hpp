#pragma once

#include <concepts>
#include <type_traits>

namespace eav::concepts {

template <typename E>
concept IsError =
    std::move_constructible<E> &&
    std::destructible<E> &&
    !std::is_void_v<E> &&
    !std::is_array_v<E> &&
    !std::is_reference_v<E>;

}  // namespace eav::concepts
