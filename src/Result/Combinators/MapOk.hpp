#pragma once

#include <functional>  // std::invoke

#include "../Result.hpp"

namespace eav::combine {

namespace pipe {

template <typename F>
struct MapOk {
    // func_: Result<T, E> -> (T -> U) -> Result<U, E>
    F func_;

    explicit MapOk(F f) : func_(std::move(f)) {}

    template <typename T, typename E> requires std::invocable<F, T>
    auto Pipe(Result<T, E>&& res) {
        using U = std::invoke_result_t<F, T>;

        if (res.is_ok()) {
            return Result<U, E>(make::Ok(std::invoke(std::move(func_), std::move(res).unwrap_ok())));
        } else {
            return Result<U, E>(make::Err(std::move(res).unwrap_err()));
        }
    }
};

}  // namespace pipe

template <typename F>
auto MapOk(F&& func) {
    return pipe::MapOk<std::decay_t<F>>{std::forward<F>(func)};
}

}  // namespace eav::combine
