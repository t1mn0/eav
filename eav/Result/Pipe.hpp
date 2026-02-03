#pragma once

#include "../Concepts/PipeableWith.hpp"

namespace eav {

template <typename R, typename C>
requires(concepts::IsResult<R> && concepts::PipeableWith<C, R>)
auto operator|(R&& res, C&& comb) {
    return std::forward<C>(comb).Pipe(std::forward<R>(res));
}

}  // namespace eav
