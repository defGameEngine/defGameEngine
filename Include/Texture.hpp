#ifndef DGE_TEXTURE_HPP
#define DGE_TEXTURE_HPP

#include "Vector2D.hpp"
#include "Sprite.hpp"

namespace def
{
	struct Texture
	{
		enum class Structure
		{
			DEFAULT,
			FAN,
			STRIP,
			WIREFRAME
		};

		Texture(Sprite* sprite);
		Texture(std::string_view fileName);

		Uint32 id;

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
		Uint32 points;

		std::vector<Pixel> tint;
		std::vector<Vector2f> vertices;
		std::vector<Vector2f> uv;
	};
}

#endif
