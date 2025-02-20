#pragma once

#ifndef DGE_UTILS_HPP
#define DGE_UTILS_HPP

#include <list>
#include <cstdio>

namespace def
{
	template <class... T>
	void Assert(bool expr, T&&... args)
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
}

#endif
