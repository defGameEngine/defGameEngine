#pragma once

#ifndef DGE_WINDOW_HPP
#define DGE_WINDOW_HPP

#include "Pch.hpp"
#include "Vector2D.hpp"
#include "Platform.hpp"

namespace def
{
    enum class WindowState
	{
		NONE,
		MAXIMIZED,
		FOCUSED
	};

    class Window
    {
    public:
        friend class Platform;
        friend class PlatformGL;
        friend class PlatformGLFW3;
        friend class PlatformEmscripten;
        friend class GameEngine;

    public:
        Window(Platform* platform);

        int GetScreenWidth() const;
        int GetScreenHeight() const;

        const Vector2i& GetScreenSize() const;
        const Vector2i& GetWindowSize() const;

        const Vector2f& GetInvertedScreenSize() const;
        const Vector2i& GetPixelSize() const;

        bool IsFullScreen() const;
        bool IsDirtyPixel() const;
        bool IsVSync() const;
        bool IsFocused() const;

        void SetTitle(const std::string& title);
        void SetIcon(const std::string& path);

        std::vector<std::string>& GetDroppedFiles();

    private:
        bool Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen, bool vsync, bool dirtyPixel);

        void UpdateCaption(int fps);
        void Flush();

    private:
        std::string m_Title;

        Vector2i m_WindowSize;
        Vector2i m_ScreenSize;

        Vector2f m_InvScreenSize;
		Vector2i m_PixelSize;

        bool m_IsFullScreen;
		bool m_IsDirtyPixel;
		bool m_IsVSync;

        std::vector<std::string> m_DropCache;

        Platform* m_Platform;

    };
}

#endif
