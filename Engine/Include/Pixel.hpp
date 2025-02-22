#pragma once

#ifndef DGE_PIXEL_HPP
#define DGE_PIXEL_HPP

#include "Pch.hpp"

namespace def
{
	struct Pixel
	{
		Pixel(uint32_t rgba = 0x000000FF);
		Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

		enum class Mode
		{
			DEFAULT,
			ALPHA,
			MASK,
			CUSTOM
		};

		union
		{
			struct { uint8_t r, g, b, a; };
			uint32_t rgba_n;
			uint8_t rgba_v[4];
		};

		Pixel Lerp(const Pixel& rhs, float factor) const;
		std::string ToString() const;

		Pixel operator+(float rhs) const;
		Pixel operator-(float rhs) const;
		Pixel operator*(float rhs) const;
		Pixel operator/(float rhs) const;

		Pixel& operator+=(float rhs);
		Pixel& operator-=(float rhs);
		Pixel& operator*=(float rhs);
		Pixel& operator/=(float rhs);

		Pixel operator+(const Pixel& rhs) const;
		Pixel operator-(const Pixel& rhs) const;
		Pixel operator*(const Pixel& rhs) const;
		Pixel operator/(const Pixel& rhs) const;

		Pixel& operator+=(const Pixel& rhs);
		Pixel& operator-=(const Pixel& rhs);
		Pixel& operator*=(const Pixel& rhs);
		Pixel& operator/=(const Pixel& rhs);

		bool operator==(const Pixel& rhs) const;
		bool operator!=(const Pixel& rhs) const;
		bool operator>(const Pixel& rhs) const;
		bool operator<(const Pixel& rhs) const;
		bool operator>=(const Pixel& rhs) const;
		bool operator<=(const Pixel& rhs) const;

		bool operator==(float rhs) const;
		bool operator!=(float rhs) const;
		bool operator>(float rhs) const;
		bool operator<(float rhs) const;
		bool operator>=(float rhs) const;
		bool operator<=(float rhs) const;

		static Pixel Float(float r, float g, float b, float a = 1.0f);
	};

	inline const Pixel
		BLACK(0, 0, 0), DARK_BLUE(0, 55, 218), DARK_GREEN(19, 161, 14), DARK_CYAN(59, 120, 255),
		DARK_RED(197, 15, 31), DARK_MAGENTA(136, 32, 152), DARK_GREY(118, 118, 118),
		DARK_ORANGE(255, 140, 0), DARK_BROWN(76, 63, 47), DARK_PURPLE(112, 31, 126), ORANGE(255, 165, 0),
		GREY(200, 200, 200), BLUE(0, 0, 255), GREEN(0, 255, 0), CYAN(0, 255, 255), RED(255, 0, 0),
		MAGENTA(180, 0, 158), YELLOW(255, 255, 0), WHITE(255, 255, 255), GOLD(255, 203, 0),
		PINK(255, 109, 194), MAROON(190, 33, 55), LIME(0, 158, 47), BROWN(127, 106, 79),
		BEIGE(211, 176, 131), VIOLET(135, 60, 190), PURPLE(200, 122, 255), NONE(0, 0, 0, 0);
}

#endif
