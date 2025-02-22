#include "Pch.hpp"
#include "Window.hpp"

namespace def
{
    Window::Window(Platform* platform) : m_Title("Undefined"), m_Platform(platform)
    {
    }

    bool Window::Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen, bool vsync, bool dirtyPixel)
    {
        m_ScreenSize = { screenWidth, screenHeight };
        m_PixelSize = { pixelWidth, pixelHeight };
        m_WindowSize = m_ScreenSize * m_PixelSize;

        m_InvScreenSize = { 1.0f / (float)screenWidth, 1.0f / (float)screenHeight };

        m_IsFullScreen = fullScreen;
        m_IsVSync = vsync;

        m_IsDirtyPixel = dirtyPixel;

        if (!m_Platform->ConstructWindow(m_ScreenSize, m_PixelSize, m_WindowSize, vsync, fullScreen, dirtyPixel))
		    return false;

        return true;
    }

    void Window::UpdateCaption(int fps)
    {
        if (fps < 0)
            m_Platform->SetTitle("defini7.github.io - defGameEngine - " + m_Title);
        else
            m_Platform->SetTitle("defini7.github.io - defGameEngine - " + m_Title + " - FPS: " + std::to_string(fps));
    }

    void Window::Flush()
    {
        m_Platform->FlushScreen(m_IsVSync);
    }
    
    int Window::GetScreenWidth() const
    {
        return m_ScreenSize.x;
    }

    int Window::GetScreenHeight() const
    {
        return m_ScreenSize.y;
    }

    const Vector2i& Window::GetScreenSize() const
    {
        return m_ScreenSize;
    }

    const Vector2i& Window::GetWindowSize() const
    {
        return m_WindowSize;
    }

    const Vector2f& Window::GetInvertedScreenSize() const
    {
        return m_InvScreenSize;
    }

    const Vector2i& Window::GetPixelSize() const
    {
        return m_PixelSize;
    }

    bool Window::IsFullScreen() const
    {
        return m_IsFullScreen;
    }

    bool Window::IsDirtyPixel() const
    {
        return m_IsDirtyPixel;
    }

    bool Window::IsVSync() const
    {
        return m_IsVSync;
    }

    bool Window::IsFocused() const
    {
        return m_Platform->IsWindowFocused();
    }

    void Window::SetTitle(const std::string& title)
    {
        m_Title = title;
    }

    void Window::SetIcon(const std::string& path)
    {
        Sprite icon(path);
	    m_Platform->SetIcon(icon);
    }

    std::vector<std::string>& Window::GetDroppedFiles()
    {
        return m_DropCache;
    }
}
