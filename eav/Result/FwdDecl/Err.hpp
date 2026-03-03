#pragma once

#include "../Concepts/IsError.hpp"
#include "../FwdDecl/Result.hpp"

namespace eav::make {

// forward declaration: Err()
template <concepts::IsError E>
Result<detail::PendingType, std::decay_t<E>> Err(E&& val);

}  // namespace eav::make
