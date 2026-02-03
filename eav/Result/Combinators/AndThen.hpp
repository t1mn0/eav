#pragma once

#include "../../Concepts/IsResult.hpp"
#include "../../Result.hpp"

#include <functional>

namespace eav::combine::result {

namespace pipe {

//                 (      func_      )
// Result<T, E> -> (T -> Result<U, E>) -> Result<U, E>

template <typename F>
struct AndThen {
    F func_;

    template <typename T, concepts::IsError E>
    requires std::invocable<F, T> && concepts::IsResult<std::invoke_result_t<F, T>>
    auto Pipe(Result<T, E>&& res) {
        using NextResultT = std::invoke_result_t<F, T>;

        if (res.is_ok()) {
            return std::invoke(std::move(func_), std::move(res).unwrap_ok());
        }
        return NextResultT(make::Err(std::move(res).unwrap_err()));
    }

    template <concepts::IsError E>
    auto Pipe(Result<detail::PendingType, E>&& res) {
        return res;
    }
};

}  // namespace pipe

template <typename F>
auto AndThen(F&& func) {
    return pipe::AndThen<F>{std::forward<F>(func)};
}

}  // namespace eav::combine::result
