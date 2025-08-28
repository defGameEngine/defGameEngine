#pragma once

#ifndef DGE_INPUT_HANDLER_HPP
#define DGE_INPUT_HANDLER_HPP

#include "Pch.hpp"
#include "Vector2D.hpp"
#include "Platform.hpp"

namespace def
{
	// You can get a state of each key specified here using GetKeyState method
    enum class Key
	{
		// Not a key, used as a placeholder for non-keys
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

		// A number of the keys in Key enum
		COUNT
	};

	// Key::COUNT casted to uint8_t
	inline static constexpr uint8_t KEYS_COUNT = static_cast<uint8_t>(Key::COUNT);
    
	// You can get a state of each button specified here using GetButtonState method
	enum class Button
	{
		LEFT, RIGHT, WHEEL,
		MOUSE4, MOUSE5, MOUSE6,
		MOUSE7, MOUSE8, COUNT
	};

	// Button::COUNT casted to uint8_t
	inline static constexpr uint8_t BUTTONS_COUNT = static_cast<uint8_t>(Button::COUNT);

	// Holds a state of each key or button (held, released, or pressed)
    struct KeyState
	{
		KeyState();
		KeyState(bool held, bool released, bool pressed);

		bool held;
		bool released;
		bool pressed;
	};

	// Provides an API for capturing user input (mouse, keyboard, touches)
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
        explicit InputHandler(std::shared_ptr<Platform> platform);

		// Erase all captured from the keyboard text
		void ClearCapturedText();

		// Returns a state of a key on your keyboard
        const KeyState& GetKeyState(Key key) const;

		// Returns a state of a button on your mouse or a touch
        const KeyState& GetButtonState(Button button) const;

		// Starts or stops capturing a text from your keyboard
		void CaptureText(bool enable);

		// Returns true if the text is being captured right now
		bool IsCapturingText() const;

		// Returns the mouse position in screen coordinates
		const Vector2i& GetMousePosition() const;

		// Returns mouse X coordinate in screen space
		int GetMouseX() const;

		// Returns mouse Y coordinate in screen space
		int GetMouseY() const;

		// Returns the number of how many scrolls the user did using mouse,
		// positivity and negativity of the value specifies the direction of scrolling
		int GetScrollDelta() const;

		// Returns text that was captured from a keyboard
		const std::string& GetCapturedText() const;

		// Returns the position of the cursor in a being captured text
		size_t GetCapturedTextCursorPosition() const;

		// Returns true if the CAPS key was activated
		bool IsCaps() const;

    public:
		// For now we have only US keyboard layout,
		// here we map a key to its character analogue on the keyboard
        static std::unordered_map<Key, std::pair<char, char>> s_KeyboardUS;

		// Maps the platform specific key constant to the value from the Key enum
		static std::unordered_map<int, Key> s_KeysTable;

	protected:
		// Sets the m_CapturedText field to text
		void SetCapturedText(const std::string& text);

		// Sets the m_CapturedTextCursorPos to pos
		void SetCapturedTextCursorPosition(size_t pos);

		// Polls all events on the current frame
		void GrabEvents();
		
		// Updates states of the keys and buttons buffers
		void FlushBuffers();

		// Handles text input from the keyboard
		void GrabText();

    private:
        static void UpdateState(KeyState* data, bool* newState, bool* oldState, uint8_t count);

    private:
		// Storing states of all possible keys
        KeyState m_Keys[KEYS_COUNT];

		// Storing state of all possible buttons or touches
        KeyState m_Mouse[8];

		// Used for storing the old state of the keys, buttons, and touches
		// so we can distinguish held and pressed states

        bool m_KeyOldState[KEYS_COUNT];
        bool m_KeyNewState[KEYS_COUNT];

        bool m_MouseOldState[8];
        bool m_MouseNewState[8];

		// Storing current mouse position in screen coordinates
        Vector2i m_MousePos;

        int m_ScrollDelta;

		bool m_CaptureText;
		bool m_Caps;

		std::string m_CapturedText;
		size_t m_CapturedTextCursorPos;

		std::shared_ptr<Platform> m_Platform;

    };
}

#endif
