#pragma once

#ifndef DGE_PLATFORM_HPP
#define DGE_PLATFORM_HPP

#include "Pch.hpp"
#include "Texture.hpp"
#include "Window.hpp"
#include "InputHandler.hpp"

namespace def
{
	class InputHandler;
	class Window;

	class Platform
	{
	public:
		Platform();
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

		void SetWindow(Window* window);
		void SetInputHandler(InputHandler* input);

	protected:
		Window* m_Window = nullptr;
		InputHandler* m_Input = nullptr;

	};
}

#endif
