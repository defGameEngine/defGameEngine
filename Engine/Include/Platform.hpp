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
		virtual void SetTitle(const std::string_view text) const = 0;

		// Checks if the window closing event has occured
		virtual bool IsWindowClose() const = 0;

		// Checks if the window focus event has occured
		virtual bool IsWindowFocused() const = 0;

		// Clears backround buffer with a specified colour
		virtual void ClearBuffer(const Pixel& col) const = 0;

		// Is called after calling ClearBuffer and
		// before drawing everything on the screen
		virtual void OnBeforeDraw() = 0;

		// Is called after drawing everything on the screen
		// and before flushing the display buffer
		virtual void OnAfterDraw() = 0;

		// Flushes the display based on the platform
		virtual void FlushScreen(bool vsync) const = 0;

		// Polls events from the event queue
		virtual void PollEvents() const = 0;

		// Draws a quad:
		// 1) polygon vertices: (-1, -1), (1, -1), (-1, 1), (1, 1)
		// 2) texture vertices: (0, 1), (1, 1), (0, 0), (1, 0)
		virtual void DrawQuad(const Pixel& tint) const = 0;

		// Draws a texture on the screen using info from TextureInstance
		virtual void DrawTexture(const TextureInstance& texInst) const = 0;

		// Binds a texture to work with
		virtual void BindTexture(int id) const = 0;

		// Constructs a window or a canvas with specified params
		virtual bool ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) = 0;

		// Sets the icon of the application
		virtual void SetIcon(Sprite& icon) const = 0;

		// Sets window pointer for internal usage
		void SetWindow(std::shared_ptr<Window> window);

		// Sets input handler pointer for internal usage
		void SetInputHandler(std::shared_ptr<InputHandler> input);

	protected:
		std::weak_ptr<Window> m_Window;
		std::weak_ptr<InputHandler> m_Input;

	};
}

#endif
