#pragma once

#include <functional>  // std::invoke

#include "../../Option.hpp"

namespace eav::combine::option {

namespace pipe {

//              (    func_     )
// Option<T> -> (T -> Option<U>) -> Option<U>

template <typename F>
struct AndThen {
    F func_;

    template <typename T>
    requires std::invocable<F, T> && concepts::IsResult<std::invoke_result_t<F, T>>
    auto Pipe(Option<T>&& opt) {
        using NextOpt = std::invoke_result_t<F, T>;
        if (opt.has_value()) {
            return std::invoke(std::move(func_), std::move(opt).unwrap());
        }
        return NextOpt(make::None());
    }

    auto Pipe(Option<detail::PendingType>&& opt) {
        return std::move(opt);
    }
};

}  // namespace pipe

template <typename F>
auto AndThen(F&& func) {
    return pipe::AndThen<F>{std::forward<F>(func)};
}

}  // namespace eav::combine::option
