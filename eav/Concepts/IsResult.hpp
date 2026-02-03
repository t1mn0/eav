#pragma once

namespace eav::concepts {

template <typename R>
concept IsResult = requires(R res) {
    typename R::OkType;
    typename R::ErrType;
};

}  // namespace eav::concepts
