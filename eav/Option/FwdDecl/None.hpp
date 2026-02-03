#pragma once

#include "../../Detail/Pending.hpp"
#include "Option.hpp"

namespace eav::make {

// None() => Option<?>
inline Option<detail::PendingType> None();

}  // namespace eav::make
