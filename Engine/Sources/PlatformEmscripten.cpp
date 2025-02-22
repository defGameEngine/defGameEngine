#include "PlatformEmscripten.hpp"
#include "defGameEngine.hpp"

namespace def
{
	bool PlatformEmscripten::s_IsWindowFocused = false;

	void PlatformEmscripten::Destroy() const
	{
		emscripten_cancel_main_loop();

		eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		eglDestroyContext(m_Display, m_Context);
		eglDestroySurface(m_Display, m_Surface);
		eglTerminate(m_Display);
	}

	void PlatformEmscripten::SetTitle(const std::string& text) const
	{
		emscripten_set_window_title(text.c_str());
	}

	bool PlatformEmscripten::IsWindowClose() const
	{
		return !GameEngine::s_Engine->m_IsAppRunning;
	}

	bool PlatformEmscripten::IsWindowFocused() const
	{
		return PlatformEmscripten::s_IsWindowFocused;
	}

	void PlatformEmscripten::ClearBuffer(const Pixel& col) const
	{
		glClearColor((float)col.r / 255.0f, (float)col.g / 255.0f, (float)col.b / 255.0f, (float)col.a / 255.0f);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void PlatformEmscripten::OnBeforeDraw()
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(m_QuadShader);
		glBindVertexArrayOES(m_VaQuad);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
	}

	void PlatformEmscripten::OnAfterDraw()
	{

	}

	void PlatformEmscripten::FlushScreen(bool vsync) const
	{
		if (vsync)
			eglSwapBuffers(m_Display, m_Surface);
		else
			glFlush();
	}

	void PlatformEmscripten::PollEvents() const
	{

	}

	void PlatformEmscripten::DrawQuad(const Pixel& tint) const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VbQuad);
		Vertex verts[4] = {
			{ { -1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f }, tint },
			{ { +1.0f, -1.0f, 1.0f }, { 1.0f, 1.0f }, tint },
			{ { -1.0f, +1.0f, 1.0f }, { 0.0f, 0.0f }, tint },
			{ { +1.0f, +1.0f, 1.0f }, { 1.0f, 0.0f }, tint },
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, verts, GL_STREAM_DRAW);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	}

	void PlatformEmscripten::DrawTexture(const TextureInstance& texInst) const
	{
		BindTexture(texInst.texture ? texInst.texture->id : 0);

		glBindBuffer(GL_ARRAY_BUFFER, m_VbQuad);

		for (uint32_t i = 0; i < texInst.points; i++)
		{
			m_VertexMemory[i].pos[0] = texInst.vertices[i].x;
			m_VertexMemory[i].pos[1] = texInst.vertices[i].y;
			m_VertexMemory[i].pos[2] = 1.0f;

			m_VertexMemory[i].uv.x = texInst.uv[i].x;
			m_VertexMemory[i].uv.y = texInst.uv[i].y;
			m_VertexMemory[i].col = texInst.tint[i];
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * texInst.points, m_VertexMemory, GL_STREAM_DRAW);

		switch (texInst.structure)
		{
		case Texture::Structure::WIREFRAME: glDrawArrays(GL_LINE_LOOP, 0, texInst.points); break;
		case Texture::Structure::FAN: glDrawArrays(GL_TRIANGLE_FAN, 0, texInst.points); break;
		case Texture::Structure::STRIP: glDrawArrays(GL_TRIANGLE_STRIP, 0, texInst.points); break;
		case Texture::Structure::DEFAULT: glDrawArrays(GL_TRIANGLES, 0, texInst.points); break;
		}
	}

	void PlatformEmscripten::BindTexture(int id) const
	{
		if (id > 0)
			glBindTexture(GL_TEXTURE_2D, id);
		else
			glBindTexture(GL_TEXTURE_2D, m_BlankQuad.texture->id);
	}

	bool PlatformEmscripten::ConstructWindow(Vector2i& screenSize, const Vector2i& pixelSize, Vector2i& windowSize, bool vsync, bool fullscreen, bool dirtypixel)
	{
		emscripten_set_canvas_element_size("#canvas", windowSize.x, windowSize.y);

		const EGLint attributes[] = { EGL_RED_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_BLUE_SIZE, 8, EGL_ALPHA_SIZE, 8, EGL_NONE };
		const EGLint configAttributes[] = { EGL_CONTEXT_CLIENT_VERSION , 2, EGL_NONE };
		EGLint configsCount;

		m_Display = eglGetDisplay(EGL_DEFAULT_DISPLAY);

		eglInitialize(m_Display, NULL, NULL);
		eglChooseConfig(m_Display, attributes, &m_Config, 1, &configsCount);

		m_Context = eglCreateContext(m_Display, m_Config, EGL_NO_CONTEXT, configAttributes);
		m_Surface = eglCreateWindowSurface(m_Display, m_Config, NULL, NULL);

		eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);
		eglSwapInterval(m_Display, vsync ? 1 : 0);

		m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		const GLchar* fragmentShader =
			"#version 300 es\n"
			"precision mediump float;"
			"out vec4 pixel;\n""in vec2 oTex;\n"
			"in vec4 oCol;\n""uniform sampler2D sprTex;\n""void main(){pixel = texture(sprTex, oTex) * oCol;}";

		glShaderSource(m_FragmentShader, 1, &fragmentShader, NULL);
		glCompileShader(m_FragmentShader);

		m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

		const GLchar* vertexShader =
			"#version 300 es\n"
			"precision mediump float;"
			"layout(location = 0) in vec3 aPos;\n""layout(location = 1) in vec2 aTex;\n"
			"layout(location = 2) in vec4 aCol;\n""out vec2 oTex;\n""out vec4 oCol;\n"
			"void main(){ float p = 1.0 / aPos.z; gl_Position = p * vec4(aPos.x, aPos.y, 0.0, 1.0); oTex = p * aTex; oCol = aCol;}";

		glShaderSource(m_VertexShader, 1, &vertexShader, NULL);
		glCompileShader(m_VertexShader);

		m_QuadShader = glCreateProgram();

		glAttachShader(m_QuadShader, m_FragmentShader);
		glAttachShader(m_QuadShader, m_VertexShader);

		glLinkProgram(m_QuadShader);

		glGenBuffers(1, &m_VbQuad);
		glGenVertexArraysOES(1, &m_VaQuad);

		glBindVertexArrayOES(m_VaQuad);
		glBindBuffer(GL_ARRAY_BUFFER, m_VbQuad);

		Vertex vertices[128];
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 128, vertices, GL_STREAM_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArrayOES(0);

		m_BlankQuad.Load({ 1, 1 });
		m_BlankQuad.sprite->pixels[0] = WHITE;
		m_BlankQuad.UpdateTexture();

		emscripten_set_keydown_callback("#canvas", 0, 1, KeyboardCallback);
		emscripten_set_keyup_callback("#canvas", 0, 1, KeyboardCallback);

		emscripten_set_wheel_callback("#canvas", 0, 1, WheelCallback);
		emscripten_set_mousedown_callback("#canvas", 0, 1, MouseCallback);

		emscripten_set_mouseup_callback("#canvas", 0, 1, MouseCallback);
		emscripten_set_mousemove_callback("#canvas", 0, 1, MouseCallback);

		emscripten_set_touchstart_callback("#canvas", 0, 1, TouchCallback);
		emscripten_set_touchmove_callback("#canvas", 0, 1, TouchCallback);
		emscripten_set_touchend_callback("#canvas", 0, 1, TouchCallback);

		emscripten_set_blur_callback("#canvas", 0, 1, FocusCallback);
		emscripten_set_focus_callback("#canvas", 0, 1, FocusCallback);

		return true;
	}

	void PlatformEmscripten::SetIcon(Sprite& icon) const
	{
		// To set icon you need to put an .ico file near the .html file
	}

	void PlatformEmscripten::MainLoop()
	{
		GameEngine::s_Engine->MainLoop();
	}

	EM_BOOL PlatformEmscripten::FocusCallback(int eventType, const EmscriptenFocusEvent* event, void* userData)
	{
		switch (eventType)
		{
		case EMSCRIPTEN_EVENT_FOCUS: s_IsWindowFocused = true; break;
		case EMSCRIPTEN_EVENT_BLUR: s_IsWindowFocused = false; break;
		}

		return EM_FALSE;
	}

	EM_BOOL PlatformEmscripten::KeyboardCallback(int eventType, const EmscriptenKeyboardEvent* event, void* userData)
	{
		GameEngine* e = GameEngine::s_Engine;

		switch (eventType)
		{
		case EMSCRIPTEN_EVENT_KEYDOWN: e->m_KeyNewState[size_t(GameEngine::s_KeysTable[emscripten_compute_dom_pk_code(event->code)])] = true; break;
		case EMSCRIPTEN_EVENT_KEYUP: e->m_KeyNewState[size_t(GameEngine::s_KeysTable[emscripten_compute_dom_pk_code(event->code)])] = false; break;
		}

		return EM_TRUE;
	}

	EM_BOOL PlatformEmscripten::WheelCallback(int eventType, const EmscriptenWheelEvent* event, void* userData)
	{
		if (eventType == EMSCRIPTEN_EVENT_WHEEL)
			GameEngine::s_Engine->m_ScrollDelta = -int(event->deltaY);

		return EM_TRUE;
	}

	EM_BOOL PlatformEmscripten::TouchCallback(int eventType, const EmscriptenTouchEvent* event, void* userData)
	{
		GameEngine* e = GameEngine::s_Engine;

		switch (eventType)
		{
		case EMSCRIPTEN_EVENT_TOUCHMOVE:
		{
			e->m_MousePos.x = event->touches->targetX / e->m_PixelSize.x;
			e->m_MousePos.y = event->touches->targetY / e->m_PixelSize.y;
		}
		break;

		case EMSCRIPTEN_EVENT_TOUCHSTART:
		{
			e->m_MousePos.x = event->touches->targetX;
			e->m_MousePos.y = event->touches->targetY;

			e->m_MouseNewState[0] = true;
		}
		break;

		case EMSCRIPTEN_EVENT_TOUCHEND:
			e->m_MouseNewState[0] = false;
		break;

		}

		return EM_TRUE;
	}

	EM_BOOL PlatformEmscripten::MouseCallback(int eventType, const EmscriptenMouseEvent* event, void* userData)
	{
		GameEngine* e = GameEngine::s_Engine;

		if (eventType == EMSCRIPTEN_EVENT_MOUSEMOVE)
		{
			e->m_MousePos.x = event->targetX / e->m_PixelSize.x;
			e->m_MousePos.y = event->targetY / e->m_PixelSize.y;
		}

		auto check = [&](int button, int index)
			{
				if (event->button == button)
				{
					switch (eventType)
					{
					case EMSCRIPTEN_EVENT_MOUSEDOWN: e->m_MouseNewState[index] = true; break;
					case EMSCRIPTEN_EVENT_MOUSEUP: e->m_MouseNewState[index] = false; break;
					}

					return true;
				}

				return false;
			};

		check(0, 0);
		check(2, 1);
		check(3, 3);
		check(4, 4);

		return check(1, 2) ? EM_TRUE : EM_FALSE;
	}
}
