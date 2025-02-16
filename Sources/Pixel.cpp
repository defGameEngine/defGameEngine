#include "Pixel.hpp"

#include <algorithm>
#include <cmath>

namespace def
{
	Pixel::Pixel(Uint32 rgba) : rgba_n(rgba)
	{

	}

	Pixel::Pixel(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a)
	{

	}

	Pixel Pixel::Lerp(const Pixel& rhs, float factor) const
	{
		return Pixel(
			(Uint8)std::lerp(r, rhs.r, factor),
			(Uint8)std::lerp(g, rhs.g, factor),
			(Uint8)std::lerp(b, rhs.b, factor),
			(Uint8)std::lerp(a, rhs.a, factor)
		);
	}

	std::string Pixel::ToString() const
	{
		return "(" + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")";
	}

	Pixel& Pixel::operator+=(float rhs)
	{
		r = Uint8(std::clamp((float)r + rhs, 0.0f, 255.0f));
		g = Uint8(std::clamp((float)g + rhs, 0.0f, 255.0f));
		b = Uint8(std::clamp((float)b + rhs, 0.0f, 255.0f));
		return *this;
	}

	Pixel& Pixel::operator-=(float rhs)
	{
		r = Uint8(std::clamp((float)r - rhs, 0.0f, 255.0f));
		g = Uint8(std::clamp((float)g - rhs, 0.0f, 255.0f));
		b = Uint8(std::clamp((float)b - rhs, 0.0f, 255.0f));
		return *this;
	}

	Pixel& Pixel::operator*=(float rhs)
	{
		r = Uint8(std::clamp((float)r * rhs, 0.0f, 255.0f));
		g = Uint8(std::clamp((float)g * rhs, 0.0f, 255.0f));
		b = Uint8(std::clamp((float)b * rhs, 0.0f, 255.0f));
		return *this;
	}

	Pixel& Pixel::operator/=(float rhs)
	{
		r = Uint8(std::clamp((float)r / rhs, 0.0f, 255.0f));
		g = Uint8(std::clamp((float)g / rhs, 0.0f, 255.0f));
		b = Uint8(std::clamp((float)b / rhs, 0.0f, 255.0f));
		return *this;
	}

	Pixel Pixel::operator+(float rhs) const
	{
		return Pixel(
			Uint8(std::clamp((float)r + rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)g + rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)b + rhs, 0.0f, 255.0f))
		);
	}

	Pixel Pixel::operator-(float rhs) const
	{
		return Pixel(
			Uint8(std::clamp((float)r - rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)g - rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)b - rhs, 0.0f, 255.0f))
		);
	}

	Pixel Pixel::operator*(float rhs) const
	{
		return Pixel(
			Uint8(std::clamp((float)r * rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)g * rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)b * rhs, 0.0f, 255.0f))
		);
	}

	Pixel Pixel::operator/(float rhs) const
	{
		return Pixel(
			Uint8(std::clamp((float)r / rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)g / rhs, 0.0f, 255.0f)),
			Uint8(std::clamp((float)b / rhs, 0.0f, 255.0f))
		);
	}

	Pixel& Pixel::operator+=(const Pixel& rhs)
	{
		r = Uint8(std::clamp((int)r + (int)rhs.r, 0, 255));
		g = Uint8(std::clamp((int)g + (int)rhs.g, 0, 255));
		b = Uint8(std::clamp((int)b + (int)rhs.b, 0, 255));
		return *this;
	}

	Pixel& Pixel::operator-=(const Pixel& rhs)
	{
		r = Uint8(std::clamp((int)r - (int)rhs.r, 0, 255));
		g = Uint8(std::clamp((int)g - (int)rhs.g, 0, 255));
		b = Uint8(std::clamp((int)b - (int)rhs.b, 0, 255));
		return *this;
	}

	Pixel& Pixel::operator*=(const Pixel& rhs)
	{
		r = Uint8(std::clamp((int)r * (int)rhs.r, 0, 255));
		g = Uint8(std::clamp((int)g * (int)rhs.g, 0, 255));
		b = Uint8(std::clamp((int)b * (int)rhs.b, 0, 255));
		return *this;
	}

	Pixel& Pixel::operator/=(const Pixel& rhs)
	{
		r = Uint8(std::clamp((int)r / (int)rhs.r, 0, 255));
		g = Uint8(std::clamp((int)g / (int)rhs.g, 0, 255));
		b = Uint8(std::clamp((int)b / (int)rhs.b, 0, 255));
		return *this;
	}

	Pixel Pixel::operator+(const Pixel& rhs) const
	{
		return Pixel(
			Uint8(std::clamp((int)r + (int)rhs.r, 0, 255)),
			Uint8(std::clamp((int)g + (int)rhs.g, 0, 255)),
			Uint8(std::clamp((int)b + (int)rhs.b, 0, 255))
		);
	}

	Pixel Pixel::operator-(const Pixel& rhs) const
	{
		return Pixel(
			Uint8(std::clamp((int)r - (int)rhs.r, 0, 255)),
			Uint8(std::clamp((int)g - (int)rhs.g, 0, 255)),
			Uint8(std::clamp((int)b - (int)rhs.b, 0, 255))
		);
	}

	Pixel Pixel::operator*(const Pixel& rhs) const
	{
		return Pixel(
			Uint8(std::clamp((float)r * (float)rhs.r / 255.0f, 0.0f, 255.0f)),
			Uint8(std::clamp((float)g * (float)rhs.g / 255.0f, 0.0f, 255.0f)),
			Uint8(std::clamp((float)b * (float)rhs.b / 255.0f, 0.0f, 255.0f))
		);
	}

	Pixel Pixel::operator/(const Pixel& rhs) const
	{
		return Pixel(
			Uint8(std::clamp((float)r / (float)rhs.r, 0.0f, 255.0f)),
			Uint8(std::clamp((float)g / (float)rhs.g, 0.0f, 255.0f)),
			Uint8(std::clamp((float)b / (float)rhs.b, 0.0f, 255.0f))
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
		return Pixel(Uint8(r * 255.0f), Uint8(g * 255.0f), Uint8(b * 255.0f), Uint8(a * 255.0f));
	}
}
