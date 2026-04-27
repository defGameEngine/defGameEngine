/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_TEXTURE_HPP
#define DGE_TEXTURE_HPP

#include "Pch.hpp"

#include "Vector2D.hpp"
#include "Sprite.hpp"

namespace def
{
	// Creates a texture, loads it to a GPU, stores ID and size
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

		Texture(Sprite* sprite, const Vector2f& pos = { 0.0f, 0.0f }, const Vector2f& size = { -1.0f, -1.0f });
		Texture(std::string_view fileName, const Vector2f& pos = { 0.0f, 0.0f }, const Vector2f& size = { -1.0f, -1.0f });

		// Is used internally to identify a texture
		uint32_t id;

		// Is used internally in drawing routines
		Vector2f uvScale;

		// Size of an image content in screen coordinates
		Vector2f imageSize;

		// Size of a texture in screen coordinates
		Vector2f size;

		// Position of an image relative to the texture
		Vector2f pos;

		// Creates a texture from Sprite data and loads it to the GPU
		void Load(Sprite* sprite, const Vector2f& pos = { 0.0f, 0.0f }, const Vector2f& customSize = { -1.0f, -1.0f });

		// Updates already existing texture on the GPU with Sprite data
		void Update(Sprite* sprite, const Vector2f& pos = { 0.0f, 0.0f }, const Vector2f& customSize = { -1.0f, -1.0f });

	private:
		void Construct(Sprite* sprite, bool deleteSprite, const Vector2f& customPos, const Vector2f& customSize);

	};

	// It stores a textured polygon and used internally
	// to draw rectangular images and arbitrary coloured polygons
	struct TextureInstance
	{
		TextureInstance();

		void ConstructUV();

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
