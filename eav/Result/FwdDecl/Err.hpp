#pragma once

#include "../Concepts/IsError.hpp"
#include "../FwdDecl/Result.hpp"

namespace eav::make {

// forward declaration: Err()
template <typename E> requires concepts::IsError<std::decay_t<E>>
Result<detail::PendingType, std::decay_t<E>> Err(E&& val);

}  // namespace eav::make
