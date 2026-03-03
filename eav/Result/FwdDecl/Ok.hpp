#pragma once

#include "../FwdDecl/Result.hpp"

namespace eav::make {

// forward declaration: Ok()
template <typename T> requires(!std::is_void_v<std::decay_t<T>>)
Result<std::decay_t<T>, detail::PendingType> Ok(T&& val);

}  // namespace eav::make
