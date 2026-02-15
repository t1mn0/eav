#pragma once


#include "../../Option.hpp"

namespace eav {

template <typename T>
requires(!std::is_void_v<T>)
class [[nodiscard]] Option<T&> {
  public:  // nested types:
    using OkType = T&;
    using ErrType = void;

  private:  // data members:
    T* storage_ = nullptr;

  public:  // member functions:
    // Constructors and destructor:
    Option() = delete;
    Option(const Option& oth) = default;
    Option(Option&& oth) = default;

    template <typename U> requires(std::same_as<U, detail::PendingType>)
    Option(Option<U>&& oth);

    ~Option() = default;  // ref is not responsible for deleting the pointer!

    // Operators:
    Option& operator=(const Option& oth) = default;
    Option& operator=(Option&& oth) noexcept = default;

    // Observers:
    bool has_value() const noexcept;
    operator bool() const noexcept;

    // Accessors:
    const T* ptr() const { return storage_; }

    T* ptr() { return storage_; }

    constexpr T& unwrap(std::string_view msg = "called .unwrap() on None") const;
    constexpr T unwrap_or(T&& else_val) const;

  private:  // member functions:
    template <typename U> requires std::constructible_from<T*, U*>
    Option(detail::SomeTag, U& val);

    Option(detail::NoneTag);

  private:  // friends declaration:
    template <typename U>
    friend Option<std::decay_t<U>> make::Some(U&&);

    template <typename U>
    friend Option<U&> make::Some(U&);

    friend inline Option<detail::PendingType> make::None();
};

}  // namespace eav

#include "PartialSpecImpl.hpp"
