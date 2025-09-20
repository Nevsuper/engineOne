#pragma once
#include <type_traits>
template <typename E>
inline constexpr auto enumValue(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}

template <typename E>
inline constexpr E toEnum(std::underlying_type_t<E> value) noexcept {
	return static_cast<E>(value);
}

template <typename T>
inline void SafeDeletePtr(T*& ptr) noexcept
{
    if (ptr != nullptr)
    {
        delete ptr;
        ptr = nullptr;
    }
}