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
	Texture::Texture(Sprite* sprite)
	{
		Construct(sprite, false);
	}

	Texture::Texture(std::string_view fileName)
	{
		Construct(new Sprite(fileName), true);
	}

	void Texture::Construct(Sprite* sprite, bool deleteSprite)
	{
		Load(sprite);

		uvScale = 1.0f / Vector2f(sprite->size);
		size = sprite->size;

		if (deleteSprite)
			delete sprite;
	}

	void Texture::Load(Sprite* sprite)
	{
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
			sprite->size.x,
			sprite->size.y,
			0, GL_RGBA,
			GL_UNSIGNED_BYTE,
			sprite->pixels.data()
		);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Update(Sprite* sprite)
	{
		glBindTexture(GL_TEXTURE_2D, id);

		glTexImage2D(
			GL_TEXTURE_2D,
			0, GL_RGBA,
			sprite->size.x,
			sprite->size.y,
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

		uv = { { 0.0f, 0.0f }, { 0.0f, 1.0f }, { 1.0f, 1.0f }, { 1.0f, 0.0f } };
	}
}
