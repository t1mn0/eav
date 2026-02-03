#pragma once

#include <functional>  // std::invoke
#include "../../Option.hpp"

namespace eav::combine::option {

namespace pipe {

//              (predicate_ )
// Option<T> -> ( T -> bool ) -> Option<T>

template <typename P>
struct Filter {
    P predicate_;

    template <typename T>
    requires std::invocable<P, T> && std::same_as<std::invoke_result_t<P, T>, bool>
    auto Pipe(Option<T>&& opt) {
        if (opt.has_value()) {
            if (std::invoke(predicate_, opt.unwrap())) {
                return std::move(opt);
            }
            return Option<T>(make::None());
        }
        return std::move(opt);
    }

    auto Pipe(Option<detail::PendingType>&& opt) {
        return std::move(opt);
    }
};

}  // namespace pipe

template <typename P>
auto Filter(P&& predicate) {
    return pipe::Filter<std::decay_t<P>>{std::forward<P>(predicate)};
}

}  // namespace eav::combine::option
