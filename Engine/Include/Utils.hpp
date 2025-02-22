#pragma once

#ifndef DGE_UTILS_HPP
#define DGE_UTILS_HPP

#include "Pch.hpp"

namespace def
{
	template <class... T>
	inline void Assert(bool expr, T&&... args);

	inline uint8_t ClampFloatToUint8(float value);
	inline uint8_t ClampIntToUint8(int value);
}

#include "Utils.inl"

#endif
