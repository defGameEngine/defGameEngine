#include "Utils.hpp"

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

	uint8_t ClampFloatToUint8(float value)
	{
		return uint8_t(std::clamp(value, 0.0f, 255.0f));
	}

	uint8_t ClampIntToUint8(int value)
	{
		return uint8_t(std::clamp(value, 0, 255));
	}
}
