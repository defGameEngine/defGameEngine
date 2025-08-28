#pragma once

#ifndef DGE_GRAPHIC_HPP
#define DGE_GRAPHIC_HPP

#include "Pch.hpp"
#include "Texture.hpp"

namespace def
{
	struct Graphic
	{
		Graphic() = default;
		Graphic(std::string_view fileName);
		Graphic(const Vector2i& size);

		~Graphic();

		Texture* texture = nullptr;
		Sprite* sprite = nullptr;

		// Loads an image as a sprite and as a texture via file name
		void Load(std::string_view fileName);

		// Allocates memory for both texture and sprites instances using size
		void Load(const Vector2i& size);

		// Saving the image data on the disk, you can specify file format
		void Save(std::string_view fileName, Sprite::FileType type) const;

		// Transfers data from the RAM (i.e. sprite) to the GPU (i.e. texture)
		void UpdateTexture();
	};
}

#endif
