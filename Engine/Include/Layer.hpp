#pragma once

#ifndef DGE_LAYER_HPP
#define DGE_LAYER_HPP

#include "Pch.hpp"
#include "Texture.hpp"
#include "Graphic.hpp"

namespace def
{
	struct Layer
	{
		std::vector<TextureInstance> textures;
		Graphic* pixels = nullptr;
		Graphic* target = pixels;

		Vector2i offset;
		Vector2i size;

		Texture::Structure textureStructure = Texture::Structure::TRIANGLE_FAN;
		Pixel::Mode pixelMode = Pixel::Mode::DEFAULT;

		bool visible = true;
		bool update = true;

		Pixel tint = WHITE;

		Pixel (*shader)(const Vector2i&, const Pixel&, const Pixel&) = nullptr;
	};
}

#endif
