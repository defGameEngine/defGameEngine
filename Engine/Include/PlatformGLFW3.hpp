#pragma once

#ifndef DGE_PLATFORM_GLFW3_HPP
#define DGE_PLATFORM_GLFW3_HPP

#include "Pch.hpp"
#include "PlatformGL.hpp"
#include "GLFW/glfw3.h"

namespace def
{
	class PlatformGLFW3 : public PlatformGL
	{
	public:
		PlatformGLFW3();

		friend class GameEngine;

	private:
		GLFWmonitor* m_Monitor;
		GLFWwindow* m_NativeWindow;

	public:
		static void ErrorCallback(int errorCode, const char* description);
		static void DropCallback(GLFWwindow* window, int pathCount, const char* paths[]);
		static void ScrollCallback(GLFWwindow* window, double x, double y);
		static void MousePosCallback(GLFWwindow* window, double x, double y);
		static void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

		void Destroy() const override;
		void SetTitle(const std::string& text) const override;

		bool IsWindowClose() const override;
		bool IsWindowFocused() const override;

		void FlushScreen(bool vsync) const override;
		void PollEvents() const override;

		bool ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;

		void SetIcon(Sprite& icon) const override;
	};
}

#endif
