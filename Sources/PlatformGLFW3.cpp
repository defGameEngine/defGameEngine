#include "../Include/PlatformGLFW3.hpp"
#include "../Include/defGameEngine.hpp"

#include <iostream>

namespace def
{
	PlatformGLFW3::PlatformGLFW3()
	{
		m_Window = nullptr;
		m_Monitor = nullptr;

		glfwSetErrorCallback(ErrorCallback);
		glfwInit();
	}

	void PlatformGLFW3::ErrorCallback(int errorCode, const char* description)
	{
		if (errorCode != GLFW_INVALID_ENUM)
		{
			std::cout << "[GLFW Error] Code: "
				<< "0x000" << std::hex << errorCode
				<< ", text: " << description << std::endl;

			GameEngine::s_Engine->Destroy();

			exit(1);
		}
	}

	void PlatformGLFW3::DropCallback(GLFWwindow* window, int pathCount, const char* paths[])
	{
		auto& cache = GameEngine::s_Engine->GetDropped();

		cache.clear();
		cache.reserve(pathCount);

		for (int i = 0; i < pathCount; i++)
			cache[i] = paths[i];
	}

	void PlatformGLFW3::ScrollCallback(GLFWwindow* window, double x, double y)
	{
		GameEngine::s_Engine->m_ScrollDelta = y;
	}

	void PlatformGLFW3::MousePosCallback(GLFWwindow* window, double x, double y)
	{
		GameEngine::s_Engine->m_MousePos.x = (int)x / GameEngine::s_Engine->m_PixelSize.x;
		GameEngine::s_Engine->m_MousePos.y = (int)y / GameEngine::s_Engine->m_PixelSize.y;
	}

	void PlatformGLFW3::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		GameEngine::s_Engine->m_KeyNewState[size_t(GameEngine::s_KeysTable[key])] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	}

	void PlatformGLFW3::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		GameEngine::s_Engine->m_MouseNewState[button] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	}

	void PlatformGLFW3::Destroy() const
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
	}

	void PlatformGLFW3::SetTitle(const std::string& text) const
	{
		glfwSetWindowTitle(m_Window, text.c_str());
	}

	bool PlatformGLFW3::IsWindowClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	bool PlatformGLFW3::IsWindowFocused() const
	{
		return glfwGetWindowAttrib(m_Window, GLFW_FOCUSED) == GLFW_TRUE;
	}

	void PlatformGLFW3::FlushScreen(bool vsync) const
	{
		if (vsync)
			glfwSwapBuffers(m_Window);
		else
			glFlush();
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

		if (!vsync)
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);
		if (!videoMode) return false;

		if (fullscreen)
		{
			windowSize = { videoMode->width, videoMode->height };
			screenSize = windowSize / pixelSize;

			m_Window = glfwCreateWindow(windowSize.x, windowSize.y, "", m_Monitor, NULL);
			if (!m_Window) return false;

			glfwSetWindowMonitor(
				m_Window,
				m_Monitor,
				0, 0,
				windowSize.x, windowSize.y,
				videoMode->refreshRate
			);
		}
		else
		{
			m_Window = glfwCreateWindow(windowSize.x, windowSize.y, "", NULL, NULL);

			if (!m_Window)
				return false;
		}

		glfwMakeContextCurrent(m_Window);
		glViewport(0, 0, windowSize.x, windowSize.y);

		glEnable(GL_TEXTURE_2D);

		if (!dirtypixel)
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		if (vsync)
		{
			glfwSwapInterval(1);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		}

		glfwSetDropCallback(m_Window, DropCallback);

		glfwSetScrollCallback(m_Window, ScrollCallback);
		glfwSetCursorPosCallback(m_Window, MousePosCallback);
		glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
		glfwSetKeyCallback(m_Window, KeyboardCallback);

		return true;
	}

	void PlatformGLFW3::SetIcon(Sprite& icon) const
	{
		GLFWimage img;
		img.width = icon.size.x;
		img.height = icon.size.y;
		img.pixels = (uint8_t*)icon.pixels.data();

		glfwSetWindowIcon(m_Window, 1, &img);
	}
}
