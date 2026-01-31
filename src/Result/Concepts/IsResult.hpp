#pragma once

#include <concepts>

namespace eav::concepts {

template <typename R>
concept IsResult = requires(R res) {
    { res.is_ok() } -> std::convertible_to<bool>;
    { res.is_err() } -> std::convertible_to<bool>;
};

}  // namespace eav::concepts
