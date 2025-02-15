#pragma once

#ifndef DGE_PLATFORM_EMSCRIPTEN_HPP
#define DGE_PLATFORM_EMSCRIPTEN_HPP

#include "Platform.hpp"
#include "Graphic.hpp"

#include <EGL/egl.h>
#include <GLES2/gl2.h>

#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2ext.h>

#include <emscripten/emscripten.h>
#include <emscripten/key_codes.h>
#include <emscripten/html5.h>

namespace def
{
	class PlatformEmscripten : public Platform
	{
	public:
		friend class GameEngine;

		virtual void Destroy() const override;
		virtual void SetTitle(const std::string& text) const override;

		virtual bool IsWindowClose() const override;
		virtual bool IsWindowFocused() const override;

		virtual void ClearBuffer(const Pixel& col) const override;

		virtual void OnBeforeDraw() override;
		virtual void OnAfterDraw() override;

		virtual void FlushScreen(bool vsync) const override;
		virtual void PollEvents() const override;

		virtual void DrawQuad(const Pixel& tint) const override;
		virtual void DrawTexture(const TextureInstance& texInst) const override;

		virtual void BindTexture(int id) const override;

		virtual bool ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;

		virtual void SetIcon(Sprite& icon) const override;

		static void MainLoop();

	private:
		static EM_BOOL FocusCallback(int eventType, const EmscriptenFocusEvent* event, void* userData);
		static EM_BOOL KeyboardCallback(int eventType, const EmscriptenKeyboardEvent* event, void* userData);
		static EM_BOOL WheelCallback(int eventType, const EmscriptenWheelEvent* event, void* userData);
		static EM_BOOL TouchCallback(int eventType, const EmscriptenTouchEvent* event, void* userData);
		static EM_BOOL MouseCallback(int eventType, const EmscriptenMouseEvent* event, void* userData);

	public:
		static bool s_IsWindowFocused;

	private:
		EGLDisplay m_Display;
		EGLConfig m_Config;
		EGLContext m_Context;
		EGLSurface m_Surface;

		uint32_t m_FragmentShader = 0;
		uint32_t m_VertexShader = 0;
		uint32_t m_QuadShader = 0;
		uint32_t m_VbQuad = 0;
		uint32_t m_VaQuad = 0;

		struct Vertex
		{
			float pos[3];
			Vector2f uv;
			Pixel col;
		};

		mutable Vertex m_VertexMemory[128];

		Graphic m_BlankQuad;
	};
}

#endif
