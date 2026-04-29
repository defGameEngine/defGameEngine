/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "PlatformGL.hpp"

#if defined(_WIN32)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <GL/gl.h>
#include <GL/glu.h>

#elif defined(__APPLE__) || defined(__MACH__)

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/OpenGL.h>

#elif defined(__linux__) || defined(__unix__)

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#else

#error "Unsupported platform"

#endif

#ifndef GL_MIRRORED_REPEAT
#define GL_MIRRORED_REPEAT 0x8370
#endif

#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

#ifndef GL_CLAMP_TO_BORDER
#define GL_CLAMP_TO_BORDER 0x812D
#endif

namespace def
{
	PlatformGL::PlatformGL(GameEngine* engine) : Platform(engine)
	{
	}

	void PlatformGL::ClearBuffer(const Pixel& col) const
	{
		glClearColor((float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void PlatformGL::OnBeforeDraw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void PlatformGL::OnAfterDraw()
	{
	}

	void PlatformGL::DrawQuad(const Pixel& tint) const
	{
		glBegin(GL_QUADS);
			glColor4ub(tint.r, tint.g, tint.b, tint.a);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, 1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, -1.0f);
		glEnd();
	}

	void PlatformGL::DrawTexture(const TextureInstance& texInst) const
	{
		BindTexture(texInst.texture ? texInst.texture->id : 0);

		if (!texInst.texture)
			glDisable(GL_TEXTURE_2D);

		switch (texInst.structure)
		{
		case Texture::Structure::DEFAULT:		 glBegin(GL_TRIANGLES);		 break;
		case Texture::Structure::TRIANGLE_FAN:	 glBegin(GL_TRIANGLE_FAN);	 break;
		case Texture::Structure::TRIANGLE_STRIP: glBegin(GL_TRIANGLE_STRIP); break;
		case Texture::Structure::LINES:		 	 glBegin(GL_LINES);	 	 	 break;
		case Texture::Structure::LINE_STRIP:	 glBegin(GL_LINE_STRIP); 	 break;
		case Texture::Structure::WIREFRAME:		 glBegin(GL_LINE_LOOP);		 break;
		}

		for (uint32_t i = 0; i < texInst.points; i++)
		{
			glColor4ub(texInst.tint[i].r, texInst.tint[i].g, texInst.tint[i].b, texInst.tint[i].a);

			if (texInst.texture)
				glTexCoord2f(texInst.uv[i].x, texInst.uv[i].y);

			glVertex2f(texInst.vertices[i].x, texInst.vertices[i].y);
		}

		glEnd();

		if (!texInst.texture)
			glEnable(GL_TEXTURE_2D);
	}

	void PlatformGL::BindTexture(int id) const
	{
		glBindTexture(GL_TEXTURE_2D, id);

		switch (m_WrapMethod)
		{
		case Sprite::WrapMethod::NONE:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		break;

		case Sprite::WrapMethod::REPEAT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		break;

		case Sprite::WrapMethod::MIRROR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		break;

		case Sprite::WrapMethod::CLAMP:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		break;

		}

		switch (m_SampleMethod)
		{
		case Sprite::SampleMethod::LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;

		// TODO: Implement TRILINEAR (requires mipmaps), for now just fall to BILINEAR
		case Sprite::SampleMethod::BILINEAR:
		case Sprite::SampleMethod::TRILINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

		}
	}

	void PlatformGL::Destroy() const {}
	void PlatformGL::SetTitle(const std::string_view text) const {}

	bool PlatformGL::IsWindowClose() const { return false; }
	bool PlatformGL::IsWindowFocused() const { return false; }
	void PlatformGL::FlushScreen(bool vsync) const {}
	void PlatformGL::PollEvents() const {}

	bool PlatformGL::ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) { return false; }
	void PlatformGL::SetIcon(Sprite& icon) const {}
	void PlatformGL::EnableVSync(bool enable) {}
	void PlatformGL::EnableFullscreen(bool enable) {}
}
