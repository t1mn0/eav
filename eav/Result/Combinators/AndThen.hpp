#pragma once

#include "../../Result.hpp"
#include "../Concepts/IsResult.hpp"

#include <functional>

namespace eav::combine {

namespace pipe {

//                 (      func_      )
// Result<T, E> -> (T -> Result<U, E>) -> Result<U, E>

template <typename F>
struct AndThen {
    F func_;

    template <typename T, typename E>
    requires std::invocable<F, T> && concepts::IsResult<std::invoke_result_t<F, T>>
    auto Pipe(Result<T, E>&& res) {
        using NextResultT = std::invoke_result_t<F, T>;

        if (res.is_ok()) {
            return std::invoke(std::move(func_), std::move(res).unwrap_ok());
        }
        return NextResultT(make::Err(std::move(res).unwrap_err()));
    }
};

}  // namespace pipe

template <typename F>
auto AndThen(F&& func) {
    return pipe::AndThen<std::decay_t<F>>{std::forward<F>(func)};
}

}  // namespace eav::combine
