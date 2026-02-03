#pragma once

#include <type_traits>  // std::is_void_v

#include "../Concepts/IsError.hpp"

namespace eav {

// forward declaration: Result<T,E>
template <typename T, concepts::IsError E> requires(!std::is_void_v<T>)
class Result;

}  // namespace eav
