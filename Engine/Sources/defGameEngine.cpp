#include "Pch.hpp"
#include "defGameEngine.hpp"

namespace def
{
	GameEngine* GameEngine::s_Engine = nullptr;

	GameEngine::GameEngine()
	{
		m_BackgroundColour = { 255, 255, 255, 255 };

		m_TabSize = 0;

		m_DeltaTime = 0.0f;
		m_TickTimer = 0.0f;

		s_Engine = this;

		m_PickedLayer = 0;

		MakeUnitCircle(s_UnitCircle, 64); // TODO: Make 64 (vertices count) as constant

		m_OnlyTextures = false;

	#if defined(DGE_PLATFORM_GLFW3)
		m_Platform = std::make_unique<PlatformGLFW3>();
	#elif defined(DGE_PLATFORM_EMSCRIPTEN)
		m_Platform = std::make_unique<PlatformEmscripten>();
	#else
		#error No platform has been selected
	#endif

		m_Input = std::make_unique<InputHandler>(m_Platform.get());
		m_Window = std::make_unique<def::Window>(m_Platform.get());
		m_Console = std::make_unique<def::Console>();

		m_Platform->SetInputHandler(m_Input.get());
		m_Platform->SetWindow(m_Window.get());
	}

	GameEngine::~GameEngine()
	{
		Destroy();
	}

	void GameEngine::Destroy()
	{
		for (auto& layer : m_Layers)
		{
			if (layer.pixels)
				delete layer.pixels;
		}

		m_Platform->Destroy();
	}

	void GameEngine::MainLoop()
	{
		if (m_IsAppRunning)
		{
			m_TimeEnd = std::chrono::system_clock::now();

			m_DeltaTime = std::chrono::duration<float>(m_TimeEnd - m_TimeStart).count();
			m_TimeStart = m_TimeEnd;

			m_TickTimer += m_DeltaTime;

			if (m_Platform->IsWindowClose())
				m_IsAppRunning = false;

			m_Input->FlushBuffers();
			m_Input->GrabText();

			if (!OnUserUpdate(m_DeltaTime))
				m_IsAppRunning = false;

			m_Console->Draw();

			m_Platform->ClearBuffer(m_BackgroundColour);
			m_Platform->OnBeforeDraw();

			for (auto iter = m_Layers.rbegin(); iter != m_Layers.rend(); iter++)
			{
				if (!m_OnlyTextures)
				{
					if (iter->update)
						iter->pixels->UpdateTexture();

					if (iter->visible)
					{
						m_Platform->BindTexture(iter->pixels->texture->id);
						m_Platform->DrawQuad(iter->tint);
					}
				}

				if (iter->visible)
				{
					for (auto& texture : iter->textures)
						m_Platform->DrawTexture(texture);
				}

				iter->textures.clear();
			}

			if (!OnAfterDraw())
				m_IsAppRunning = false;

			m_Platform->OnAfterDraw();
			m_Window->Flush();
			m_Input->GrabEvents();

		#ifndef DGE_PLATFORM_EMSCRIPTEN
			m_FramesCount++;

			if (m_TickTimer >= 1.0f)
			{
				m_Window->UpdateCaption(m_FramesCount);

				m_TickTimer = 0.0f;
				m_FramesCount = 0;
			}
		#endif
		}
	}

	void GameEngine::MakeUnitCircle(std::vector<Vector2f>& circle, const size_t verts)
	{
		circle.resize(verts);
		float step = 2.0f * 3.14159f / float(verts - 1);

		for (size_t i = 0; i < verts; i++)
		{
			float angle = step * (float)i;

			circle[i].x = cos(angle);
			circle[i].y = sin(angle);
		}
	}

	void GameEngine::Run()
	{
		m_IsAppRunning = OnUserCreate();

		m_TimeStart = std::chrono::system_clock::now();
		m_TimeEnd = m_TimeStart;

	#ifdef DGE_PLATFORM_EMSCRIPTEN
		m_Window->UpdateCaption(-1);
		
		emscripten_set_main_loop(&PlatformEmscripten::MainLoop, 0, 1);
	#else
		m_Window->UpdateCaption(0);
		m_FramesCount = 0;

		while (m_IsAppRunning)
			MainLoop();
	#endif
	}

	bool GameEngine::OnAfterDraw()
	{
		return true;
	}

	void GameEngine::OnTextCapturingComplete(const std::string& text)
	{

	}

	bool GameEngine::OnConsoleCommand(const std::string& command, std::stringstream& output, Pixel& colour)
	{
		return false;
	}

	bool GameEngine::Construct(int screenWidth, int screenHeight, int pixelWidth, int pixelHeight, bool fullScreen, bool vsync, bool dirtyPixel)
	{
		if (!m_Window->Construct(screenWidth, screenHeight, pixelWidth, pixelHeight, fullScreen, vsync, dirtyPixel))
			return false;

		// Console layer, always create it as 0'th layer
		CreateLayer({ 0, 0 }, m_Window->GetScreenSize(), false, false);
		
		m_PickedLayer = CreateLayer({ 0, 0 }, m_Window->GetScreenSize());

		std::string data =
			"?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000"
			"O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400"
			"Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000"
			"720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000"
			"OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000"
			"ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000"
			"Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000"
			"70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000"
			"OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000"
			"00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000"
			"<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000"
			"O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000"
			"00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000"
			"Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0"
			"O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000"
			"?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

		m_Font.Load({ 128, 48 });

		int px = 0;
		int py = 0;

		for (size_t b = 0; b < 1024; b += 4)
		{
			uint32_t sym1 = (uint32_t)data[b + 0] - 48;
			uint32_t sym2 = (uint32_t)data[b + 1] - 48;
			uint32_t sym3 = (uint32_t)data[b + 2] - 48;
			uint32_t sym4 = (uint32_t)data[b + 3] - 48;
			uint32_t r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

			for (int i = 0; i < 24; i++)
			{
				uint8_t k = (r & (1 << i)) ? 255 : 0;

				m_Font.sprite->SetPixel(px, py, { k, k, k, k });

				if (++py == 48)
				{
					px++;
					py = 0;
				}
			}
		}

		m_Font.UpdateTexture();

		return true;
	}

	bool GameEngine::Draw(int x, int y, const Pixel& col)
	{
		Layer& layer = m_Layers[m_PickedLayer];

		if (!layer.target)
			return false;

		Sprite* target = layer.target->sprite;

		switch (layer.pixelMode)
		{
		case Pixel::Mode::CUSTOM:
		return target->SetPixel(x, y, layer.shader({ x, y }, target->GetPixel(x, y), col));

		case Pixel::Mode::DEFAULT:
		return target->SetPixel(x, y, col);

		case Pixel::Mode::MASK:
		{
			if (col.a == 255)
				return target->SetPixel(x, y, col);
		}
		break;

		case Pixel::Mode::ALPHA:
		{
			Pixel d = target->GetPixel(x, y);

			uint8_t r = uint8_t(std::lerp(d.r, col.r, (float)col.a / 255.0f));
			uint8_t g = uint8_t(std::lerp(d.g, col.g, (float)col.a / 255.0f));
			uint8_t b = uint8_t(std::lerp(d.b, col.b, (float)col.a / 255.0f));

			return target->SetPixel(x, y, { r, g, b });
		}

		}

		return false;
	}

	void GameEngine::DrawLine(int x1, int y1, int x2, int y2, const Pixel& col)
	{
		int dx = x2 - x1;
		int dy = y2 - y1;

		int dx1 = abs(dx);
		int dy1 = abs(dy);

		int px = 2 * dy1 - dx1;
		int py = 2 * dx1 - dy1;

		int x, y, xe, ye;

		if (dy1 <= dx1)
		{
			if (dx >= 0)
			{
				x = x1;
				y = y1;
				xe = x2;
			}
			else
			{
				x = x2;
				y = y2;
				xe = x1;
			}

			Draw(x, y, col);

			while (x < xe)
			{
				x++;

				if (px < 0)
					px = px + 2 * dy1;
				else
				{
					y += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ? 1 : -1;
					px = px + 2 * (dy1 - dx1);
				}

				Draw(x, y, col);
			}
		}
		else
		{
			if (dy >= 0)
			{
				x = x1;
				y = y1;
				ye = y2;
			}
			else
			{
				x = x2;
				y = y2;
				ye = y1;
			}

			Draw(x, y, col);

			while (y < ye)
			{
				y++;

				if (py <= 0)
					py = py + 2 * dx1;
				else
				{
					x += ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) ? 1 : -1;
					py = py + 2 * (dx1 - dy1);
				}

				Draw(x, y, col);
			}
		}
	}

	void GameEngine::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col)
	{
		DrawLine(x1, y1, x2, y2, col);
		DrawLine(x2, y2, x3, y3, col);
		DrawLine(x3, y3, x1, y1, col);
	}

	void GameEngine::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, const Pixel& col)
	{
		auto draw_line = [&](int start, int end, int y)
			{
				for (int i = start; i <= end; i++)
					Draw(i, y, col);
			};

		int t1x, t2x, y, minx, maxx, t1xp, t2xp;

		bool changed1 = false;
		bool changed2 = false;

		int signx1, signx2, dx1, dy1, dx2, dy2;
		int e1, e2;

		if (y1 > y2)
		{
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		if (y1 > y3)
		{
			std::swap(y1, y3);
			std::swap(x1, x3);
		}

		if (y2 > y3)
		{
			std::swap(y2, y3);
			std::swap(x2, x3);
		}

		t1x = t2x = x1;
		y = y1;
		dx1 = x2 - x1;

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = y2 - y1;
		dx2 = x3 - x1;

		if (dx2 < 0)
		{
			dx2 = -dx2;
			signx2 = -1;
		}
		else
			signx2 = 1;

		dy2 = y3 - y1;

		if (dy1 > dx1)
		{
			std::swap(dx1, dy1);
			changed1 = true;
		}

		if (dy2 > dx2)
		{
			std::swap(dy2, dx2);
			changed2 = true;
		}

		e2 = dx2 >> 1;

		if (y1 == y2)
			goto next;

		e1 = dx1 >> 1;

		for (int i = 0; i < dx1;)
		{
			t1xp = t2xp = 0;

			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}

			while (i++ < dx1)
			{
				e1 += dy1;

				while (e1 >= dx1)
				{
					e1 -= dx1;

					if (changed1)
						t1xp = signx1;
					else
						goto next1;
				}

				if (changed1)
					break;
				else
					t1x += signx1;
			}

		next1:
			while (1)
			{
				e2 += dy2;

				while (e2 >= dx2)
				{
					e2 -= dx2;

					if (changed2)
						t2xp = signx2;
					else
						goto next2;
				}

				if (changed2)
					break;
				else
					t2x += signx2;
			}

		next2:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;

			draw_line(minx, maxx, y);

			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y++;

			if (y == y2)
				break;
		}

	next:
		dx1 = x3 - x2;

		if (dx1 < 0)
		{
			dx1 = -dx1;
			signx1 = -1;
		}
		else
			signx1 = 1;

		dy1 = y3 - y2;
		t1x = x2;

		if (dy1 > dx1)
		{
			std::swap(dy1, dx1);
			changed1 = true;
		}
		else
			changed1 = false;

		e1 = dx1 >> 1;

		for (int i = 0; i <= dx1; i++)
		{
			t1xp = t2xp = 0;

			if (t1x < t2x)
			{
				minx = t1x;
				maxx = t2x;
			}
			else
			{
				minx = t2x;
				maxx = t1x;
			}

			while (i < dx1)
			{
				e1 += dy1;

				while (e1 >= dx1)
				{
					e1 -= dx1;

					if (changed1)
					{
						t1xp = signx1;
						break;
					}
					else
						goto next3;
				}

				if (changed1)
					break;
				else
					t1x += signx1;

				if (i < dx1) i++;
			}

		next3:
			while (t2x != x3)
			{
				e2 += dy2;

				while (e2 >= dx2)
				{
					e2 -= dx2;

					if (changed2)
						t2xp = signx2;
					else
						goto next4;
				}

				if (changed2)
					break;
				else
					t2x += signx2;
			}

		next4:
			if (minx > t1x) minx = t1x;
			if (minx > t2x) minx = t2x;
			if (maxx < t1x) maxx = t1x;
			if (maxx < t2x) maxx = t2x;

			draw_line(minx, maxx, y);

			if (!changed1)
				t1x += signx1;

			t1x += t1xp;

			if (!changed2)
				t2x += signx2;

			t2x += t2xp;
			y++;

			if (y > y3)
				return;
		}
	}

	void GameEngine::DrawRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		for (int i = 0; i < sizeX; i++)
		{
			Draw(x + i, y, col);
			Draw(x + i, y + sizeY, col);
		}

		for (int i = 0; i < sizeY; i++)
		{
			Draw(x, y + i, col);
			Draw(x + sizeX - 1, y + i, col);
		}
	}

	void GameEngine::FillRectangle(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		for (int i = 0; i < sizeX; i++)
			for (int j = 0; j < sizeY; j++)
				Draw(x + i, y + j, col);
	}

	void GameEngine::DrawCircle(int x, int y, int radius, const Pixel& col)
	{
		int x1 = 0;
		int y1 = radius;
		int p1 = 3 - 2 * radius;

		while (y1 >= x1)
		{
			Draw(x - x1, y - y1, col);
			Draw(x - y1, y - x1, col);
			Draw(x + y1, y - x1, col);
			Draw(x + x1, y - y1, col);
			Draw(x - x1, y + y1, col);
			Draw(x - y1, y + x1, col);
			Draw(x + y1, y + x1, col);
			Draw(x + x1, y + y1, col);

			if (p1 < 0)
			{
				p1 += 4 * x1 + 6;
				x1++;
			}
			else
			{
				p1 += 4 * (x1 - y1) + 10;
				x1++;
				y1--;
			}
		}
	}

	void GameEngine::FillCircle(int x, int y, int radius, const Pixel& col)
	{
		auto draw_line = [&](int start, int end, int y)
			{
				for (int i = start; i <= end; i++)
					Draw(i, y, col);
			};

		int x1 = 0;
		int y1 = radius;
		int p1 = 3 - 2 * radius;

		while (y1 >= x1)
		{
			draw_line(x - x1, x + x1, y - y1);
			draw_line(x - y1, x + y1, y - x1);
			draw_line(x - x1, x + x1, y + y1);
			draw_line(x - y1, x + y1, y + x1);

			if (p1 < 0)
			{
				p1 += 4 * x1 + 6;
				x1++;
			}
			else
			{
				p1 += 4 * (x1 - y1) + 10;
				x1++;
				y1--;
			}
		}
	}

	void GameEngine::DrawEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		int x1 = x + sizeX;
		int y1 = y + sizeY;

		int a = abs(x1 - x);
		int b = abs(y1 - y);
		int b1 = b & 1;

		int dx = 4 * (1 - a) * b * b;
		int dy = 4 * (b1 + 1) * a * a;

		int err = dx + dy + b1 * a * a;

		if (x > x1)
		{
			x = x1;
			x1 += a;
		}

		if (y > y1)
			y = y1;

		y += (b + 1) / 2;
		y1 = y - b1;

		a *= 8 * a;
		b1 = 8 * b * b;

		do
		{
			Draw(x1, y, col);
			Draw(x, y, col);
			Draw(x, y1, col);
			Draw(x1, y1, col);

			int e2 = 2 * err;

			if (e2 <= dy)
			{
				y++;
				y1--;
				err += dy += a;
			}

			if (e2 >= dx || 2 * err > dy)
			{
				x++;
				x1--;
				err += dx += b1;
			}
		} while (x <= x1);

		while (y - y1 < b)
		{
			Draw(x - 1, y, col);
			Draw(x1 + 1, y++, col);
			Draw(x - 1, y1, col);
			Draw(x1 + 1, y1--, col);
		}
	}

	void GameEngine::FillEllipse(int x, int y, int sizeX, int sizeY, const Pixel& col)
	{
		auto draw_line = [&](int start, int end, int y)
			{
				for (int i = start; i <= end; i++)
					Draw(i, y, col);
			};

		int x1 = x + sizeX;
		int y1 = y + sizeY;

		int a = abs(x1 - x);
		int b = abs(y1 - y);
		int b1 = b & 1;

		int dx = 4 * (1 - a) * b * b;
		int dy = 4 * (b1 + 1) * a * a;

		int err = dx + dy + b1 * a * a;

		if (x > x1)
		{
			x = x1;
			x1 += a;
		}

		if (y > y1)
			y = y1;

		y += (b + 1) / 2;
		y1 = y - b1;

		a *= 8 * a;
		b1 = 8 * b * b;

		do
		{
			draw_line(x, x1, y);
			draw_line(x, x1, y1);

			int e2 = 2 * err;

			if (e2 <= dy)
			{
				y++;
				y1--;
				err += dy += a;
			}

			if (e2 >= dx || 2 * err > dy)
			{
				x++;
				x1--;
				err += dx += b1;
			}
		} while (x <= x1);

		while (y - y1 < b)
		{
			draw_line(x - 1, x1 + 1, y++);
			draw_line(x - 1, x1 + 1, y1--);
		}
	}

	void GameEngine::DrawSprite(int x, int y, const Sprite* sprite)
	{
		for (int j = 0; j < sprite->size.y; j++)
			for (int i = 0; i < sprite->size.x; i++)
				Draw(x + i, y + j, sprite->GetPixel(i, j));
	}

	void GameEngine::DrawPartialSprite(int x, int y, int fileX, int fileY, int fileSizeX, int fileSizeY, const Sprite* sprite)
	{
		for (int i = 0, x1 = 0; i < fileSizeX; i++, x1++)
			for (int j = 0, y1 = 0; j < fileSizeY; j++, y1++)
				Draw(x + x1, y + y1, sprite->GetPixel(fileX + i, fileY + j));
	}

	void GameEngine::DrawWarpedTexture(const std::vector<Vector2f>& points, const Texture* tex, const Pixel& tint)
	{
		auto& layer = m_Layers[m_PickedLayer];

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.structure = layer.textureStructure;
		texInst.points = 4;
		texInst.tint = { tint, tint, tint, tint };
		texInst.vertices.resize(texInst.points);
		texInst.uv = { { 0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };

		float rd = ((points[2].x - points[0].x) * (points[3].y - points[1].y) - (points[3].x - points[1].x) * (points[2].y - points[0].y));

		if (rd != 0.0f)
		{
			rd = 1.0f / rd;

			float rn = ((points[3].x - points[1].x) * (points[0].y - points[1].y) - (points[3].y - points[1].y) * (points[0].x - points[1].x)) * rd;
			float sn = ((points[2].x - points[0].x) * (points[0].y - points[1].y) - (points[2].y - points[0].y) * (points[0].x - points[1].x)) * rd;

			Vector2f center;
			if (!(rn < 0.0f || rn > 1.0f || sn < 0.0f || sn > 1.0f))
				center = points[0] + rn * (points[2] - points[0]);

			float d[4];

			for (int i = 0; i < 4; i++)
				d[i] = (points[i] - center).Length();

			const Vector2f& inv = m_Window->GetInvertedScreenSize();

			for (int i = 0; i < 4; i++)
			{
				float q = d[i] == 0.0f ? 1.0f : (d[i] + d[(i + 2) & 3]) / d[(i + 2) & 3];
				texInst.uv[i] *= q;
				texInst.vertices[i] = { (points[i].x * inv.x) * 2.0f - 1.0f, ((points[i].y * inv.y) * 2.0f - 1.0f) * -1.0f };
			}

			layer.textures.push_back(texInst);
		}
	}

	void GameEngine::DrawWireFrameModel(const std::vector<Vector2f>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		size_t verts = modelCoordinates.size();

		std::vector<Vector2f> coordinates(verts);
		float cs = cosf(rotation), sn = sinf(rotation);

		for (size_t i = 0; i < verts; i++)
		{
			coordinates[i].x = (modelCoordinates[i].x * cs - modelCoordinates[i].y * sn) * scale + x;
			coordinates[i].y = (modelCoordinates[i].x * sn + modelCoordinates[i].y * cs) * scale + y;
		}

		for (size_t i = 0; i <= verts; i++)
			DrawLine(coordinates[i % verts], coordinates[(i + 1) % verts], col);
	}

	void GameEngine::FillWireFrameModel(const std::vector<Vector2f>& modelCoordinates, float x, float y, float rotation, float scale, const Pixel& col)
	{
		size_t verts = modelCoordinates.size();

		std::vector<Vector2f> coordinates(verts);
		float cs = cosf(rotation), sn = sinf(rotation);

		for (size_t i = 0; i < verts; i++)
		{
			coordinates[i].x = (modelCoordinates[i].x * cs - modelCoordinates[i].y * sn) * scale + x;
			coordinates[i].y = (modelCoordinates[i].x * sn + modelCoordinates[i].y * cs) * scale + y;
		}

		auto GetAngle = [](const Vector2f& p1, const Vector2f& p2)
			{
				float a = atan2(p2.y, p2.x) - atan2(p1.y, p1.x);
				while (a > 3.14159f) a -= 3.14159f * 2.0f;
				while (a < -3.14159f) a += 3.14159f * 2.0f;
				return a;
			};

		auto PointInPolygon = [&](const Vector2f& p)
			{
				float angle = 0.0f;

				for (size_t i = 0; i < verts; i++)
					angle += GetAngle(coordinates[i] - p, coordinates[(i + 1) % verts] - p);

				return std::abs(angle) >= 3.14159f;
			};

		Vector2f& min = coordinates.front();
		Vector2f& max = coordinates.front();

		for (size_t i = 1; i < verts; i++)
		{
			if (min.x > coordinates[i].x) min.x = coordinates[i].x;
			if (min.y > coordinates[i].y) min.y = coordinates[i].y;

			if (max.x < coordinates[i].x) max.x = coordinates[i].x;
			if (max.y < coordinates[i].y) max.y = coordinates[i].y;
		}

		Vector2f point;
		for (point.x = min.x; point.x < max.x; point.x++)
			for (point.y = min.y; point.y < max.y; point.y++)
			{
				if (PointInPolygon(point))
					Draw(point, col);
			}
	}

	void GameEngine::DrawString(int x, int y, std::string_view s, const Pixel& col, int scaleX, int scaleY)
	{
		int sx = 0;
		int sy = 0;

		for (auto c : s)
		{
			if (c == '\n')
			{
				sx = 0;
				sy += 8 * scaleY;
			}
			else if (c == '\t')
				sx += 8 * m_TabSize * scaleX;
			else
			{
				int ox = (c - 32) % 16;
				int oy = (c - 32) / 16;

				if (scaleX > 1 || scaleY > 1)
				{
					for (int i = 0; i < 8; i++)
						for (int j = 0; j < 8; j++)
						{
							if (m_Font.sprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
							{
								for (int is = 0; is < scaleX; is++)
									for (int js = 0; js < scaleY; js++)
										Draw(x + sx + i * scaleX + is, y + sy + j * scaleY + js, col);
							}
						}
				}
				else
				{
					for (int i = 0; i < 8; i++)
						for (int j = 0; j < 8; j++)
						{
							if (m_Font.sprite->GetPixel(i + ox * 8, j + oy * 8).r > 0)
								Draw(x + sx + i, y + sy + j, col);
						}
				}

				sx += 8 * scaleX;
			}
		}
	}

	void GameEngine::Clear(const Pixel& col)
	{
		m_Layers[m_PickedLayer].target->sprite->SetPixelData(col);
	}

	void GameEngine::SetDrawTarget(Graphic* target)
	{
		m_Layers[m_PickedLayer].target = target ? target : m_Layers[m_PickedLayer].pixels;
		m_Layers[m_PickedLayer].target->UpdateTexture();
	}

	Graphic* GameEngine::GetDrawTarget()
	{
		return m_Layers[m_PickedLayer].target;
	}

	void GameEngine::SetPixelMode(Pixel::Mode pixelMode)
	{
		m_Layers[m_PickedLayer].pixelMode = pixelMode;
	}

	Pixel::Mode GameEngine::GetPixelMode() const
	{
		return m_Layers[m_PickedLayer].pixelMode;
	}

	void GameEngine::SetTextureStructure(Texture::Structure textureStructure)
	{
		m_Layers[m_PickedLayer].textureStructure = textureStructure;
	}

	Texture::Structure GameEngine::GetTextureStructure() const
	{
		return m_Layers[m_PickedLayer].textureStructure;
	}

	bool GameEngine::Draw(const Vector2i& pos, const Pixel& p)
	{
		return Draw(pos.x, pos.y, p);
	}

	void GameEngine::DrawLine(const Vector2i& pos1, const Vector2i& pos2, const Pixel& col)
	{
		DrawLine(pos1.x, pos1.y, pos2.x, pos2.y, col);
	}

	void GameEngine::DrawTexturePolygon(const std::vector<Vector2f>& verts, const std::vector<Pixel>& cols, Texture::Structure structure)
	{
		TextureInstance texInst;

		texInst.texture = nullptr;
		texInst.points = verts.size();
		texInst.structure = structure;

		texInst.tint.resize(verts.size());

		if (cols.size() > 1)
		{
			std::copy(
				cols.begin(),
				cols.end(),
				texInst.tint.begin());
		}
		else
		{
			std::fill(
				texInst.tint.begin(),
				texInst.tint.end(),
				cols[0]);
		}

		texInst.uv.resize(verts.size());
		texInst.vertices.resize(verts.size());

		const Vector2f& inv = m_Window->GetInvertedScreenSize();

		for (size_t i = 0; i < verts.size(); i++)
		{
			texInst.vertices[i].x = verts[i].x * inv.x * 2.0f - 1.0f;
			texInst.vertices[i].y = 1.0f - verts[i].y * inv.y * 2.0f;
		}

		m_Layers[m_PickedLayer].textures.push_back(texInst);
	}

	void GameEngine::DrawTextureLine(const Vector2i& pos1, const Vector2i& pos2, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2 }, { col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::DrawTextureTriangle(const Vector2i& pos1, const Vector2i& pos2, const Vector2i& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::FillTextureTriangle(const Vector2i& pos1, const Vector2i& pos2, const Vector2i& pos3, const Pixel& col)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col }, Texture::Structure::TRIANGLE_FAN);
	}

	void GameEngine::DrawTextureRectangle(const Vector2i& pos, const Vector2i& size, const Pixel& col)
	{
		// Adding 0.25 is a fix for now, I don't know if that can lead to problems on different configurations
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) + 0.25f } }, { col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::FillTextureRectangle(const Vector2i& pos, const Vector2i& size, const Pixel& col)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { col }, Texture::Structure::TRIANGLE_FAN);
	}

	void GameEngine::DrawTextureCircle(const Vector2i& pos, int radius, const Pixel& col)
	{
		std::vector<Vector2f> verts(s_UnitCircle.size());

		for (size_t i = 0; i < verts.size(); i++)
			verts[i] = s_UnitCircle[i] * (float)radius + pos;

		DrawTexturePolygon(verts, { col }, Texture::Structure::WIREFRAME);
	}

	void GameEngine::FillTextureCircle(const Vector2i& pos, int radius, const Pixel& col)
	{
		std::vector<Vector2f> verts(s_UnitCircle.size());

		for (size_t i = 0; i < verts.size(); i++)
			verts[i] = s_UnitCircle[i] * (float)radius + pos;

		DrawTexturePolygon(verts, { col }, Texture::Structure::TRIANGLE_FAN);
	}

	void GameEngine::GradientTextureTriangle(const Vector2i& pos1, const Vector2i& pos2, const Vector2i& pos3, const Pixel& col1, const Pixel& col2, const Pixel& col3)
	{
		DrawTexturePolygon({ pos1, pos2, pos3 }, { col1, col2, col3 }, Texture::Structure::TRIANGLE_FAN);
	}

	void GameEngine::GradientTextureRectangle(const Vector2i& pos, const Vector2i& size, const Pixel& colTL, const Pixel& colTR, const Pixel& colBR, const Pixel& colBL)
	{
		DrawTexturePolygon({ pos, { float(pos.x + size.x), (float)pos.y }, pos + size, { (float)pos.x, float(pos.y + size.y) } }, { colTL, colTR, colBR, colBL }, Texture::Structure::TRIANGLE_FAN);
	}

	void GameEngine::DrawTextureString(const Vector2i& pos, std::string_view text, const Pixel& col, const Vector2f& scale)
	{
		Vector2f p = { 0.0f, 0.0f };

		for (auto c : text)
		{
			if (c == '\n')
			{
				p.x = 0;
				p.y += 8.0f * scale.y;
			}
			else if (c == '\t')
			{
				p.x += 8.0f * float(m_TabSize) * scale.x;
			}
			else
			{
				Vector2f offset((c - 32) % 16, (c - 32) / 16);

				DrawPartialTexture(pos + p, m_Font.texture, offset * 8.0f, { 8.0f, 8.0f }, scale, col);
				p.x += 8.0f * scale.x;
			}
		}
	}

	void GameEngine::DrawTriangle(const Vector2i& pos1, const Vector2i& pos2, const Vector2i& pos3, const Pixel& col)
	{
		DrawTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, col);
	}

	void GameEngine::FillTriangle(const Vector2i& pos1, const Vector2i& pos2, const Vector2i& pos3, const Pixel& col)
	{
		FillTriangle(pos1.x, pos1.y, pos2.x, pos2.y, pos3.x, pos3.y, col);
	}

	void GameEngine::DrawRectangle(const Vector2i& pos, const Vector2i& size, const Pixel& col)
	{
		DrawRectangle(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::FillRectangle(const Vector2i& pos, const Vector2i& size, const Pixel& col)
	{
		FillRectangle(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::DrawCircle(const Vector2i& pos, int radius, const Pixel& col)
	{
		DrawCircle(pos.x, pos.y, radius, col);
	}

	void GameEngine::FillCircle(const Vector2i& pos, int radius, const Pixel& col)
	{
		FillCircle(pos.x, pos.y, radius, col);
	}

	void GameEngine::DrawEllipse(const Vector2i& pos, const Vector2i& size, const Pixel& col)
	{
		DrawEllipse(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::FillEllipse(const Vector2i& pos, const Vector2i& size, const Pixel& col)
	{
		FillEllipse(pos.x, pos.y, size.x, size.y, col);
	}

	void GameEngine::DrawSprite(const Vector2i& pos, const Sprite* spr)
	{
		DrawSprite(pos.x, pos.y, spr);
	}

	void GameEngine::DrawPartialSprite(const Vector2i& pos, const Vector2i& filePos, const Vector2i& fileSize, const Sprite* spr)
	{
		DrawPartialSprite(pos.x, pos.y, filePos.x, filePos.y, fileSize.x, fileSize.y, spr);
	}

	void GameEngine::DrawTexture(const Vector2f& pos, const Texture* tex, const Vector2f& scale, const Pixel& tint)
	{
		auto& layer = m_Layers[m_PickedLayer];

		const Vector2f& inv = m_Window->GetInvertedScreenSize();

		Vector2f pos1 = (pos * inv * 2.0f - 1.0f) * Vector2f(1.0f, -1.0f);
		Vector2f pos2 = pos1 + 2.0f * tex->size * inv * scale * Vector2f(1.0f, -1.0f);

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = layer.textureStructure;
		texInst.tint = { tint, tint, tint, tint };
		texInst.vertices = { pos1, { pos1.x, pos2.y }, pos2, { pos2.x, pos1.y } };

		layer.textures.push_back(texInst);
	}

	void GameEngine::DrawPartialTexture(const Vector2f& pos, const Texture* tex, const Vector2f& filePos, const Vector2f& fileSize, const Vector2f& scale, const Pixel& tint)
	{
		auto& layer = m_Layers[m_PickedLayer];

		const Vector2f& inv = m_Window->GetInvertedScreenSize();

		Vector2f screenPos1 = (pos * inv * 2.0f - 1.0f) * Vector2f(1.0f, -1.0f);
		Vector2f screenPos2 = ((pos + fileSize * scale) * inv * 2.0f - 1.0f) * Vector2f(1.0f, -1.0f);

		const Vector2i& size = m_Window->GetWindowSize();

		Vector2f quantPos1 = (screenPos1 * Vector2f(m_Window->GetWindowSize()) + Vector2f(0.5f, 0.5f)).Floor() / Vector2f(size);
		Vector2f quantPos2 = (screenPos2 * Vector2f(m_Window->GetWindowSize()) + Vector2f(0.5f, -0.5f)).Ceil() / Vector2f(size);

		Vector2f tl = (filePos + 0.0001f) * tex->uvScale;
		Vector2f br = (filePos + fileSize - 0.0001f) * tex->uvScale;

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = layer.textureStructure;
		texInst.tint = { tint, tint, tint, tint };
		texInst.vertices = { quantPos1, { quantPos1.x, quantPos2.y }, quantPos2, { quantPos2.x, quantPos1.y } };
		texInst.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };

		layer.textures.push_back(texInst);
	}

	void GameEngine::DrawRotatedTexture(const Vector2f& pos, const Texture* tex, float rotation, const Vector2f& center, const Vector2f& scale, const Pixel& tint)
	{
		auto& layer = m_Layers[m_PickedLayer];

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = layer.textureStructure;
		texInst.tint = { tint, tint, tint, tint };

		Vector2f denormCenter = center * tex->size;

		texInst.vertices = {
			-denormCenter * scale,
			(Vector2f(0.0f, tex->size.y) - denormCenter) * scale,
			(tex->size - denormCenter) * scale,
			(Vector2f(tex->size.x, 0.0f) - denormCenter) * scale
		};

		const Vector2f& inv = m_Window->GetInvertedScreenSize();

		float c = cos(rotation), s = sin(rotation);
		for (size_t i = 0; i < texInst.points; i++)
		{
			Vector2f offset =
			{
				texInst.vertices[i].x * c - texInst.vertices[i].y * s,
				texInst.vertices[i].x * s + texInst.vertices[i].y * c
			};

			texInst.vertices[i] = pos + offset;
			texInst.vertices[i] = texInst.vertices[i] * inv * 2.0f - 1.0f;
			texInst.vertices[i].y *= -1.0f;
		}

		layer.textures.push_back(texInst);
	}

	void GameEngine::DrawPartialRotatedTexture(const Vector2f& pos, const Texture* tex, const Vector2f& filePos, const Vector2f& fileSize, float rotation, const Vector2f& center, const Vector2f& scale, const Pixel& tint)
	{
		auto& layer = m_Layers[m_PickedLayer];

		TextureInstance texInst;

		texInst.texture = tex;
		texInst.points = 4;
		texInst.structure = layer.textureStructure;
		texInst.tint = { tint, tint, tint, tint };

		Vector2f denormCenter = center * fileSize;

		texInst.vertices = {
			-denormCenter * scale,
			(Vector2f(0.0f, fileSize.y) - denormCenter) * scale,
			(fileSize - denormCenter) * scale,
			(Vector2f(fileSize.x, 0.0f) - denormCenter) * scale
		};

		const Vector2f& inv = m_Window->GetInvertedScreenSize();

		float c = cos(rotation), s = sin(rotation);
		for (size_t i = 0; i < texInst.points; i++)
		{
			Vector2f offset =
			{
				texInst.vertices[i].x * c - texInst.vertices[i].y * s,
				texInst.vertices[i].x * s + texInst.vertices[i].y * c
			};

			texInst.vertices[i] = pos + offset;
			texInst.vertices[i] = texInst.vertices[i] * inv * 2.0f - 1.0f;
			texInst.vertices[i].y *= -1.0f;
		}

		Vector2f tl = filePos * tex->uvScale;
		Vector2f br = tl + fileSize * tex->uvScale;

		texInst.uv = { tl, { tl.x, br.y }, br, { br.x, tl.y } };

		layer.textures.push_back(texInst);
	}

	void GameEngine::DrawWireFrameModel(const std::vector<Vector2f>& modelCoordinates, const Vector2f& pos, float rotation, float scale, const Pixel& col)
	{
		DrawWireFrameModel(modelCoordinates, pos.x, pos.y, rotation, scale, col);
	}

	void GameEngine::FillWireFrameModel(const std::vector<Vector2f>& modelCoordinates, const Vector2f& pos, float rotation, float scale, const Pixel& col)
	{
		FillWireFrameModel(modelCoordinates, pos.x, pos.y, rotation, scale, col);
	}

	void GameEngine::DrawString(const Vector2i& pos, std::string_view text, const Pixel& col, const Vector2i& scale)
	{
		DrawString(pos.x, pos.y, text, col, scale.x, scale.y);
	}

	void GameEngine::ClearTexture(const Pixel& col)
	{
		m_BackgroundColour = col;
	}

	void GameEngine::SetShader(Pixel(*func)(const Vector2i&, const Pixel&, const Pixel&))
	{
		auto& layer = m_Layers[m_PickedLayer];

		layer.shader = func;
		layer.pixelMode = func ? Pixel::Mode::CUSTOM : Pixel::Mode::DEFAULT;
	}

	void GameEngine::UseOnlyTextures(bool enable)
	{
		m_OnlyTextures = enable;
	}

	float GameEngine::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	size_t GameEngine::CreateLayer(const Vector2i& offset, const Vector2i& size, bool update, bool visible, const Pixel& tint)
	{
		Layer layer;
		layer.offset = offset;
		layer.size = size;

		if (!m_OnlyTextures)
			layer.pixels = new Graphic(size);

		layer.update = update;
		layer.visible = visible;
		layer.tint = tint;
		layer.target = layer.pixels;

		m_Layers.push_back(std::move(layer));
		return m_Layers.size() - 1;
	}

	void GameEngine::PickLayer(size_t layer)
	{
		if (layer < m_Layers.size())
			m_PickedLayer = layer;
	}

	size_t GameEngine::GetPickedLayer() const
	{
		return m_PickedLayer;
	}

	Layer* GameEngine::GetLayerByIndex(size_t index)
	{
		return &m_Layers[index];
	}

	Window *const GameEngine::GetWindow()
	{
		return m_Window.get();
	}

	InputHandler *const GameEngine::GetInput()
	{
		return m_Input.get();
	}

	Console *const GameEngine::GetConsole()
	{
		return m_Console.get();
	}

}
