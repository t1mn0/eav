#pragma once

#include "../../Result.hpp"
#include "../Concepts/IsResult.hpp"

#include <functional>

namespace eav::combine {

namespace pipe {

//                 (       func_      )
// Result<T, E> -> (E -> Result<T, E'>) -> Result<T, E'>

template <typename F>
struct OrElse {
    F func_;

    template <typename T, typename E>
    requires std::invocable<F, E> && concepts::IsResult<std::invoke_result_t<F, E>>
    auto Pipe(Result<T, E>&& res) {
        using NextResultT = std::invoke_result_t<F, E>;

        if (res.is_err()) {
            return std::invoke(std::move(func_), std::move(res).unwrap_err());
        }

        return NextResultT(make::Ok(std::move(res).unwrap_ok()));
    }
};

}  // namespace pipe

template <typename F>
auto OrElse(F&& func) {
    return pipe::OrElse<std::decay_t<F>>{std::forward<F>(func)};
}

}  // namespace eav::combine
