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
	// Creates a texture, and loads it to a GPU,
	// and stores its id and size
	struct Texture
	{
		// Describes how vertices are related to each other
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

		// Is used internally to identify a texture
		uint32_t id;

		// Is used internally in drawing routines
		Vector2f uvScale;

		// Size of a texture in screen coordinates
		Vector2i size;

		// Creates a texture from Sprite data and loads it to the GPU
		void Load(Sprite* sprite);

		// Updates already existing texture on the GPU with Sprite data
		void Update(Sprite* sprite);

	private:
		void Construct(Sprite* sprite, bool deleteSprite);

	};

	// It stores a textured polygon and used internally
	// to draw rectangular images and arbitrary coloured polygons
	struct TextureInstance
	{
		TextureInstance();

		const Texture* texture;

		Texture::Structure structure;

		// Number of points in a polygon
		uint32_t points;

		// Colour of each vertex
		std::vector<Pixel> tint;

		// Window coordinates of each vertex
		std::vector<Vector2f> vertices;

		// Texture coordinates of each vertex
		std::vector<Vector2f> uv;
	};
}

#endif
