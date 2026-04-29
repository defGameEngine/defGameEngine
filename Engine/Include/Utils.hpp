/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_UTILS_HPP
#define DGE_UTILS_HPP

#include "Pch.hpp"

namespace def
{
	// Prints an error to the console and terminates an application
	template <class... T>
	inline void Assert(bool expr, T&&... args)
	{
		if (!expr)
		{
			std::list<const char*> values;
			(values.emplace_back(std::move(args)), ...);

			for (const auto& val : values)
				fprintf(stderr, "%s\n", val);

			fprintf(stderr, "\n");

			exit(1);
		}
	}

	inline uint8_t ClampFloatToUint8(float value)
	{
		return uint8_t(std::clamp(value, 0.0f, 255.0f));
	}

	inline uint8_t ClampIntToUint8(int value)
	{
		return uint8_t(std::clamp(value, 0, 255));
	}
}

#endif
