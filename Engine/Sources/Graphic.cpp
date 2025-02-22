#include "Pch.hpp"
#include "Graphic.hpp"

namespace def
{
	Graphic::Graphic(std::string_view fileName)
	{
		Load(fileName);
	}

	Graphic::Graphic(const Vector2i& size)
	{
		Load(size);
	}

	Graphic::~Graphic()
	{
		delete texture;
		delete sprite;
	}

	void Graphic::Load(std::string_view fileName)
	{
		sprite = new Sprite(fileName);
		texture = new Texture(sprite);
	}

	void Graphic::Load(const Vector2i& size)
	{
		sprite = new Sprite(size);
		texture = new Texture(sprite);
	}

	void Graphic::Save(std::string_view fileName, const Sprite::FileType type) const
	{
		return sprite->Save(fileName, type);
	}

	void Graphic::UpdateTexture()
	{
		texture->Update(sprite);
	}
}
