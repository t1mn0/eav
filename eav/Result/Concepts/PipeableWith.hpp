#pragma once

#include <utility>  // std::forward

#include "IsResult.hpp"

namespace eav::concepts {

template <typename C, typename R>
concept PipeableWith = IsResult<std::remove_cvref_t<R>> && requires(C&& comb, R&& res) {
    { std::forward<C>(comb).Pipe(std::forward<R>(res)) } -> IsResult;
};

}  // namespace eav::concepts
