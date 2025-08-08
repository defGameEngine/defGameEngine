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
		// These are supported file formats for loading and saving image data
		enum class FileType { BMP, PNG, JPG, TGA, TGA_RLE };

		// Can be used only within the Sample method
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
		// Creates a sprite of a specified size filled with black pixels
		void Create(const Vector2i& size);

		// Loads an image data from a file
		void Load(std::string_view fileName);

		// Saves an image data to a file
		void Save(std::string_view fileName, FileType type) const;

		// Changes a specific pixel value
		bool SetPixel(int x, int y, const Pixel& col);

		// Changes a specific pixel value
		bool SetPixel(const Vector2i& pos, const Pixel& col);

		// Returns a pixel on a specified coordinates
		Pixel GetPixel(int x, int y, WrapMethod wrap = WrapMethod::NONE) const;

		// Returns a pixel on a specified coordinates
		Pixel GetPixel(const Vector2i& pos, WrapMethod wrap = WrapMethod::NONE) const;

		// Sets every pixel of the image to a specified colour
		void SetPixelData(const Pixel& col);

		// Takes values X and Y from 0 to 1 and applies sampling and wrapping to the pixel at the specified coordinates
		Pixel Sample(float x, float y, SampleMethod sampleMethod, WrapMethod wrapMethod) const;

		// Takes values X and Y from 0 to 1 and applies sampling and wrapping to the pixel at the specified coordinates
		Pixel Sample(const Vector2f& pos, SampleMethod sampleMethod, WrapMethod wrapMethod) const;
	};
}

#endif
