#ifndef FPP_ERROR_CONCEPT_HPP
#define FPP_ERROR_CONCEPT_HPP

#include <string>
#include <concepts> // for: std::convertible_to<T>;

namespace fpp {

//* <--- The basic concept that the "Error" type structure should correspond to --->
template<typename E>
concept Error = requires(const E& e, E&& moved) {
    { e.err_message() } -> std::convertible_to<std::string>;
    requires std::destructible<E>;
    requires std::copy_constructible<E> || std::move_constructible<E>;
    requires std::equality_comparable<E>;
};

} // end of namespace 'fpp'

#endif // FPP_ERROR_CONCEPT_HPP