#include "Pch.hpp"
#include "PlatformGL.hpp"
#include "GLFW/glfw3.h"

namespace def
{
	PlatformGL::PlatformGL()
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
			glTexCoord2f(texInst.uv[i].x, texInst.uv[i].y);
			glVertex2f(texInst.vertices[i].x, texInst.vertices[i].y);
		}

		glEnd();
	}

	void PlatformGL::BindTexture(int id) const
	{
		glBindTexture(GL_TEXTURE_2D, id);
	}

	void PlatformGL::Destroy() const {}
	void PlatformGL::SetTitle(const std::string& text) const {}

	bool PlatformGL::IsWindowClose() const { return false; }
	bool PlatformGL::IsWindowFocused() const { return false; }
	void PlatformGL::FlushScreen(bool vsync) const {}
	void PlatformGL::PollEvents() const {}

	bool PlatformGL::ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) { return false; }
	void PlatformGL::SetIcon(Sprite& icon) const {}
}
