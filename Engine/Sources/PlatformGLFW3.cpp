/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "PlatformGLFW3.hpp"
#include "defGameEngine.hpp"

namespace def
{
	PlatformGLFW3::PlatformGLFW3()
	{
		m_NativeWindow = nullptr;
		m_Monitor = nullptr;

		glfwSetErrorCallback(ErrorCallback);
		glfwInit();
	}

	void PlatformGLFW3::ErrorCallback(int errorCode, const char* description)
	{
		if (errorCode != GLFW_INVALID_ENUM)
		{
			std::cout << "[GLFW Error] "
				<< "0x000" << std::hex << errorCode
				<< ' ' << description << std::endl;

			exit(1);
		}
	}

	void PlatformGLFW3::DropCallback(GLFWwindow* _window, int pathCount, const char* paths[])
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(_window));

		if (auto window = platform->m_Window.lock())
		{
			auto& cache = window->GetDroppedFiles();

			cache.clear();
			cache.reserve(pathCount);

			for (int i = 0; i < pathCount; i++)
				cache[i] = paths[i];
		}	
	}

	void PlatformGLFW3::ScrollCallback(GLFWwindow* window, double x, double y)
	{
		(void)x;

		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));

		if (auto input = platform->m_Input.lock())
			input->m_ScrollDelta = (int)y;
	}

	void PlatformGLFW3::MousePosCallback(GLFWwindow* nativeWindow, double x, double y)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(nativeWindow));
		auto input = platform->m_Input.lock();
		auto window = platform->m_Window.lock();

		if (!input || !window) return;

		Vector2i mouse((int)x, (int)y);

		if (platform->m_ViewSize.x > 0 && platform->m_ViewSize.y > 0)
		{
			mouse -= platform->m_ViewPos;
			mouse = mouse.Clamp({ 0, 0 }, platform->m_ViewSize - 1);

			Vector2f scale = platform->m_ViewSize / Vector2f(window->m_ScreenSize * window->m_PixelSize);

			input->m_MousePos = mouse / std::min(scale.x, scale.y) / window->m_PixelSize;
		}
		else
			input->m_MousePos = mouse / window->m_PixelSize;
	}

	void PlatformGLFW3::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));

		if (auto input = platform->m_Input.lock())
		{
			uint8_t mappedKey = static_cast<uint8_t>(InputHandler::s_KeysTable[key]);
			input->m_KeyNewState[mappedKey] = action == GLFW_PRESS || action == GLFW_REPEAT;
		}
	}

	void PlatformGLFW3::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));

		if (auto input = platform->m_Input.lock())
			input->m_MouseNewState[button] = action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	void PlatformGLFW3::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));

		auto win = platform->m_Window.lock();

		if (win)
		{
			platform->UpdateViewport(width, height);

			if (win->m_IsFullScreen)
			{
				platform->m_FullscreenSize.x = width;
				platform->m_FullscreenSize.y = height;
			}
		}

		GameEngine::s_Engine->MainLoop();
	}

	void PlatformGLFW3::WindowPosCallback(GLFWwindow* window, int x, int y)
	{
		GameEngine::s_Engine->MainLoop();
	}

	void PlatformGLFW3::Destroy() const
	{
		glfwTerminate();
	}

	void PlatformGLFW3::SetTitle(const std::string_view text) const
	{
		glfwSetWindowTitle(m_NativeWindow, text.data());
	}

	bool PlatformGLFW3::IsWindowClose() const
	{
		return glfwWindowShouldClose(m_NativeWindow);
	}

	bool PlatformGLFW3::IsWindowFocused() const
	{
		return glfwGetWindowAttrib(m_NativeWindow, GLFW_FOCUSED) == GLFW_TRUE;
	}

	void PlatformGLFW3::FlushScreen(bool vsync) const
	{
		glfwSwapBuffers(m_NativeWindow);
	}

	void PlatformGLFW3::PollEvents() const
	{
		glfwPollEvents();
	}

	bool PlatformGLFW3::ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel)
	{
		m_Monitor = glfwGetPrimaryMonitor();

		if (!m_Monitor)
			return false;

		// I guess it is enabled by default but let's be specific
		glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

		if (glfwGetPlatform() == GLFW_PLATFORM_COCOA)
		{
			// Disabling window resizing fixes all scaling
			// problems on Apple Retina displays
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
			glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
		}

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);

		if (!videoMode)
			return false;

		if (fullscreen)
		{
			// Store monitor resolution for fullscreen rendering, but keep original logical screen/window sizes
			m_FullscreenSize.x = videoMode->width;
			m_FullscreenSize.y = videoMode->height;

			// In fullscreen mode we can enable VSync by setting a hint
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

			m_NativeWindow = glfwCreateWindow(m_FullscreenSize.x, m_FullscreenSize.y, "", m_Monitor, NULL);

			if (!m_NativeWindow)
				return false;

			glfwSetWindowMonitor(
				m_NativeWindow,
				m_Monitor,
				0, 0,
				m_FullscreenSize.x, m_FullscreenSize.y,
				videoMode->refreshRate);
		}
		else
		{
			m_NativeWindow = glfwCreateWindow(windowSize.x, windowSize.y, "", NULL, NULL);

			if (!m_NativeWindow)
				return false;
		}

		glfwMakeContextCurrent(m_NativeWindow);

		glEnable(GL_TEXTURE_2D);

		if (!dirtypixel)
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		glfwSwapInterval(vsync ? 1 : 0);

		glfwSetWindowUserPointer(m_NativeWindow, this);

		glfwSetDropCallback(m_NativeWindow, DropCallback);
		glfwSetScrollCallback(m_NativeWindow, ScrollCallback);
		glfwSetCursorPosCallback(m_NativeWindow, MousePosCallback);
		glfwSetMouseButtonCallback(m_NativeWindow, MouseButtonCallback);
		glfwSetKeyCallback(m_NativeWindow, KeyboardCallback);
		glfwSetFramebufferSizeCallback(m_NativeWindow, FramebufferSizeCallback);
		glfwSetWindowPosCallback(m_NativeWindow, WindowPosCallback);

		return true;
	}

	void PlatformGLFW3::SetIcon(Sprite& icon) const
	{
		GLFWimage img;
		img.width = icon.size.x;
		img.height = icon.size.y;
		img.pixels = (uint8_t*)icon.pixels.data();
		glfwSetWindowIcon(m_NativeWindow, 1, &img);
	}

	void PlatformGLFW3::EnableVSync(bool enable)
	{
		glfwSwapInterval(enable ? 1 : 0);
	}

	void PlatformGLFW3::EnableFullscreen(bool enable)
	{
		auto window = m_Window.lock();
		if (!window) return;

		if (!m_Monitor)
			m_Monitor = glfwGetPrimaryMonitor();

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);
		if (!videoMode) return;

		if (enable)
		{
			m_FullscreenSize.x = videoMode->width;
			m_FullscreenSize.y = videoMode->height;

			glfwSetWindowMonitor(
				m_NativeWindow,
				m_Monitor,
				0, 0,
				m_FullscreenSize.x, m_FullscreenSize.y,
				videoMode->refreshRate);

			window->m_IsFullScreen = true;
		}
		else
		{
			glfwSetWindowMonitor(
				m_NativeWindow,
				nullptr,
				50, 50, // TODO: maybe avoid constants?
				window->m_WindowSize.x, window->m_WindowSize.y,
				GLFW_DONT_CARE);

			window->m_IsFullScreen = false;
		}

		int w, h;
		glfwGetFramebufferSize(m_NativeWindow, &w, &h);

		UpdateViewport(w, h);

		glfwPollEvents();
	}

	void PlatformGLFW3::UpdateViewport(int width, int height)
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

