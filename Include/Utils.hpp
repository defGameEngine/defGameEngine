#ifndef DGE_UTILS_HPP
#define DGE_UTILS_HPP

#include <list>
#include <iostream>

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
				std::cout << val << std::endl;

			std::cerr << std::endl;

			exit(1);
		}
	}
}

#endif
