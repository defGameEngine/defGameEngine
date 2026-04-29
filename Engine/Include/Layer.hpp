/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_LAYER_HPP
#define DGE_LAYER_HPP

#include "Pch.hpp"
#include "Texture.hpp"
#include "Graphic.hpp"
#include "defGameEngine.hpp"

namespace def
{
	class GameEngine;

	struct Layer
	{
		Layer(GameEngine* engine);
		virtual ~Layer();

		virtual bool OnCreate();
		virtual bool OnUpdate(float deltaTime);

		friend class GameEngine;
		friend class Console;

	protected:
		// All textures on the current layer
		std::vector<TextureInstance> textures;

		// Pixel data that will be drawn by default on the current layer
		Graphic* pixels = nullptr;

		// You can also change drawing targets so it becomes easier to populate sprites
		// and textures with some pixel data
		Graphic* target = pixels;

		// Offset from the top-left corner
		Vector2i offset;

		// Size of the layer
		Vector2i size;

		// Each texture on the layer will use this value as a structure
		Texture::Structure textureStructure = Texture::Structure::TRIANGLE_FAN;

		// Each pixel that is being drawn on this layer
		// will use this mode to be drawn
		Pixel::Mode pixelMode = Pixel::Mode::DEFAULT;

		// Specifies the presence of the layer on the screen
		bool visible = true;

		// The layer will be update if this value is true and won't be updated
		// if this value is false
		bool update = true;

		// Tint that only applied to the current laeyer
		Pixel tint = WHITE;

		// Use your custom function so you can change colour of the pixel
		// knowing its current value and its position on the screen
		Pixel (*shader)(const Vector2i&, const Pixel&, const Pixel&) = nullptr;

		GameEngine& context;
	};
}

#endif
