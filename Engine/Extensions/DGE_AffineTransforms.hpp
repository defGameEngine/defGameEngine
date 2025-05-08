#ifndef DGE_AFFINE_TRANSFORMS_HPP
#define DGE_AFFINE_TRANSFORMS_HPP

#include "../Include/defGameEngine.hpp"

namespace def
{
	class AffineTransforms
	{
	public:
		AffineTransforms() = default;;
		AffineTransforms(const def::Vector2i& viewArea, const def::Vector2f& pixelScale);
		~AffineTransforms() = default;

	public:
		Vector2f ScreenToWorld(const Vector2f& pos) const;
		Vector2f WorldToScreen(const Vector2f& pos) const;

		Vector2f GetScale() const;
		Vector2f GetOffset() const;

		Vector2f GetOrigin();
		Vector2f GetEnd();

		void SetViewArea(const def::Vector2i& viewArea);
		void SetPixelScale(const Vector2f& scale);
		void SetScale(const Vector2f& scale);
		void SetOffset(const Vector2f& offset);

		void Zoom(float factor, const Vector2f& pos);

		void StartPan(const Vector2f& pos);
		void UpdatePan(const Vector2f& pos);

		bool IsPointVisible(const Vector2f& point);
		bool IsRectVisible(const Vector2f& pos, const Vector2f& size);

	public:
		bool Draw(const Vector2f& pos, Pixel col = WHITE);
		virtual bool Draw(float x, float y, Pixel col = WHITE);

		void DrawLine(const Vector2f& pos1, const Vector2f& pos2, const Pixel& col = WHITE);
		virtual void DrawLine(float x1, float y1, float x2, float y2, const Pixel& col = WHITE);

		void DrawTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col = WHITE);
		virtual void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Pixel& col = WHITE);

		void FillTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col = WHITE);
		virtual void FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Pixel& col = WHITE);

		void DrawRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col = WHITE);
		virtual void DrawRectangle(float x, float y, float sizeX, float sizeY, const Pixel& col = WHITE);

		void FillRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col = WHITE);
		virtual void FillRectangle(float x, float y, float sizeX, float sizeY, const Pixel& col = WHITE);

		void DrawCircle(const Vector2f& pos, float radius, const Pixel& col = WHITE);
		virtual void DrawCircle(float x, float y, float radius, const Pixel& col = WHITE);

		void FillCircle(const Vector2f& pos, float radius, const Pixel& col = WHITE);
		virtual void FillCircle(float x, float y, float radius, const Pixel& col = WHITE);

		void DrawEllipse(const Vector2f& pos, const Vector2f& size, const Pixel& col = WHITE);
		virtual void DrawEllipse(float x, float y, float sizeX, float sizeY, const Pixel& col = WHITE);

		void FillEllipse(const Vector2f& pos, const Vector2f& size, const Pixel& col = WHITE);
		virtual void FillEllipse(float x, float y, float sizeX, float sizeY, const Pixel& col = WHITE);

		void DrawSprite(const Vector2f& pos, const Sprite* sprite);
		virtual void DrawSprite(float x, float y, const Sprite* sprite);

		void DrawPartialSprite(const Vector2f& pos, const Vector2i& filePos, const Vector2i& fileSize, const Sprite* sprite);
		virtual void DrawPartialSprite(float x, float y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite);

		void DrawWireFrameModel(const std::vector<Vector2f>& modelCoordinates, const Vector2f& pos, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);
		virtual void DrawWireFrameModel(const std::vector<Vector2f>& modelCoordinates, float x, float y, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);

		void FillWireFrameModel(const std::vector<Vector2f>& modelCoordinates, const Vector2f& pos, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);
		virtual void FillWireFrameModel(const std::vector<Vector2f>& modelCoordinates, float x, float y, float rotation = 0.0f, float scale = 1.0f, const Pixel& col = WHITE);

		void DrawTexture(const Vector2f& pos, const Texture* tex, const Vector2f& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialTexture(const Vector2f& pos, const Texture* tex, const Vector2f& filePos, const Vector2f& fileSize, const Vector2f& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);

		void DrawWarpedTexture(const std::vector<Vector2f>& points, const Texture* tex, const Pixel& tint = WHITE);

		void DrawRotatedTexture(const Vector2f& pos, const Texture* tex, float rotation, const Vector2f& center = { 0.0f, 0.0f }, const Vector2f& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);
		void DrawPartialRotatedTexture(const Vector2f& pos, const Texture* tex, const Vector2f& filePos, const Vector2f& fileSize, float rotation, const Vector2f& center = { 0.0f, 0.0f }, const Vector2f& scale = { 1.0f, 1.0f }, const Pixel& tint = WHITE);

		void DrawTexturePolygon(const std::vector<Vector2f>& verts, const std::vector<Pixel>& cols, Texture::Structure structure);

		void DrawTextureLine(const Vector2f& pos1, const Vector2f& pos2, const Pixel& col = WHITE);

		void DrawTextureTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col = WHITE);
		void DrawTextureRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col = WHITE);
		void DrawTextureCircle(const Vector2f& pos, float radius, const Pixel& col = WHITE);

		void FillTextureTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col = WHITE);
		void FillTextureRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col = WHITE);
		void FillTextureCircle(const Vector2f& pos, float radius, const Pixel& col = WHITE);

		void GradientTextureTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col1 = WHITE, const Pixel& col2 = WHITE, const Pixel& col3 = WHITE);
		void GradientTextureRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& colTL = WHITE, const Pixel& colTR = WHITE, const Pixel& colBR = WHITE, const Pixel& colBL = WHITE);

		void DrawTextureString(const Vector2f& pos, std::string_view text, const Pixel& col = def::WHITE, const Vector2f& scale = { 1.0f, 1.0f });

	protected:
		Vector2f m_Offset;
		Vector2f m_Scale = { 1.0f, 1.0f };
		Vector2f m_PanPrev;
		Vector2f m_ViewArea;
		Vector2f m_PixelStep = { 1.0f, 1.0f };

		GameEngine* m_Engine = GameEngine::s_Engine;

	};

	class TileAffineTransforms : public AffineTransforms
	{
	public:
		TileAffineTransforms() = default;
		TileAffineTransforms(const def::Vector2i& viewArea, const def::Vector2f& tileSize);
		~TileAffineTransforms() = default;

		Vector2i GetTileOffset() const;

		Vector2i GetTileOrigin();
		Vector2i GetTileEnd();
	};

#ifdef DGE_AFFINE_TRANSFORMS
#undef DGE_AFFINE_TRANSFORMS

	AffineTransforms::AffineTransforms(const def::Vector2i& viewArea, const def::Vector2f& pixelScale)
	{
		m_ViewArea = viewArea;
		SetPixelScale(pixelScale);
	}

	Vector2f AffineTransforms::GetScale() const
	{
		return m_Scale;
	}

	Vector2f AffineTransforms::GetOffset() const
	{
		return m_Offset;
	}

	Vector2f AffineTransforms::GetOrigin()
	{
		return ScreenToWorld({ 0, 0 });
	}

	Vector2f AffineTransforms::GetEnd()
	{
		return ScreenToWorld(m_ViewArea);
	}

	void AffineTransforms::SetViewArea(const def::Vector2i& viewArea)
	{
		m_ViewArea = viewArea;
	}

	void AffineTransforms::SetPixelScale(const Vector2f& scale)
	{
		m_PixelStep = 1.0f / scale;
		m_Scale = scale;
	}

	void AffineTransforms::SetScale(const Vector2f& scale)
	{
		m_Scale = scale;
	}

	void AffineTransforms::SetOffset(const Vector2f& offset)
	{
		m_Offset = offset;
	}

	void AffineTransforms::Zoom(float factor, const Vector2f& pos)
	{
		Vector2f before = ScreenToWorld(pos);
		m_Scale *= factor;
		Vector2f after = ScreenToWorld(pos);

		m_Offset += before - after;
	}

	void AffineTransforms::StartPan(const Vector2f& pos)
	{
		m_PanPrev = pos;
	}

	void AffineTransforms::UpdatePan(const Vector2f& pos)
	{
		m_Offset -= (pos - m_PanPrev) / m_Scale;
		StartPan(pos);
	}

	bool AffineTransforms::IsPointVisible(const Vector2f& point)
	{
		Vector2f p = WorldToScreen(point);
		return p.x >= 0.0f && p.y >= 0.0f && p < m_ViewArea;
	}

	bool AffineTransforms::IsRectVisible(const Vector2f& pos, const Vector2f& size)
	{
		Vector2f p = WorldToScreen(pos);
		Vector2f s = size * m_Scale;

		return p.x + s.x >= 0.0f && p.y + s.y >= 0.0f && p < m_ViewArea;
	}

	bool AffineTransforms::Draw(const Vector2f& pos, Pixel col)
	{
		return m_Engine->Draw(WorldToScreen(pos), col);
	}

	bool AffineTransforms::Draw(float x, float y, Pixel col)
	{
		return Draw({ x, y }, col);
	}

	void AffineTransforms::DrawLine(const Vector2f& pos1, const Vector2f& pos2, const Pixel& col)
	{
		m_Engine->DrawLine(WorldToScreen(pos1), WorldToScreen(pos2), col);
	}

	void AffineTransforms::DrawLine(float x1, float y1, float x2, float y2, const Pixel& col)
	{
		DrawLine({ x1, y1 }, { x2, y2 }, col);
	}

	void AffineTransforms::DrawTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col)
	{
		m_Engine->DrawTriangle(WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3), col);
	}

	void AffineTransforms::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Pixel& col)
	{
		DrawTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, col);
	}

	void AffineTransforms::FillTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col)
	{
		m_Engine->FillTriangle(WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3), col);
	}

	void AffineTransforms::FillTriangle(float x1, float y1, float x2, float y2, float x3, float y3, const Pixel& col)
	{
		FillTriangle({ x1, y1 }, { x2, y2 }, { x3, y3 }, col);
	}

	void AffineTransforms::DrawRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col)
	{
		m_Engine->DrawRectangle(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::DrawRectangle(float x, float y, float sizeX, float sizeY, const Pixel& col)
	{
		DrawRectangle({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::FillRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col)
	{
		m_Engine->FillRectangle(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::FillRectangle(float x, float y, float sizeX, float sizeY, const Pixel& col)
	{
		FillRectangle({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::DrawCircle(const Vector2f& pos, float radius, const Pixel& col)
	{
		m_Engine->DrawCircle(WorldToScreen(pos), (float)radius * m_Scale.x, col);
	}

	void AffineTransforms::DrawCircle(float x, float y, float radius, const Pixel& col)
	{
		DrawCircle({ x, y }, radius, col);
	}

	void AffineTransforms::FillCircle(const Vector2f& pos, float radius, const Pixel& col)
	{
		m_Engine->FillCircle(WorldToScreen(pos), (float)radius * m_Scale.x, col);
	}

	void AffineTransforms::FillCircle(float x, float y, float radius, const Pixel& col)
	{
		FillCircle({ x, y }, radius, col);
	}

	void AffineTransforms::DrawEllipse(const Vector2f& pos, const Vector2f& size, const Pixel& col)
	{
		m_Engine->DrawEllipse(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::DrawEllipse(float x, float y, float sizeX, float sizeY, const Pixel& col)
	{
		DrawEllipse({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::FillEllipse(const Vector2f& pos, const Vector2f& size, const Pixel& col)
	{
		m_Engine->FillEllipse(WorldToScreen(pos), size * m_Scale, col);
	}

	void AffineTransforms::FillEllipse(float x, float y, float sizeX, float sizeY, const Pixel& col)
	{
		FillEllipse({ x, y }, { sizeX, sizeY }, col);
	}

	void AffineTransforms::DrawSprite(const Vector2f& pos, const Sprite* sprite)
	{
		if (IsRectVisible(pos, sprite->size))
		{
			Vector2f size = sprite->size * m_Scale * m_PixelStep;
			Vector2i spriteStart = WorldToScreen(pos);
			Vector2i spriteEnd = WorldToScreen(pos + sprite->size);

			Vector2i screenStart = spriteStart.Max({ 0, 0 });
			Vector2i screenEnd = spriteEnd.Min(m_Engine->GetWindow()->GetScreenSize());

			Vector2f step = 1.0f / size;

			Vector2i pixel;
			for (pixel.y = screenStart.y; pixel.y < screenEnd.y; pixel.y++)
				for (pixel.x = screenStart.x; pixel.x < screenEnd.x; pixel.x++)
				{
					Vector2f sampled = Vector2f(pixel - spriteStart) * step;
					m_Engine->Draw(pixel, sprite->Sample(sampled, Sprite::SampleMethod::LINEAR, Sprite::WrapMethod::NONE));
				}
		}
	}

	void AffineTransforms::DrawSprite(float x, float y, const Sprite* sprite)
	{
		DrawSprite({ x, y }, sprite);
	}

	void AffineTransforms::DrawPartialSprite(const Vector2f& pos, const Vector2i& filePos, const Vector2i& fileSize, const Sprite* sprite)
	{
		Vector2f flFileSize = fileSize;

		if (IsRectVisible(pos, flFileSize * m_Scale))
		{
			Vector2f size = flFileSize * m_Scale * m_PixelStep;
			Vector2f spriteStep = 1.0f / Vector2f(sprite->size);
			Vector2f screenStep = 1.0f / size;

			Vector2i start = WorldToScreen(pos);
			Vector2i end = start + size;

			Vector2i p;
			for (p.y = start.y; p.y < end.y; p.y++)
				for (p.x = start.x; p.x < end.x; p.x++)
				{
					Vector2f sampledPos = (Vector2f(p - start) * screenStep * flFileSize + Vector2f(filePos)) * spriteStep;
					m_Engine->Draw(p, sprite->Sample(sampledPos, Sprite::SampleMethod::LINEAR, Sprite::WrapMethod::NONE));
				}
		}
	}

	void AffineTransforms::DrawPartialSprite(float x, float y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite)
	{
		DrawPartialSprite({ x, y }, { fileX, fileY }, { fileSizeX, fileSizeY }, sprite);
	}

	void AffineTransforms::DrawWireFrameModel(const std::vector<Vector2f>& modelCoordinates, const Vector2f& pos, float rotation, float scale, const Pixel& col)
	{
		std::vector<Vector2f> transformed(modelCoordinates.size());

		std::transform(modelCoordinates.begin(), modelCoordinates.end(), transformed.begin(),
					   [&](const Vector2f& p) { return p * m_Scale; });

		m_Engine->DrawWireFrameModel(transformed, WorldToScreen(pos), rotation, scale, col);
	}

	void AffineTransforms::DrawWireFrameModel(const std::vector<Vector2f>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		DrawWireFrameModel(modelCoordinates, { x, y }, rotation, scale, col);
	}

	void AffineTransforms::FillWireFrameModel(const std::vector<Vector2f>& modelCoordinates, const Vector2f& pos, float rotation, float scale, const Pixel& col)
	{
		std::vector<Vector2f> transformed(modelCoordinates.size());

		std::transform(modelCoordinates.begin(), modelCoordinates.end(), transformed.begin(),
					   [&](const Vector2f& p) { return p * m_Scale; });

		m_Engine->FillWireFrameModel(transformed, WorldToScreen(pos), rotation, scale, col);
	}

	void AffineTransforms::FillWireFrameModel(const std::vector<Vector2f>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		FillWireFrameModel(modelCoordinates, { x, y }, rotation, scale, col);
	}

	void AffineTransforms::DrawTexture(const Vector2f& pos, const Texture* tex, const Vector2f& scale, const Pixel& tint)
	{
		m_Engine->DrawTexture(WorldToScreen(pos), tex, scale * m_Scale * m_PixelStep, tint);
	}

	void AffineTransforms::DrawPartialTexture(const Vector2f& pos, const Texture* tex, const Vector2f& filePos, const Vector2f& fileSize, const Vector2f& scale, const Pixel& tint)
	{
		m_Engine->DrawPartialTexture(WorldToScreen(pos), tex, filePos, fileSize, scale * m_Scale * m_PixelStep, tint);
	}

	void AffineTransforms::DrawWarpedTexture(const std::vector<Vector2f>& points, const Texture* tex, const Pixel& tint)
	{
		std::vector<Vector2f> transformed(points.size());

		std::transform(points.begin(), points.end(), transformed.begin(),
					   [&](const Vector2f& p) { return WorldToScreen(p); });

		m_Engine->DrawWarpedTexture(transformed, tex, tint);
	}

	void AffineTransforms::DrawRotatedTexture(const Vector2f& pos, const Texture* tex, float rotation, const Vector2f& center, const Vector2f& scale, const Pixel& tint)
	{
		m_Engine->DrawRotatedTexture(WorldToScreen(pos), tex, rotation, center, scale * m_Scale * m_PixelStep, tint);
	}

	void AffineTransforms::DrawPartialRotatedTexture(const Vector2f& pos, const Texture* tex, const Vector2f& filePos, const Vector2f& fileSize, float rotation, const Vector2f& center, const Vector2f& scale, const Pixel& tint)
	{
		m_Engine->DrawPartialRotatedTexture(WorldToScreen(pos), tex, filePos, fileSize, rotation, center, scale * m_Scale * m_PixelStep, tint);
	}

	void AffineTransforms::DrawTexturePolygon(const std::vector<Vector2f>& verts, const std::vector<Pixel>& cols, Texture::Structure structure)
	{
		std::vector<Vector2f> transformed(verts.size());

		std::transform(verts.begin(), verts.end(), transformed.begin(),
					   [&](const Vector2f& p) { return WorldToScreen(p); });

		m_Engine->DrawTexturePolygon(transformed, cols, structure);
	}

	void AffineTransforms::DrawTextureLine(const Vector2f& pos1, const Vector2f& pos2, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2 }, { col, col }, Texture::Structure::WIREFRAME);
	}

	void AffineTransforms::DrawTextureTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::WIREFRAME);
	}

	void AffineTransforms::FillTextureTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col, col, col }, Texture::Structure::TRIANGLE_FAN);
	}

	void AffineTransforms::DrawTextureRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::WIREFRAME);
	}

	void AffineTransforms::FillTextureRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col, col, col, col }, Texture::Structure::TRIANGLE_FAN);
	}

	void AffineTransforms::DrawTextureCircle(const Vector2f& pos, float radius, const Pixel& col)
	{
		m_Engine->DrawTextureCircle(WorldToScreen(pos), (float)radius * m_Scale.x * m_PixelStep.x, col);
	}

	void AffineTransforms::FillTextureCircle(const Vector2f& pos, float radius, const Pixel& col)
	{
		m_Engine->FillTextureCircle(WorldToScreen(pos), (float)radius * m_Scale.x * m_PixelStep.x, col);
	}

	void AffineTransforms::GradientTextureTriangle(const Vector2f& pos1, const Vector2f& pos2, const Vector2f& pos3, const Pixel& col1, const Pixel& col2, const Pixel& col3)
	{
		m_Engine->GradientTextureTriangle(
			WorldToScreen(pos1), WorldToScreen(pos2), WorldToScreen(pos3),
			col1, col2, col3
		);
	}

	void AffineTransforms::GradientTextureRectangle(const Vector2f& pos, const Vector2f& size, const Pixel& colTL, const Pixel& colTR, const Pixel& colBR, const Pixel& colBL)
	{
		m_Engine->GradientTextureRectangle(WorldToScreen(pos), size * m_Scale * m_PixelStep, colTL, colTR, colBR, colBL);
	}

	void AffineTransforms::DrawTextureString(const Vector2f& pos, std::string_view text, const Pixel& col, const Vector2f& scale)
	{
		m_Engine->DrawTextureString(WorldToScreen(pos), text, col, scale * m_Scale * m_PixelStep);
	}

	Vector2f AffineTransforms::ScreenToWorld(const Vector2f& pos) const
	{
		return pos / m_Scale + m_Offset;
	}

	Vector2f AffineTransforms::WorldToScreen(const Vector2f& pos) const
	{
		return (pos - m_Offset) * m_Scale;
	}

	TileAffineTransforms::TileAffineTransforms(const def::Vector2i& viewArea, const def::Vector2f& tileSize)
		: AffineTransforms(viewArea, tileSize)
	{}

	Vector2i TileAffineTransforms::GetTileOffset() const
	{
		return (m_Offset - m_Offset.Floor()) * m_Scale;
	}

	Vector2i TileAffineTransforms::GetTileOrigin()
	{
		return ScreenToWorld({ 0, 0 }).Floor();
	}

	Vector2i TileAffineTransforms::GetTileEnd()
	{
		return ScreenToWorld(m_ViewArea).Ceil();
	}

#endif

}

#endif
