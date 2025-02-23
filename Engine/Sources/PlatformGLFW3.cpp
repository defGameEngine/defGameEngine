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
			std::cout << "[GLFW Error] Code: "
				<< "0x000" << std::hex << errorCode
				<< ", text: " << description << std::endl;

			GameEngine::s_Engine->Destroy();

			exit(1);
		}
	}

	void PlatformGLFW3::DropCallback(GLFWwindow* window, int pathCount, const char* paths[])
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));

		auto& cache = platform->m_Window->GetDroppedFiles();

		cache.clear();
		cache.reserve(pathCount);

		for (int i = 0; i < pathCount; i++)
			cache[i] = paths[i];
	}

	void PlatformGLFW3::ScrollCallback(GLFWwindow* window, double x, double y)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));
		platform->m_Input->m_ScrollDelta = (int)y;
	}

	void PlatformGLFW3::MousePosCallback(GLFWwindow* window, double x, double y)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));
		const Vector2i& pixelSize = platform->m_Window->m_PixelSize;

		platform->m_Input->m_MousePos.x = (int)x / pixelSize.x;
		platform->m_Input->m_MousePos.y = (int)y / pixelSize.y;
	}

	void PlatformGLFW3::KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));

		int mappedKey = static_cast<int>(InputHandler::s_KeysTable[key]);
		platform->m_Input->m_KeyNewState[mappedKey] = action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	void PlatformGLFW3::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		PlatformGLFW3* platform = static_cast<PlatformGLFW3*>(glfwGetWindowUserPointer(window));
		platform->m_Input->m_MouseNewState[button] = action == GLFW_PRESS || action == GLFW_REPEAT;
	}

	void PlatformGLFW3::Destroy() const
	{
		glfwDestroyWindow(m_NativeWindow);
		glfwTerminate();
	}

	void PlatformGLFW3::SetTitle(const std::string& text) const
	{
		glfwSetWindowTitle(m_NativeWindow, text.c_str());
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
		if (vsync)
			glfwSwapBuffers(m_NativeWindow);
		else
			glFlush();
	}

	void PlatformGLFW3::PollEvents() const
	{
		glfwPollEvents();
		//glfwWaitEvents();
	}

	bool PlatformGLFW3::ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel)
	{
		m_Monitor = glfwGetPrimaryMonitor();

		if (!m_Monitor)
			return false;

		glfwWindowHint(GLFW_DOUBLEBUFFER, vsync ? GLFW_TRUE : GLFW_FALSE);

		const GLFWvidmode* videoMode = glfwGetVideoMode(m_Monitor);

		if (!videoMode)
			return false;

		if (fullscreen)
		{
			windowSize = { videoMode->width, videoMode->height };
			screenSize = windowSize / pixelSize;

			m_NativeWindow = glfwCreateWindow(windowSize.x, windowSize.y, "", m_Monitor, NULL);

			if (!m_NativeWindow)
				return false;

			glfwSetWindowMonitor(
				m_NativeWindow,
				m_Monitor,
				0, 0,
				windowSize.x, windowSize.y,
				videoMode->refreshRate
			);
		}
		else
		{
			m_NativeWindow = glfwCreateWindow(windowSize.x, windowSize.y, "", NULL, NULL);

			if (!m_NativeWindow)
				return false;
		}

		glfwMakeContextCurrent(m_NativeWindow);
		glViewport(0, 0, windowSize.x, windowSize.y);

		glEnable(GL_TEXTURE_2D);

		if (!dirtypixel)
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

		if (vsync)
		{
			glfwSwapInterval(1);
			glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);
		}

		glfwSetWindowUserPointer(m_NativeWindow, this);

		glfwSetDropCallback(m_NativeWindow, DropCallback);
		glfwSetScrollCallback(m_NativeWindow, ScrollCallback);
		glfwSetCursorPosCallback(m_NativeWindow, MousePosCallback);
		glfwSetMouseButtonCallback(m_NativeWindow, MouseButtonCallback);
		glfwSetKeyCallback(m_NativeWindow, KeyboardCallback);

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
}
