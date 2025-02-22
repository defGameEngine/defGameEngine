#include "Pch.hpp"
#include "Pixel.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <cmath>

namespace def
{
	Pixel::Pixel(uint32_t rgba) : rgba_n(rgba)
	{

	}

	Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a)
	{

	}

	Pixel Pixel::Lerp(const Pixel& rhs, float factor) const
	{
		return Pixel(
			(uint8_t)std::lerp(r, rhs.r, factor),
			(uint8_t)std::lerp(g, rhs.g, factor),
			(uint8_t)std::lerp(b, rhs.b, factor),
			(uint8_t)std::lerp(a, rhs.a, factor)
		);
	}

	std::string Pixel::ToString() const
	{
		return "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")";
	}

	Pixel& Pixel::operator+=(float rhs)
	{
		r = ClampFloatToUint8((float)r + rhs);
		g = ClampFloatToUint8((float)g + rhs);
		b = ClampFloatToUint8((float)b + rhs);
		return *this;
	}

	Pixel& Pixel::operator-=(float rhs)
	{
		r = ClampFloatToUint8((float)r - rhs);
		g = ClampFloatToUint8((float)g - rhs);
		b = ClampFloatToUint8((float)b - rhs);
		return *this;
	}

	Pixel& Pixel::operator*=(float rhs)
	{
		r = ClampFloatToUint8((float)r * rhs);
		g = ClampFloatToUint8((float)g * rhs);
		b = ClampFloatToUint8((float)b * rhs);
		return *this;
	}

	Pixel& Pixel::operator/=(float rhs)
	{
		r = ClampFloatToUint8((float)r / rhs);
		g = ClampFloatToUint8((float)g / rhs);
		b = ClampFloatToUint8((float)b / rhs);
		return *this;
	}

	Pixel Pixel::operator+(float rhs) const
	{
		return Pixel(
			ClampFloatToUint8((float)r + rhs),
			ClampFloatToUint8((float)g + rhs),
			ClampFloatToUint8((float)b + rhs)
		);
	}

	Pixel Pixel::operator-(float rhs) const
	{
		return Pixel(
			ClampFloatToUint8((float)r - rhs),
			ClampFloatToUint8((float)g - rhs),
			ClampFloatToUint8((float)b - rhs)
		);
	}

	Pixel Pixel::operator*(float rhs) const
	{
		return Pixel(
			ClampFloatToUint8((float)r * rhs),
			ClampFloatToUint8((float)g * rhs),
			ClampFloatToUint8((float)b * rhs)
		);
	}

	Pixel Pixel::operator/(float rhs) const
	{
		return Pixel(
			ClampFloatToUint8((float)r / rhs),
			ClampFloatToUint8((float)g / rhs),
			ClampFloatToUint8((float)b / rhs)
		);
	}

	Pixel& Pixel::operator+=(const Pixel& rhs)
	{
		r = ClampIntToUint8((int)r + (int)rhs.r);
		g = ClampIntToUint8((int)g + (int)rhs.g);
		b = ClampIntToUint8((int)b + (int)rhs.b);
		return *this;
	}

	Pixel& Pixel::operator-=(const Pixel& rhs)
	{
		r = ClampIntToUint8((int)r - (int)rhs.r);
		g = ClampIntToUint8((int)g - (int)rhs.g);
		b = ClampIntToUint8((int)b - (int)rhs.b);
		return *this;
	}

	Pixel& Pixel::operator*=(const Pixel& rhs)
	{
		r = ClampIntToUint8((int)r * (int)rhs.r);
		g = ClampIntToUint8((int)g * (int)rhs.g);
		b = ClampIntToUint8((int)b * (int)rhs.b);
		return *this;
	}

	Pixel& Pixel::operator/=(const Pixel& rhs)
	{
		r = ClampIntToUint8((int)r / (int)rhs.r);
		g = ClampIntToUint8((int)g / (int)rhs.g);
		b = ClampIntToUint8((int)b / (int)rhs.b);
		return *this;
	}

	Pixel Pixel::operator+(const Pixel& rhs) const
	{
		return Pixel(
			ClampIntToUint8((int)r + (int)rhs.r),
			ClampIntToUint8((int)g + (int)rhs.g),
			ClampIntToUint8((int)b + (int)rhs.b)
		);
	}

	Pixel Pixel::operator-(const Pixel& rhs) const
	{
		return Pixel(
			ClampIntToUint8((int)r - (int)rhs.r),
			ClampIntToUint8((int)g - (int)rhs.g),
			ClampIntToUint8((int)b - (int)rhs.b)
		);
	}

	Pixel Pixel::operator*(const Pixel& rhs) const
	{
		return Pixel(
			ClampIntToUint8((int)r * (int)rhs.r),
			ClampIntToUint8((int)g * (int)rhs.g),
			ClampIntToUint8((int)b * (int)rhs.b)
		);
	}

	Pixel Pixel::operator/(const Pixel& rhs) const
	{
		return Pixel(
			ClampIntToUint8((int)r / (int)rhs.r),
			ClampIntToUint8((int)g / (int)rhs.g),
			ClampIntToUint8((int)b / (int)rhs.b)
		);
	}

	bool Pixel::operator==(const Pixel& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b; }
	bool Pixel::operator!=(const Pixel& rhs) const { return r != rhs.r || g != rhs.g || b != rhs.b; }
	bool Pixel::operator>(const Pixel& rhs) const { return r > rhs.r && g > rhs.g && b > rhs.b; }
	bool Pixel::operator<(const Pixel& rhs) const { return r < rhs.r && g < rhs.g && b < rhs.b; }
	bool Pixel::operator>=(const Pixel& rhs) const { return r >= rhs.r && g >= rhs.g && b >= rhs.b; }
	bool Pixel::operator<=(const Pixel& rhs) const { return r <= rhs.r && g <= rhs.g && b <= rhs.b; }

	bool Pixel::operator==(float rhs) const { return r == rhs && g == rhs && b == rhs; }
	bool Pixel::operator!=(float rhs) const { return r != rhs && g != rhs && b != rhs; }
	bool Pixel::operator>(float rhs) const { return r > rhs && g > rhs && b > rhs; }
	bool Pixel::operator<(float rhs) const { return r < rhs && g < rhs && b < rhs; }
	bool Pixel::operator>=(float rhs) const { return r >= rhs && g >= rhs && b >= rhs; }
	bool Pixel::operator<=(float rhs) const { return r <= rhs && g <= rhs && b <= rhs; }

	Pixel Pixel::Float(float r, float g, float b, float a)
	{
		return Pixel(
			uint8_t(r * 255.0f),
			uint8_t(g * 255.0f),
			uint8_t(b * 255.0f),
			uint8_t(a * 255.0f));
	}
}
