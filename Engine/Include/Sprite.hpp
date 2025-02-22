#pragma once

#ifndef DGE_SPRITE_HPP
#define DGE_SPRITE_HPP

#include "Pch.hpp"

#ifndef DGE_IGNORE_VECTOR2D
#include "Vector2D.hpp"
#endif

#include "Pixel.hpp"

namespace def
{
	class Sprite
	{
	public:
		enum class FileType { BMP, PNG, JPG, TGA, TGA_RLE };

		enum class SampleMethod { LINEAR, BILINEAR, TRILINEAR };
		enum class WrapMethod { NONE, REPEAT, MIRROR, CLAMP };

		Sprite() = default;
		Sprite(const Vector2i& size);
		Sprite(std::string_view fileName);
		~Sprite();

	public:
		Vector2i size;
		std::vector<Pixel> pixels;

	public:
		void Create(const Vector2i& size);

		void Load(std::string_view fileName);
		void Save(std::string_view fileName, FileType type) const;

		bool SetPixel(int x, int y, const Pixel& col);
		bool SetPixel(const Vector2i& pos, const Pixel& col);

		Pixel GetPixel(int x, int y, WrapMethod wrap = WrapMethod::NONE) const;
		Pixel GetPixel(const Vector2i& pos, WrapMethod wrap = WrapMethod::NONE) const;

		void SetPixelData(const Pixel& col);

		Pixel Sample(float x, float y, SampleMethod sampleMethod, WrapMethod wrapMethod) const;
		Pixel Sample(const Vector2f& pos, SampleMethod sampleMethod, WrapMethod wrapMethod) const;
	};
}

#endif
