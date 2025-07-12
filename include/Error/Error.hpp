#ifndef FPP_ERROR_HPP
#define FPP_ERROR_HPP

#include <string>
#include <concepts> // for: std::convertible_to;
#include <functional> // for: std::invoke;

#include "../Result/Result.hpp"
#include "ErrorConcept.hpp"

namespace fpp {

//* <--- Basic wrappers for classic error handling methods --->
struct StringError {
private:
    std::string msg = "Undefined error";

public:
    explicit StringError(const std::string& msg) : msg(msg) {}
    explicit StringError(const char* msg) : msg(msg) {}

    std::string err_message() const noexcept { return msg; }

    bool operator==(const StringError& oth) const noexcept { return msg == oth.msg; }
};

struct CodeError {
private:
    int code;

public:
    explicit CodeError(int code) : code(code) {}

    std::string err_message() const noexcept { return "Error code: " + std::to_string(code); }

    bool operator==(const CodeError& oth) const noexcept { return code == oth.code; }
};

struct ExceptionError {
private:
    std::string msg;
    std::string type_name;

public:
    template<typename E> requires std::derived_from<E, std::exception>
    explicit ExceptionError(const E& ex) : msg(ex.what()), type_name(typeid(ex).name()) {}

    explicit ExceptionError() : msg("Unknown exception"), type_name("unknown") {}
    
    std::string err_message() const noexcept { return "[" + type_name + "]: "+ msg; }
    
    bool operator==(const ExceptionError& oth) const noexcept { return type_name == oth.type_name && msg == oth.msg; }
};

//* <--- Functions that can help to process code written in the style of exception handling, in the style of error handling --->

// forward-declaration:
template<typename T, typename E> requires (!std::is_void_v<T> && Error<E>)
class Result;

// analog of 'try', which converts the received exception to Error
template<typename Fn, typename... Args>
auto try_or_convert(Fn&& fn, Args&&... args) -> Result<std::invoke_result_t<Fn, Args...>, ExceptionError> {
    try {
        return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Ok(std::invoke(std::forward<Fn>(fn), std::forward<Args>(args)...));
    } catch (const std::exception& e) {
        return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Err(ExceptionError{e});
    } catch (...) {
        return Result<std::invoke_result_t<Fn, Args...>, ExceptionError>::Err(ExceptionError{});
    }
}

} // namespace 'fpp'

// #include "../../src/Error/Error.tpp"

#endif // FPP_ERROR_HPP