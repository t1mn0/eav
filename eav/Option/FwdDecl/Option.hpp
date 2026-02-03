#pragma once

#include <type_traits>  // std::is_void_v

namespace eav {

template <typename T> requires(!std::is_void_v<T>)
class Option;

}  // namespace eav
