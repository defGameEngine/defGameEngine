#pragma once

#ifndef DGE_VECTOR2D_HPP
#define DGE_VECTOR2D_HPP

#include "Pch.hpp"

namespace def
{
	template <class T>
	struct Vector2D
	{
		static_assert(std::is_arithmetic<T>::value, "Vector2D<T> must be numeric");

		Vector2D() = default;
		Vector2D(const T& x, const T& y) : x(x), y(y) {}

		Vector2D(const Vector2D&) = default;
		Vector2D& operator=(const Vector2D&) = default;

		T x = 0, y = 0;

		inline Vector2D Clamp(const Vector2D& start, const Vector2D& end) const
		{
			return { std::clamp(x, start.x, end.x), std::clamp(y, start.y, end.y) };
		}

		inline Vector2D Lerp(const Vector2D& v, const double t) const
		{
			return { (T)std::lerp(x, v.x, t), (T)std::lerp(y, v.y, t) };
		}

		inline T Distance(const Vector2D& v) const
		{
			return (*this - v).Length();
		}

		inline T DotProduct(const Vector2D& v) const
		{
			return x * v.x + y * v.y;
		}

		inline T CrossProduct(const Vector2D& v) const
		{
			return x * v.y - y * v.x;
		}

		inline T Angle(const Vector2D& v) const
		{
			return acos(DotProduct(v) / (Length() + v.Length()));
		}

		inline T Length2() const
		{
			return x * x + y * y;
		}

		inline T Length() const
		{
			return std::sqrt(x * x + y * y);
		}

		inline T ManhattanDistance(const Vector2D& v) const
		{
			return std::abs(x - v.x) + std::abs(y - v.y);
		}

		inline Vector2D Max(const Vector2D& v) const
		{
			return Vector2D(std::max(x, v.x), std::max(y, v.y));
		}

		inline Vector2D Min(const Vector2D& v) const
		{
			return Vector2D(std::min(x, v.x), std::min(y, v.y));
		}

		inline void Swap(Vector2D& v)
		{
			std::swap(x, v.x);
			std::swap(y, v.y);
		}

		inline Vector2D Normalise() const
		{
			const T n = (T)1 / Length();
			return Vector2D(x * n, y * n);
		}

		inline Vector2D Abs() const
		{
			return Vector2D(std::abs(x), std::abs(y));
		}

		inline Vector2D Perpendicular() const
		{
			return Vector2D(-y, x);
		}

		inline Vector2D Floor() const
		{
			return Vector2D(std::floor(x), std::floor(y));
		}

		inline Vector2D Ceil() const
		{
			return Vector2D(std::ceil(x), std::ceil(y));
		}

		inline Vector2D Round() const
		{
			return Vector2D(std::round(x), std::round(y));
		}

		inline Vector2D Cartesian() const
		{
			return Vector2D(std::cos(y) * x, std::sin(y) * x);
		}

		inline Vector2D Polar() const
		{
			return Vector2D(Length(), atan2(y, x));
		}

		inline std::string ToString() const
		{
			return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
		}

		template <class F>
		inline operator Vector2D<F>() const
		{
			return { static_cast<F>(this->x), static_cast<F>(this->y) };
		}
	};

	typedef Vector2D<int> Vector2i;
	typedef Vector2D<float> Vector2f;
	typedef Vector2D<double> Vector2d;

	template <class T1, class T2>
	Vector2D<T1>& operator+=(Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator-=(Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator*=(Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		v1.x *= v2.x;
		v1.y *= v2.y;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator/=(Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		v1.x /= v2.x;
		v1.y /= v2.y;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator%=(Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		v1.x %= v2.x;
		v1.y %= v2.y;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator+=(Vector2D<T1>& v1, const T2& v2)
	{
		v1.x += v2;
		v1.y += v2;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator-=(Vector2D<T1>& v1, const T2& v2)
	{
		v1.x -= v2;
		v1.y -= v2;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator*=(Vector2D<T1>& v1, const T2& v2)
	{
		v1.x *= v2;
		v1.y *= v2;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator/=(Vector2D<T1>& v1, const T2& v2)
	{
		v1.x /= v2;
		v1.y /= v2;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<T1>& operator%=(Vector2D<T1>& v1, const T2& v2)
	{
		v1.x %= v2;
		v1.y %= v2;
		return v1;
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator+(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1.x + v2.x, v1.y + v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator-(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1.x - v2.x, v1.y - v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator*(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1.x * v2.x, v1.y * v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator/(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1.x / v2.x, v1.y / v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator%(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1.x % v2.x, v1.y % v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator+(const Vector2D<T1>& v1, const T2& v2)
	{
		return Vector2D(v1.x + v2, v1.y + v2);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator-(const Vector2D<T1>& v1, const T2& v2)
	{
		return Vector2D(v1.x - v2, v1.y - v2);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator*(const Vector2D<T1>& v1, const T2& v2)
	{
		return Vector2D(v1.x * v2, v1.y * v2);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator/(const Vector2D<T1>& v1, const T2& v2)
	{
		return Vector2D(v1.x / v2, v1.y / v2);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator+(const T1& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1 + v2.x, v1 + v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator-(const T1& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1 - v2.x, v1 - v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator*(const T1& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1 * v2.x, v1 * v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator/(const T1& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1 / v2.x, v1 / v2.y);
	}

	template <class T1, class T2>
	Vector2D<decltype(T1(1) + T2(1))> operator%(const T1& v1, const Vector2D<T2>& v2)
	{
		return Vector2D(v1 % v2.x, v1 % v2.y);
	}

	template <class T>
	Vector2D<T> operator-(const Vector2D<T>& v)
	{
		return Vector2D(-v.x, -v.y);
	}

	template <class T1, class T2>
	bool operator==(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}

	template <class T1, class T2>
	bool operator<=(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return v1.x <= v2.x && v1.y <= v2.y;
	}

	template <class T1, class T2>
	bool operator>=(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return v1.x >= v2.x && v1.y >= v2.y;
	}

	template <class T1, class T2>
	bool operator<(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return v1.x < v2.x && v1.y < v2.y;
	}

	template <class T1, class T2>
	bool operator>(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return v1.x > v2.x && v1.y > v2.y;
	}

	template <class T1, class T2>
	bool operator!=(const Vector2D<T1>& v1, const Vector2D<T2>& v2)
	{
		return v1.x != v2.x || v1.y != v2.y;
	}
}

#endif
