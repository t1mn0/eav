#pragma once

namespace eav::concepts {

// Option<T>   is Result
// Result<T,E> is Result
template <typename R>
concept IsResult = requires(R res) {
    typename R::OkType;
    typename R::ErrType;
};

}  // namespace eav::concepts
