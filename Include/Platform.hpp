#ifndef DGE_PLATFORM_HPP
#define DGE_PLATFORM_HPP

#include "Texture.hpp"

namespace def
{
	class Platform
	{
	public:
		virtual ~Platform() = default;

		virtual void Destroy() const = 0;
		virtual void SetTitle(const std::string& text) const = 0;

		virtual bool IsWindowClose() const = 0;
		virtual bool IsWindowFocused() const = 0;

		virtual void ClearBuffer(const Pixel& col) const = 0;

		virtual void OnBeforeDraw() = 0;
		virtual void OnAfterDraw() = 0;

		virtual void FlushScreen(bool vsync) const = 0;
		virtual void PollEvents() const = 0;

		virtual void DrawQuad(const Pixel& tint) const = 0;
		virtual void DrawTexture(const TextureInstance& texInst) const = 0;

		virtual void BindTexture(int id) const = 0;

		virtual bool ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) = 0;

		virtual void SetIcon(Sprite& icon) const = 0;
	};
}

#endif
