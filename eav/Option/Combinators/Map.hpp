#pragma once

#include <functional>  // std::invoke

#include "../../Option.hpp"

namespace eav::combine::option {

namespace pipe {

//              (  func_ )
// Option<T> -> ( T -> U ) -> Option<U>

template <typename F>
struct Map {
    F func_;

    template <typename T> requires std::invocable<F, T>
    auto Pipe(Option<T>&& opt) {
        using U = std::invoke_result_t<F, T>;
        if (opt.has_value()) {
            return make::Some(std::invoke(std::move(func_), std::move(opt).unwrap()));
        }
        return Option<U>(make::None());
    }

    auto Pipe(Option<detail::PendingType>&& opt) {
        return std::move(opt);
    }
};
}  // namespace pipe

template <typename F>
auto Map(F&& func) {
    return pipe::Map<F>{std::forward<F>(func)};
}

}  // namespace eav::combine::option
