#pragma once
#include <type_traits>

template <typename T,
	typename std::enable_if<std::is_arithmetic<T>::value,
	T>::type* = nullptr>
struct Vec2
{
	T x;
	T y;
};