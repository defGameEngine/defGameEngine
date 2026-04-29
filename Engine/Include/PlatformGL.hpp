/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_PLATFORM_GL_HPP
#define DGE_PLATFORM_GL_HPP

#include "Pch.hpp"
#include "Platform.hpp"

namespace def
{
	class PlatformGL : public Platform
	{
	public:
		PlatformGL(GameEngine* engine);
		
		void ClearBuffer(const Pixel& col) const override;

		void OnBeforeDraw() override;
		void OnAfterDraw() override;

		void DrawQuad(const Pixel& tint) const override;
		void DrawTexture(const TextureInstance& texInst) const override;

		void BindTexture(int id) const override;

		void Destroy() const override;
		void SetTitle(const std::string_view text) const override;

		bool IsWindowClose() const override;
		bool IsWindowFocused() const override;

		void FlushScreen(bool vsync) const override;
		void PollEvents() const override;

		bool ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel) override;

		void SetIcon(Sprite& icon) const override;

		void EnableVSync(bool enable) override;

		void EnableFullscreen(bool enable) override;
	};
}

#endif
