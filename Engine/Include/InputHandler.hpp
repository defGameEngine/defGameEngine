#pragma once

#ifndef DGE_INPUT_HANDLER_HPP
#define DGE_INPUT_HANDLER_HPP

#include "Pch.hpp"
#include "Vector2D.hpp"
#include "Platform.hpp"

namespace def
{
    enum class Key
	{
		NONE = -1,

		SPACE, APOSTROPHE, COMMA, MINUS, PERIOD, SLASH,
		K0, K1, K2, K3, K4, K5, K6, K7, K8, K9,
		SEMICOLON, EQUAL,

		A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S,
		T, U, V, W, X, Y, Z,

		LEFT_BRACKET, BACKSLASH, RIGHT_BRACKET,

		ESCAPE, ENTER, TAB, BACKSPACE, INSERT, DEL, RIGHT, LEFT,
		DOWN, UP, PAGE_UP, PAGE_DOWN, HOME, END,
		CAPS_LOCK, SCROLL_LOCK, NUM_LOCK, PRINT_SCREEN, PAUSE,

		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13,
		F14, F15, F16, F17, F18, F19, F20, F21, F22, F23, F24,

		NP_0, NP_1, NP_2, NP_3, NP_4, NP_5, NP_6, NP_7, NP_8,
		NP_9, NP_DECIMAL, NP_DIVIDE, NP_MULTIPLY, NP_SUBTRACT, NP_ADD,
		NP_ENTER, NP_EQUAL,

		LEFT_SHIFT, LEFT_CONTROL, LEFT_ALT, LEFT_SUPER, RIGHT_SHIFT,
		RIGHT_CONTROL, RIGHT_ALT, RIGHT_SUPER, MENU,

		KEYS_COUNT
	};
    
	enum class Button
	{
		LEFT, RIGHT, WHEEL,
		MOUSE4, MOUSE5, MOUSE6,
		MOUSE7, MOUSE8
	};

    struct KeyState
	{
		KeyState();
		KeyState(bool held, bool released, bool pressed);

		bool held;
		bool released;
		bool pressed;
	};

    class InputHandler
    {
	public:
		friend class Platform;
		friend class PlatformGL;
		friend class PlatformGLFW3;
		friend class PlatformEmscripten;
		friend class Console;
		friend class GameEngine;

    public:
        explicit InputHandler(Platform* platform);

		void ClearCapturedText();

        const KeyState& GetKeyState(Key key) const;
        const KeyState& GetButtonState(Button button) const;

		void CaptureText(bool enable);
		bool IsCapturingText() const;

		const Vector2i& GetMousePosition() const;

		int GetMouseX() const;
		int GetMouseY() const;

		int GetScrollDelta() const;

		const std::string& GetCapturedText() const;
		size_t GetCapturedTextCursorPosition() const;

		bool IsCaps() const;

    public:
        static std::unordered_map<Key, std::pair<char, char>> s_KeyboardUS;
		static std::unordered_map<int, Key> s_KeysTable;

	protected:
		void SetCapturedText(const std::string& text);
		void SetCapturedTextCursorPosition(size_t pos);

		void GrabEvents();
		void FlushBuffers();

		void GrabText();

    private:
        static void UpdateState(KeyState* data, bool* newState, bool* oldState, uint8_t count);

    private:
        KeyState m_Keys[static_cast<uint8_t>(Key::KEYS_COUNT)];
        KeyState m_Mouse[8];

        bool m_KeyOldState[static_cast<uint8_t>(Key::KEYS_COUNT)];
        bool m_KeyNewState[static_cast<uint8_t>(Key::KEYS_COUNT)];

        bool m_MouseOldState[8];
        bool m_MouseNewState[8];

        Vector2i m_MousePos;
        int m_ScrollDelta;

		bool m_CaptureText;
		bool m_Caps;

		std::string m_CapturedText;
		size_t m_CapturedTextCursorPos;

		Platform* m_Platform;

    };
}

#endif
