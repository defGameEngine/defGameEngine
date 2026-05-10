/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "PlatformSDL3.hpp"
#include "defGameEngine.hpp"

namespace def
{
	PlatformSDL3::PlatformSDL3(GameEngine* engine) : PlatformGL(engine)
	{
		m_NativeWindow = nullptr;
		m_ContextGL = nullptr;
		m_DisplayID = 0;
		m_WindowID = 0;
		m_WindowShouldClose = false;

		if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
		{
			std::cout << "[SDL3 Error] Failed to initialize: " << SDL_GetError() << std::endl;
			exit(1);
		}
	}

	void PlatformSDL3::Destroy() const
	{
		if (m_ContextGL)
			SDL_GL_DestroyContext(m_ContextGL);

		if (m_NativeWindow)
			SDL_DestroyWindow(m_NativeWindow);

		SDL_Quit();
	}

	void PlatformSDL3::SetTitle(const std::string_view text) const
	{
		SDL_SetWindowTitle(m_NativeWindow, text.data());
	}

	bool PlatformSDL3::IsWindowClose() const
	{
		return m_WindowShouldClose;
	}

	bool PlatformSDL3::IsWindowFocused() const
	{
		SDL_WindowFlags flags = SDL_GetWindowFlags(m_NativeWindow);
		return (flags & SDL_WINDOW_INPUT_FOCUS) != 0;
	}

	void PlatformSDL3::FlushScreen(bool vsync) const
	{
		SDL_GL_SwapWindow(m_NativeWindow);
	}

	void PlatformSDL3::PollEvents()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_EVENT_QUIT:
			case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
				m_WindowShouldClose = true;
			break;

			case SDL_EVENT_DROP_FILE:
			{
				if (auto window = m_Window.lock())
				{
					auto& cache = window->GetDroppedFiles();

					cache.clear();
					cache.resize(1);

					cache[0] = event.drop.data;
				}
			}
			break;

			case SDL_EVENT_MOUSE_WHEEL:
			{
				if (auto input = m_Input.lock())
					input->m_ScrollDelta = (int)event.wheel.y;
			}
			break;

			case SDL_EVENT_MOUSE_MOTION:
			{
				auto input = m_Input.lock();
				auto window = m_Window.lock();

				if (!input || !window)
					return;

				Vector2i mouse((int)event.motion.x, (int)event.motion.y);

				if (m_ViewSize.x > 0 && m_ViewSize.y > 0)
				{
					mouse -= m_ViewPos;
					mouse = mouse.Clamp({ 0, 0 }, m_ViewSize - 1);

					Vector2f scale = m_ViewSize / Vector2f(window->m_ScreenSize * window->m_PixelSize);

					input->m_MousePos = mouse / std::min(scale.x, scale.y) / window->m_PixelSize;
				}
				else
					input->m_MousePos = mouse / window->m_PixelSize;
			}
			break;

			case SDL_EVENT_KEY_DOWN:
			case SDL_EVENT_KEY_UP:
			{
				if (auto input = m_Input.lock())
				{
					Key mappedKey = InputHandler::s_KeysTable[static_cast<int>(event.key.key)];
					input->m_KeyNewState[(int)mappedKey] = event.type == SDL_EVENT_KEY_DOWN;
				}
			}
			break;

			case SDL_EVENT_MOUSE_BUTTON_DOWN:
			case SDL_EVENT_MOUSE_BUTTON_UP:
			{
				if (auto input = m_Input.lock())
				{
					Button mappedKey = InputHandler::s_ButtonsTable[static_cast<int>(event.button.button)];
					input->m_MouseNewState[(int)mappedKey] = event.type == SDL_EVENT_MOUSE_BUTTON_DOWN;
				}
			}
			break;

			case SDL_EVENT_WINDOW_RESIZED:
			case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
			case SDL_EVENT_WINDOW_MOVED:
			{
				switch (event.window.type)
				{
				case SDL_EVENT_WINDOW_RESIZED:
				case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
				{
					UpdateViewport(event.window.data1, event.window.data2);

					auto win = m_Window.lock();
					if (win && win->m_IsFullScreen)
					{
						m_FullscreenSize.x = event.window.data1;
						m_FullscreenSize.y = event.window.data2;
					}

					m_Engine->MainLoop();
				}
				break;

				case SDL_EVENT_WINDOW_MOVED:
					m_Engine->MainLoop();
				break;
				}
			}
			break;

			}
		}
	}

	bool PlatformSDL3::ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel)
	{
		m_DisplayID = SDL_GetPrimaryDisplay();

		if (!m_DisplayID)
			return false;

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		const SDL_DisplayMode* videoMode = SDL_GetCurrentDisplayMode(m_DisplayID);

		if (!videoMode)
			return false;

		uint32_t windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;

		if (fullscreen)
		{
			m_FullscreenSize.x = videoMode->w;
			m_FullscreenSize.y = videoMode->h;

			windowFlags |= SDL_WINDOW_FULLSCREEN;

			m_NativeWindow = SDL_CreateWindow(
				"",
				m_FullscreenSize.x, m_FullscreenSize.y,
				windowFlags
			);
		}
		else
		{
			m_NativeWindow = SDL_CreateWindow(
				"",
				windowSize.x, windowSize.y,
				windowFlags
			);
		}

		if (!m_NativeWindow)
		{
			std::cout << "[SDL3 Error] Failed to create window: " << SDL_GetError() << std::endl;
			return false;
		}

		m_WindowID = SDL_GetWindowID(m_NativeWindow);
		m_ContextGL = SDL_GL_CreateContext(m_NativeWindow);

		if (!m_ContextGL)
		{
			std::cout << "[SDL3 Error] Failed to create GL context: " << SDL_GetError() << std::endl;
			return false;
		}

		glEnable(GL_TEXTURE_2D);

		if (!dirtypixel)
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		SDL_GL_SetSwapInterval(vsync ? 1 : 0);

		int w, h;
		SDL_GetWindowSize(m_NativeWindow, &w, &h);
		UpdateViewport(w, h);

		return true;
	}

	void PlatformSDL3::SetIcon(Sprite& icon) const
	{
		SDL_Surface* surface = SDL_CreateSurfaceFrom(
			icon.size.x, icon.size.y,
			SDL_PIXELFORMAT_RGBA8888,
			icon.pixels.data(),
			icon.size.x * 4
		);

		if (surface)
		{
			SDL_SetWindowIcon(m_NativeWindow, surface);
			SDL_DestroySurface(surface);
		}
	}

	void PlatformSDL3::EnableVSync(bool enable)
	{
		SDL_GL_SetSwapInterval(enable ? 1 : 0);
	}

	void PlatformSDL3::EnableFullscreen(bool enable)
	{
		auto window = m_Window.lock();
		if (!window) return;

		const SDL_DisplayMode* videoMode = SDL_GetCurrentDisplayMode(m_DisplayID);
		if (!videoMode) return;

		if (enable)
		{
			m_FullscreenSize.x = videoMode->w;
			m_FullscreenSize.y = videoMode->h;

			SDL_SetWindowFullscreen(m_NativeWindow, true);
			SDL_SetWindowSize(m_NativeWindow, m_FullscreenSize.x, m_FullscreenSize.y);

			window->m_IsFullScreen = true;
		}
		else
		{
			SDL_SetWindowFullscreen(m_NativeWindow, false);
			SDL_SetWindowSize(m_NativeWindow, window->m_WindowSize.x, window->m_WindowSize.y);
			SDL_SetWindowPosition(m_NativeWindow, 50, 50);

			window->m_IsFullScreen = false;
		}

		int w, h;
		SDL_GetWindowSize(m_NativeWindow, &w, &h);
		UpdateViewport(w, h);

		SDL_PumpEvents();
	}

	void PlatformSDL3::UpdateViewport(int width, int height)
	{
		auto win = m_Window.lock();

		if (!win)
			return;

		float screenAspect = (float)win->m_ScreenSize.x / (float)win->m_ScreenSize.y;
		float windowAspect = (float)width / (float)height;

		Vector2i viewSize;

		if (windowAspect > screenAspect)
		{
			viewSize.y = height;
			viewSize.x = (int)((float)height * screenAspect);
		}
		else
		{
			viewSize.x = width;
			viewSize.y = (int)((float)width / screenAspect);
		}

		m_ViewPos = { (width - viewSize.x) / 2, (height - viewSize.y) / 2 };
		m_ViewSize = viewSize;

		glViewport(m_ViewPos.x, m_ViewPos.y, m_ViewSize.x, m_ViewSize.y);
	}
}