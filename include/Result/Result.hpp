#ifndef RESULT_H
#define RESULT_H

#include <concepts> // for: default_initializable;

namespace throwless {

template<typename T, typename E>
class Result {
private: //* supstructures :
    enum class State {
        Ok,
        Error,
    };

private: //* fields :
    State state;
    union { T ok_value; E err_value; };
    
public: //* methods :
    //*   <--- static mnemonic methods that call the constructor from an argument --->
    static Result Ok(T value) noexcept;
    #if 0
    static Result Ok(const T& value) noexcept;
    static Result Ok(T&& value) noexcept;
    #endif

    static Result Err(E error) noexcept;
    #if 0
    static Result Err(const E& error) noexcept;
    static Result Err(E&& error) noexcept;
    #endif

    //*   <--- constructors, (~)ro5, destructor --->
    Result(const Result& other) noexcept;
    Result(Result&& other) noexcept;
    Result& operator=(const Result& other) noexcept;
    Result& operator=(Result&& other) noexcept;
    ~Result();

    //*   <--- specialized algorithms & methods  --->
    bool is_ok() const noexcept;
    bool is_err() const noexcept;

    const T& unwrap() const;

    T& unwrap_or(T& default_value) noexcept;
    const T& unwrap_or(const T& default_value) const noexcept;
    T unwrap_or_default() const noexcept 
        requires std::default_initializable<T>;

    E unwrap_err() const;

    // TODO : template<typename F>  T unwrap_or_else(F fn) const;

    //*   <--- functional methods (from funcprog)  --->
    // TODO : map
    // TODO : map_err
    // TODO : and_then
    // TODO : and_then_async
    // TODO : or_else 

private: //* methods :
    //*   <--- constructors that are called by static methods Ok(value), Err(error) --->
    explicit Result(T&& value) noexcept;
    explicit Result(E&& error) noexcept;

}; // end of class Result

} // end of namespace throwless

#include "../../src/Result/Result.tpp"

#endif