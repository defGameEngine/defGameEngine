#pragma once

#ifndef DGE_PLATFORM_HPP
#define DGE_PLATFORM_HPP

#include "Pch.hpp"
#include "Texture.hpp"

namespace def
{
	class InputHandler;
	class Window;

	// An abstract class that uses
	// platform specific API for capturing input
	// and for drawing things on the screen
	class Platform
	{
	public:
		Platform();
		virtual ~Platform() = default;

		// Terminates the API
		virtual void Destroy() const = 0;

		// Sets a name for the application
		virtual void SetTitle(const std::string& text) const = 0;

		// Checks if the window closing event has occured
		virtual bool IsWindowClose() const = 0;

		// Checks if the window focus event has occured
		virtual bool IsWindowFocused() const = 0;

		// Clears bo
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

		void SetWindow(std::shared_ptr<Window> window);
		void SetInputHandler(std::shared_ptr<InputHandler> input);

	protected:
		std::weak_ptr<Window> m_Window;
		std::weak_ptr<InputHandler> m_Input;

	};
}

#endif
