/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_WINDOW_HPP
#define DGE_WINDOW_HPP

#include "Pch.hpp"
#include "Vector2D.hpp"
#include "Platform.hpp"

// Include platform-specific headers to ensure macros and types are available
#if defined(DGE_PLATFORM_GLFW3)
    #include "PlatformGLFW3.hpp"
#elif defined(DGE_PLATFORM_EMSCRIPTEN)
    #include "PlatformEmscripten.hpp"
#endif

namespace def
{
    class Window
    {
    public:
        friend class Platform;
        friend class PlatformGL;
        friend class PlatformGLFW3;
        friend class PlatformEmscripten;
        friend class GameEngine;

    public:
        Window(std::shared_ptr<Platform> platform);

        // Returns the width of the screen in screen coordinates
        int GetScreenWidth() const;

        // Returns the height of the screen in screen coordinates
        int GetScreenHeight() const;

        // Returns the size of the screen in screen coordinates,
        // the one you've specified in Construct method call
        const Vector2i& GetScreenSize() const;

        // Returns the size of the window in window coordinates
        const Vector2i& GetWindowSize() const;

        // Returns 1.0f / GetWindowSize()
        const Vector2f& GetInvertedScreenSize() const;

        // Returns size of each pixel on the screen,
        // the one you've specified in Construct method call
        const Vector2i& GetPixelSize() const;

        // Returns true if the fullscreen mode is enabled
        bool IsFullScreen() const;

        // Returns true if the pixel correction is disabled
        // on OpenGL dependent platforms enabling pixel cohesion
        // is 
        bool IsDirtyPixel() const;

        // Returns true if vertical synchronization is enabled
        bool IsVSync() const;

        // Returns true if the window is focused
        bool IsFocused() const;

        // Sets the name of the application that will appear on the caption
        // of the window
        void SetTitle(const std::string& title);

        // Sets the icon of the application,
        // this method does nothing on the Emscripten platform so you must
        // put the favicon.ico file near the .html
        void SetIcon(const std::string& path);

        // Returns the list of paths to files that were dragged & dropped on the screen,
        // values here remains the same until the next dragging & dropping. By default the vector is empty
        std::vector<std::string>& GetDroppedFiles();

		// Enables or disables vertical synchronization, this method does nothing on the Emscripten platform
		void EnableVSync(bool enable);

		// Enables or disables fullscreen mode, this method does nothing on the Emscripten platform
		void EnableFullscreen(bool enable);

#if defined(DGE_PLATFORM_GLFW3)

        GLFWwindow* GetNativeWindow()
        {
            return ((PlatformGLFW3*)m_Platform.get())->m_NativeWindow;
        }

#elif defined(DGE_PLATFORM_EMSCRIPTEN)

        EGLDisplay& GetNativeWindow()
        {
            return ((PlatformEmscripten*)m_Platform.get())->m_Display;
        }

#endif

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

        std::shared_ptr<Platform> m_Platform;

    };
}

#endif
