#pragma once

#include <functional>  // std::invoke
#include "../../Option.hpp"

namespace eav::combine::option {

namespace pipe {

//              (      func_     )
// Option<T> -> (void -> Option<T>) -> Option<T>

template <typename F>
struct OrElse {
    F func_;

    template <typename T>
    auto Pipe(Option<T>&& opt) {
        if (opt.has_value()) {
            return std::move(opt);
        }
        return std::invoke(std::move(func_));
    }

    auto Pipe(Option<detail::PendingType>&&) {
        return std::invoke(std::move(func_));
    }
};

}  // namespace pipe

template <typename F>
auto OrElse(F&& func) {
    return pipe::OrElse<std::decay_t<F>>{std::forward<F>(func)};
}

}  // namespace eav::combine::option
