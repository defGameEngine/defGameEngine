#pragma once

#ifndef DGE_TEXTURE_HPP
#define DGE_TEXTURE_HPP

#include "Pch.hpp"

#ifndef DGE_IGNORE_VECTOR2D
#include "Vector2D.hpp"
#endif

#include "Sprite.hpp"

namespace def
{
	struct Texture
	{
		enum class Structure
		{
			DEFAULT,
			TRIANGLE_FAN,
			TRIANGLE_STRIP,
			LINES,
			LINE_STRIP,
			WIREFRAME
		};

		Texture(Sprite* sprite);
		Texture(std::string_view fileName);

		uint32_t id;

		Vector2f uvScale;
		Vector2i size;

		void Load(Sprite* sprite);
		void Update(Sprite* sprite);

	private:
		void Construct(Sprite* sprite, bool deleteSprite);

	};

	struct TextureInstance
	{
		TextureInstance();

		const Texture* texture;

		Texture::Structure structure;
		uint32_t points;

		std::vector<Pixel> tint;
		std::vector<Vector2f> vertices;
		std::vector<Vector2f> uv;
	};
}

#endif
