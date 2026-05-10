/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_PLATFORM_SDL3_HPP
#define DGE_PLATFORM_SDL3_HPP

#include "Pch.hpp"
#include "PlatformGL.hpp"

#include "SDL3/SDL.h"
#include "SDL3/SDL_error.h"
#include "SDL3/SDL_opengl.h"

namespace def
{
	class PlatformSDL3 : public PlatformGL
	{
	public:
		PlatformSDL3(GameEngine* engine);

		friend class GameEngine;
		friend class Window;

	private:
		SDL_Window* m_NativeWindow;
		SDL_GLContext m_ContextGL;

		SDL_DisplayID m_DisplayID;
		SDL_WindowID m_WindowID;

		Vector2i m_FullscreenSize;

		Vector2i m_ViewPos;
		Vector2i m_ViewSize;

		bool m_WindowShouldClose;

	public:
		void Destroy() const override;
		void SetTitle(const std::string_view text) const override;

		bool IsWindowClose() const override;
		bool IsWindowFocused() const override;

		void FlushScreen(bool vsync) const override;
		void PollEvents() override;

		bool ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;

		void SetIcon(Sprite& icon) const override;

		void EnableVSync(bool enable) override;
		void EnableFullscreen(bool enable) override;

	private:
		void UpdateViewport(int width, int height);

	};
}

#endif
