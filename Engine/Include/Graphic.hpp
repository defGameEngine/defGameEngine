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

		void Load(std::string_view fileName);
		void Load(const Vector2i& size);
		void Save(std::string_view fileName, Sprite::FileType type) const;

		void UpdateTexture();
	};
}

#endif
