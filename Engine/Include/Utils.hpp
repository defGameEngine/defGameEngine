#pragma once

#ifndef DGE_UTILS_HPP
#define DGE_UTILS_HPP

#include "Pch.hpp"

namespace def
{
	// Prints an error to the console and terminates an application
	template <class... T>
	inline void Assert(bool expr, T&&... args);

	inline uint8_t ClampFloatToUint8(float value);
	inline uint8_t ClampIntToUint8(int value);
}

#include "../Sources/Utils.cpp"

#endif
