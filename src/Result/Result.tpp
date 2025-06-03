#include "../../include/Result/Result.hpp"

#ifndef RESULT_H
#error "Include Result.hpp instead of Result.tpp"
#endif

#include <stdexcept> // for: std::runtime_error;
#include <utility> // for std::move;

namespace throwless {

//*   <--- constructors, (~)ro5, destructor --->

template <typename T, typename E>
Result<T, E>::Result(T&& value) noexcept : state(State::Ok), ok_value(std::move(value)) {}

template <typename T, typename E>
Result<T, E>::Result(E&& error) noexcept : state(State::Error), err_value(std::move(error)) {}

template <typename T, typename E>
Result<T,E> Result<T, E>::Ok(T value) noexcept {
    return Result(std::move(value));
}

#if 0
template <typename T, typename E>
Result<T,E> Result<T, E>::Ok(const T& value) noexcept {
    return Result(std::move(value));
}

template <typename T, typename E>
Result<T,E> Result<T, E>::Ok(T&& value) noexcept {
    return Result(std::move(value));
}
#endif

template <typename T, typename E>
Result<T,E> Result<T, E>::Err(E error) noexcept {
    return Result(std::move(error));
}

#if 0
template <typename T, typename E>
Result<T,E> Result<T, E>::Err(const E& error) noexcept {
    return Result(std::move(error));
}

template <typename T, typename E>
Result<T,E> Result<T, E>::Err(E&& error) noexcept {
    return Result(std::move(error));
}
#endif

template <typename T, typename E>
Result<T, E>::Result(const Result& other) noexcept {
    if (other.is_ok()) {
        ok_value = other.ok_value;
    } 
    else {
        err_value = other.err_value;
    }
}

template <typename T, typename E>
Result<T, E>::Result(Result&& other) noexcept {
    if (other.is_ok()) {
        ok_value = std::move(other.ok_value);
    } 
    else {
        err_value = std::move(other.err_value);
    }
}

template <typename T, typename E>
Result<T,E>& Result<T, E>::operator=(const Result<T,E>& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (state == State::Ok) {
        ok_value.~T();
    }
    else if (state == State::Error) {
        err_value.~E();
    }

    state = other.state;
    if (state == State::Ok) {
        ok_value = other.ok_value;
    } 
    else if (state == State::Error) {
        err_value = other.err_value;
    }

    return *this;
}

template <typename T, typename E>
Result<T,E>& Result<T,E>::operator=(Result<T,E>&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    if (state == State::Ok) {
        ok_value.~T();
    } 
    else if (state == State::Error) {
        err_value.~E();
    }

    state = other.state;
    if (state == State::Ok) {
        ok_value = other.ok_value;
    } 
    else if (state == State::Error) {
        err_value = other.err_value;
    }

    return *this;
}

template <typename T, typename E>
Result<T, E>::~Result() {
    if (is_ok()) {
        ok_value.~T();
    }
    else {
        err_value.~E();
    }
}

//*   <--- specialized algorithms & methods  --->

template <typename T, typename E>
bool Result<T,E>::is_ok() const noexcept { 
    return state == State::Ok; 
}

template <typename T, typename E>
bool Result<T,E>::is_err() const noexcept { 
    return state == State::Error; 
}

template <typename T, typename E>
const T& Result<T,E>::unwrap() const {
    if (is_err()) {
        throw std::runtime_error("Called unwrap() on an Err value!");
    }
    return ok_value;
}

template <typename T, typename E>
T& Result<T,E>::unwrap_or(T& default_value) noexcept {
    return is_ok() ? ok_value : default_value;
}

template <typename T, typename E>
const T& Result<T,E>::unwrap_or(const T& default_value) const noexcept {
    return is_ok() ? ok_value : default_value;
}

template <typename T, typename E>
T Result<T, E>::unwrap_or_default() const noexcept requires std::default_initializable<T> {
    return T();
}

template <typename T, typename E>
E Result<T, E>::unwrap_err() const {
    if (is_ok()) {
        throw std::runtime_error("Called unwrap_err() on an valid value!");
    }
    return err_value;
}

}
