#pragma once

#include <functional>  // std::invoke

#include "../../Result.hpp"

namespace eav::combine {

namespace pipe {

//                 (predicate_ )
// Result<T, E> -> ( T -> bool ) -> Result<T, E>

template <typename P, concepts::IsError E>
struct Filter {
    P predicate_;
    E else_err_;

    explicit Filter(P&& p, E&& e) : predicate_(std::move(p)), else_err_(std::move(e)) {}

    auto Pipe(Result<detail::PendingType, E>&& res) {
        return std::move(res);
    }

    template <typename T> requires std::invocable<P, T> && std::same_as<std::invoke_result_t<P, T>, bool>
    auto Pipe(Result<T, detail::PendingType>&& res) {
        if (std::invoke(predicate_, res.unwrap_ok())) {
            return Result<T, E>(make::Ok(T{std::move(res.unwrap_ok())}));
        }
        return Result<T, E>(make::Err(E{std::move(else_err_)}));
    }

    template <typename T> requires std::invocable<P, T> && std::same_as<std::invoke_result_t<P, T>, bool>
    auto Pipe(Result<T, E>&& res) {
        if (res.is_ok() && std::invoke(predicate_, res.unwrap_ok())) {
            return Result<T, E>(make::Ok(T{std::move(res.unwrap_ok())}));
        }
        return Result<T, E>(make::Err(E{std::move(else_err_)}));
    }
};

}  // namespace pipe

template <typename P, concepts::IsError E>
auto Filter(P&& predicate, E&& else_err) {
    return pipe::Filter{std::move(predicate), std::move(else_err)};
}

}  // namespace eav::combine
