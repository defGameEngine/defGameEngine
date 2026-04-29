/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "Texture.hpp"
#include "defGameEngine.hpp"

#if defined(DGE_PLATFORM_GLFW3)
#include "PlatformGL.hpp"
#elif defined(DGE_PLATFORM_EMSCRIPTEN)
#include "PlatformEmscripten.hpp"
#else
#error Consider defining DGE_PLATFORM_GLFW3 or DGE_PLATFORM_EMSCRIPTEN
#endif

namespace def
{
	Texture::Texture(Sprite* sprite, const Vector2f& pos, const Vector2f& size)
	{
		Construct(sprite, false, pos, size);
	}

	Texture::Texture(std::string_view fileName, const Vector2f& pos, const Vector2f& size)
	{
		Construct(new Sprite(fileName), true, pos, size);
	}

	void Texture::Construct(Sprite* sprite, bool deleteSprite, const Vector2f& customPos, const Vector2f& customSize)
	{
		Load(sprite, customPos, customSize);

		if (deleteSprite)
			delete sprite;
	}

	void Texture::Load(Sprite* sprite, const Vector2f& customPos, const Vector2f& customSize)
	{
		bool isCustomSize = customSize >= Vector2f(0, 0);

		imageSize = sprite->size;
		size = isCustomSize ? customSize : Vector2f(sprite->size);
		uvScale = 1.0f / Vector2f(imageSize);
		pos = customPos / size;

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA,
			imageSize.x,
			imageSize.y,
			0, GL_RGBA,
			GL_UNSIGNED_BYTE,
			sprite->pixels.data()
		);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Update(Sprite* sprite, const Vector2f& customPos, const Vector2f& customSize)
	{
		imageSize = sprite->size;
		uvScale = 1.0f / Vector2f(imageSize);
		size = customSize >= Vector2f(0, 0) ? customSize : Vector2f(sprite->size);
		pos = customPos / size;

		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA,
			size.x,
			size.y,
			0, GL_RGBA,
			GL_UNSIGNED_BYTE,
			sprite->pixels.data()
		);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	TextureInstance::TextureInstance()
	{
		texture = nullptr;

		structure = Texture::Structure::TRIANGLE_FAN;
		points = 0;

		//uv = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };
	}

	void TextureInstance::ConstructUV()
	{
		Vector2f size = texture->size / texture->imageSize;
		uv = { -texture->pos, { -texture->pos.x, -texture->pos.y + size.y }, -texture->pos + size, { -texture->pos.x + size.x, -texture->pos.y } };
	}
}
