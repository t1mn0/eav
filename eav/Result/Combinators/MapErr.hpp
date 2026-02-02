#pragma once

#include <functional>  // std::invoke

#include "../../Result.hpp"

namespace eav::combine {

namespace pipe {

//                 (  func_  )
// Result<T, E> -> ( E -> E' ) -> Result<T, E'>

template <typename F>
struct MapErr {
    F func_;

    explicit MapErr(F&& f) : func_(std::move(f)) {}

    template <typename T, concepts::IsError E>
    auto Pipe(Result<T, E>&& res) {
        using Q = std::invoke_result_t<F, E>;

        if (res.is_ok()) {
            return Result<T, Q>(make::Ok(std::move(res).unwrap_ok()));
        }

        return Result<T, Q>(make::Err(std::invoke(std::move(func_), std::move(res).unwrap_err())));
    }

    template <typename T>
    auto Pipe(Result<T, detail::PendingType>&& res) {
        return std::move(res);
    }
};

}  // namespace pipe

template <typename F>
auto MapErr(F&& func) {
    return pipe::MapErr{std::forward<F>(func)};
}

}  // namespace eav::combine
