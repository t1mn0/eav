#pragma once

#include <functional>  // std::invoke

#include "../../Result.hpp"

namespace eav::combine {

namespace pipe {

//                 (  func_ )
// Result<T, E> -> ( T -> U ) -> Result<U, E>

template <typename F>
struct MapOk {
    F func_;

    explicit MapOk(F&& f) : func_(std::move(f)) {}

    template <typename T, concepts::IsError E> requires std::invocable<F, T>
    auto Pipe(Result<T, E>&& res) {
        using U = std::invoke_result_t<F, T>;

        if (res.is_ok()) {
            return Result<U, E>(make::Ok(std::invoke(std::move(func_), std::move(res).unwrap_ok())));
        }

        return Result<U, E>(make::Err(std::move(res).unwrap_err()));
    }

    template <concepts::IsError E>
    auto Pipe(Result<detail::PendingType, E>&& res) {
        return std::move(res);
    }
};

}  // namespace pipe

template <typename F>
auto MapOk(F&& func) {
    return pipe::MapOk{std::forward<F>(func)};
}

}  // namespace eav::combine
