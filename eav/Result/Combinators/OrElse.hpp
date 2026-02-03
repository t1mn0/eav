#pragma once

#include "../../Result.hpp"

#include <functional>

namespace eav::combine::result {

namespace pipe {

//                 (       func_      )
// Result<T, E> -> (E -> Result<T, E'>) -> Result<T, E'>

template <typename F>
struct OrElse {
    F func_;

    template <typename T, concepts::IsError E>
    requires std::invocable<F, E> && concepts::IsResult<std::invoke_result_t<F, E>>
    auto Pipe(Result<T, E>&& res) {
        if constexpr (std::same_as<E, detail::PendingType>) {
            return std::move(res);
        }

        using NextResultT = std::invoke_result_t<F, E>;

        if (res.is_err()) {
            return std::invoke(std::move(func_), std::move(res).unwrap_err());
        }

        using NewT = typename NextResultT::OkType;
        using NewE = typename NextResultT::ErrType;
        return Result<NewT, NewE>(make::Ok(std::move(res).unwrap_ok()));
    }

    template <typename T>
    auto Pipe(Result<T, detail::PendingType>&& res) {
        return std::move(res);
    }
};

}  // namespace pipe

template <typename F>
auto OrElse(F&& func) {
    return pipe::OrElse<F>{std::forward<F>(func)};
}

}  // namespace eav::combine::result
