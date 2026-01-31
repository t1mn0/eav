#pragma once

#include "Concepts/PipeableWith.hpp"

namespace eav::result::pipe {

template <concepts::IsResult R, typename C> requires concepts::PipeableWith<C, R>
auto operator|(R&& res, C&& comb) {
    return std::forward<C>(comb).Pipe(std::forward<R>(res));
}

}  // namespace eav::result::pipe
