#pragma once

#include "Option.hpp"

namespace eav::make {

// Some(T) => Option<T>
template <typename T>
Option<std::decay_t<T>> Some(T&& val);

}  // namespace eav::make
