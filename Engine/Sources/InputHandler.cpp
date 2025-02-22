#include "Pch.hpp"
#include "InputHandler.hpp"
#include "defGameEngine.hpp"

namespace def
{
    std::unordered_map<Key, std::pair<char, char>> InputHandler::s_KeyboardUS =
    {
        { Key::SPACE, { ' ', ' ' } }, { Key::APOSTROPHE, { '\'', '"' } },
        { Key::COMMA, { ',', '<' } }, { Key::MINUS, { '-', '_' } },
        { Key::PERIOD, { '.', '>' } }, { Key::SLASH, { '/', '?' } },
        { Key::K0, { '0', ')' } }, { Key::K1, { '1', '!' } },
        { Key::K2, { '2', '@' } }, { Key::K3, { '3', '#' } },
        { Key::K4, { '4', '$' } }, { Key::K5, { '5', '%' } },
        { Key::K6, { '6', '^' } }, { Key::K7, { '7', '&' } },
        { Key::K8, { '8', '*' } }, { Key::K9, { '9', '(' } },
        { Key::SEMICOLON, { ';', ':' } }, { Key::EQUAL, { '=', '+' } },
        { Key::A, { 'a', 'A' } }, { Key::B, { 'b', 'B' } },
        { Key::C, { 'c', 'C' } }, { Key::D, { 'd', 'D' } },
        { Key::E, { 'e', 'E' } }, { Key::F, { 'f', 'F' } },
        { Key::G, { 'g', 'G' } }, { Key::H, { 'h', 'H' } },
        { Key::I, { 'i', 'I' } }, { Key::J, { 'j', 'J' } },
        { Key::K, { 'k', 'K' } }, { Key::L, { 'l', 'L' } },
        { Key::M, { 'm', 'M' } }, { Key::N, { 'n', 'N' } },
        { Key::O, { 'o', 'O' } }, { Key::P, { 'p', 'P' } },
        { Key::Q, { 'q', 'Q' } }, { Key::R, { 'r', 'R' } },
        { Key::S, { 's', 'S' } }, { Key::T, { 't', 'T' } },
        { Key::U, { 'u', 'U' } }, { Key::V, { 'v', 'V' } },
        { Key::W, { 'w', 'W' } }, { Key::X, { 'x', 'X' } },
        { Key::Y, { 'y', 'Y' } }, { Key::Z, { 'z', 'Z' } },
        { Key::LEFT_BRACKET, { '[', '{' } }, { Key::BACKSLASH, { '\\', '|' } },
        { Key::RIGHT_BRACKET, { ']', '}' } },
        { Key::NP_0, { '0', '0' } }, { Key::NP_1, { '1', '1' } },
        { Key::NP_2, { '2', '2' } }, { Key::NP_3, { '3', '3' } },
        { Key::NP_4, { '4', '4' } }, { Key::NP_5, { '5', '5' } },
        { Key::NP_6, { '6', '6' } }, { Key::NP_7, { '7', '7' } },
        { Key::NP_8, { '8', '8' } }, { Key::NP_9, { '9', '9' } },
        { Key::NP_DIVIDE, { '/', '/' } }, { Key::NP_MULTIPLY, { '*', '*' } },
        { Key::NP_SUBTRACT, { '-', '-' } }, { Key::NP_ADD, { '+', '+' } },
        { Key::NP_EQUAL, { '=', '+' } }
    };

#ifdef DGE_PLATFORM_GLFW3

    std::unordered_map<int, Key> InputHandler::s_KeysTable =
    {
        { GLFW_KEY_SPACE, Key::SPACE }, { GLFW_KEY_APOSTROPHE, Key::APOSTROPHE }, { GLFW_KEY_COMMA, Key::COMMA },
        { GLFW_KEY_MINUS, Key::MINUS }, { GLFW_KEY_PERIOD, Key::PERIOD }, { GLFW_KEY_SLASH, Key::SLASH },

        { GLFW_KEY_0, Key::K0 }, { GLFW_KEY_1, Key::K1 }, { GLFW_KEY_2, Key::K2 }, { GLFW_KEY_3, Key::K3 }, { GLFW_KEY_4, Key::K4 },
        { GLFW_KEY_5, Key::K5 }, { GLFW_KEY_6, Key::K6 }, { GLFW_KEY_7, Key::K7 }, { GLFW_KEY_8, Key::K8 }, { GLFW_KEY_9, Key::K9 },

        { GLFW_KEY_SEMICOLON, Key::SEMICOLON }, { GLFW_KEY_EQUAL, Key::EQUAL },

        { GLFW_KEY_A, Key::A }, { GLFW_KEY_B, Key::B }, { GLFW_KEY_C, Key::C }, { GLFW_KEY_D, Key::D },
        { GLFW_KEY_E, Key::E }, { GLFW_KEY_F, Key::F }, { GLFW_KEY_G, Key::G }, { GLFW_KEY_H, Key::H },
        { GLFW_KEY_I, Key::I }, { GLFW_KEY_J, Key::J }, { GLFW_KEY_K, Key::K }, { GLFW_KEY_L, Key::L },
        { GLFW_KEY_M, Key::M }, { GLFW_KEY_N, Key::N }, { GLFW_KEY_O, Key::O }, { GLFW_KEY_P, Key::P },
        { GLFW_KEY_Q, Key::Q }, { GLFW_KEY_R, Key::R }, { GLFW_KEY_S, Key::S }, { GLFW_KEY_T, Key::T },
        { GLFW_KEY_U, Key::U }, { GLFW_KEY_V, Key::V }, { GLFW_KEY_W, Key::W }, { GLFW_KEY_X, Key::X },
        { GLFW_KEY_Y, Key::Y }, { GLFW_KEY_Z, Key::Z },
        { GLFW_KEY_LEFT_BRACKET, Key::LEFT_BRACKET }, { GLFW_KEY_BACKSLASH, Key::BACKSLASH }, { GLFW_KEY_RIGHT_BRACKET, Key::RIGHT_BRACKET },

        { GLFW_KEY_ESCAPE, Key::ESCAPE }, { GLFW_KEY_ENTER, Key::ENTER }, { GLFW_KEY_TAB, Key::TAB }, { GLFW_KEY_BACKSPACE, Key::BACKSPACE },
        { GLFW_KEY_INSERT, Key::INSERT }, { GLFW_KEY_DELETE, Key::DEL }, { GLFW_KEY_RIGHT, Key::RIGHT }, { GLFW_KEY_LEFT, Key::LEFT },
        { GLFW_KEY_DOWN, Key::DOWN }, { GLFW_KEY_UP, Key::UP }, { GLFW_KEY_PAGE_UP, Key::PAGE_UP }, { GLFW_KEY_PAGE_DOWN, Key::PAGE_DOWN },
        { GLFW_KEY_HOME, Key::HOME }, { GLFW_KEY_END, Key::END },
        { GLFW_KEY_CAPS_LOCK, Key::CAPS_LOCK }, { GLFW_KEY_SCROLL_LOCK, Key::SCROLL_LOCK }, { GLFW_KEY_NUM_LOCK, Key::NUM_LOCK },
        { GLFW_KEY_PRINT_SCREEN, Key::PRINT_SCREEN }, { GLFW_KEY_PAUSE, Key::PAUSE },

        { GLFW_KEY_F1, Key::F1 }, { GLFW_KEY_F2, Key::F2 }, { GLFW_KEY_F3, Key::F3 }, { GLFW_KEY_F4, Key::F4 }, { GLFW_KEY_F5, Key::F5 }, { GLFW_KEY_F6, Key::F6 },
        { GLFW_KEY_F7, Key::F7 }, { GLFW_KEY_F8, Key::F8 }, { GLFW_KEY_F9, Key::F9 }, { GLFW_KEY_F10, Key::F10 }, { GLFW_KEY_F11, Key::F11 }, { GLFW_KEY_F12, Key::F12 },
        { GLFW_KEY_F13, Key::F13 }, { GLFW_KEY_F14, Key::F14 }, { GLFW_KEY_F15, Key::F15 }, { GLFW_KEY_F16, Key::F16 }, { GLFW_KEY_F17, Key::F17 }, { GLFW_KEY_F18, Key::F18 },
        { GLFW_KEY_F19, Key::F19 }, { GLFW_KEY_F20, Key::F20 }, { GLFW_KEY_F21, Key::F21 }, { GLFW_KEY_F22, Key::F22 }, { GLFW_KEY_F23, Key::F23 }, { GLFW_KEY_F24, Key::F24 },

        { GLFW_KEY_KP_0, Key::NP_0 }, { GLFW_KEY_KP_1, Key::NP_1 }, { GLFW_KEY_KP_2, Key::NP_2 },
        { GLFW_KEY_KP_3, Key::NP_3 }, { GLFW_KEY_KP_4, Key::NP_4 }, { GLFW_KEY_KP_5, Key::NP_5 },
        { GLFW_KEY_KP_6, Key::NP_6 }, { GLFW_KEY_KP_7, Key::NP_7 }, { GLFW_KEY_KP_8, Key::NP_8 },
        { GLFW_KEY_KP_9, Key::NP_9 }, { GLFW_KEY_KP_DECIMAL, Key::NP_DECIMAL },
        { GLFW_KEY_KP_DIVIDE, Key::NP_DIVIDE }, { GLFW_KEY_KP_MULTIPLY, Key::NP_MULTIPLY },
        { GLFW_KEY_KP_SUBTRACT, Key::NP_SUBTRACT }, { GLFW_KEY_KP_ADD, Key::NP_ADD },
        { GLFW_KEY_KP_ENTER, Key::NP_ENTER }, { GLFW_KEY_KP_EQUAL, Key::NP_EQUAL },

        { GLFW_KEY_LEFT_SHIFT, Key::LEFT_SHIFT }, { GLFW_KEY_LEFT_CONTROL, Key::LEFT_CONTROL },
        { GLFW_KEY_LEFT_ALT, Key::LEFT_ALT }, { GLFW_KEY_LEFT_SUPER, Key::LEFT_SUPER },
        { GLFW_KEY_RIGHT_SHIFT, Key::RIGHT_SHIFT }, { GLFW_KEY_RIGHT_CONTROL, Key::RIGHT_CONTROL },
        { GLFW_KEY_RIGHT_ALT, Key::RIGHT_ALT }, { GLFW_KEY_RIGHT_SUPER, Key::RIGHT_SUPER }, { GLFW_KEY_MENU, Key::MENU },

        { GLFW_KEY_UNKNOWN, Key::NONE }
    };

#endif

#ifdef DGE_PLATFORM_EMSCRIPTEN

    std::unordered_map<int, Key> InputHandler::s_KeysTable =
    {
        { DOM_PK_SPACE, Key::SPACE }, { DOM_PK_QUOTE, Key::APOSTROPHE }, { DOM_PK_COMMA, Key::COMMA },
        { DOM_PK_MINUS, Key::MINUS }, { DOM_PK_PERIOD, Key::PERIOD }, { DOM_PK_SLASH, Key::SLASH },

        { DOM_PK_0, Key::K0 }, { DOM_PK_1, Key::K1 }, { DOM_PK_2, Key::K2 }, { DOM_PK_3, Key::K3 }, { DOM_PK_4, Key::K4 },
        { DOM_PK_5, Key::K5 }, { DOM_PK_6, Key::K6 }, { DOM_PK_7, Key::K7 }, { DOM_PK_8, Key::K8 }, { DOM_PK_9, Key::K9 },

        { DOM_PK_SEMICOLON, Key::SEMICOLON }, { DOM_PK_EQUAL, Key::EQUAL },

        { DOM_PK_A, Key::A }, { DOM_PK_B, Key::B }, { DOM_PK_C, Key::C }, { DOM_PK_D, Key::D },
        { DOM_PK_E, Key::E }, { DOM_PK_F, Key::F }, { DOM_PK_G, Key::G }, { DOM_PK_H, Key::H },
        { DOM_PK_I, Key::I }, { DOM_PK_J, Key::J }, { DOM_PK_K, Key::K }, { DOM_PK_L, Key::L },
        { DOM_PK_M, Key::M }, { DOM_PK_N, Key::N }, { DOM_PK_O, Key::O }, { DOM_PK_P, Key::P },
        { DOM_PK_Q, Key::Q }, { DOM_PK_R, Key::R }, { DOM_PK_S, Key::S }, { DOM_PK_T, Key::T },
        { DOM_PK_U, Key::U }, { DOM_PK_V, Key::V }, { DOM_PK_W, Key::W }, { DOM_PK_X, Key::X },
        { DOM_PK_Y, Key::Y }, { DOM_PK_Z, Key::Z },

        { DOM_PK_BRACKET_LEFT, Key::LEFT_BRACKET }, { DOM_PK_BACKSLASH, Key::BACKSLASH }, { DOM_PK_BRACKET_RIGHT, Key::RIGHT_BRACKET },

        { DOM_PK_ESCAPE, Key::ESCAPE }, { DOM_PK_ENTER, Key::ENTER }, { DOM_PK_TAB, Key::TAB }, { DOM_PK_BACKSPACE, Key::BACKSPACE },
        { DOM_PK_INSERT, Key::INSERT }, { DOM_PK_DELETE, Key::DEL }, { DOM_PK_ARROW_RIGHT, Key::RIGHT }, { DOM_PK_ARROW_LEFT, Key::LEFT },
        { DOM_PK_ARROW_DOWN, Key::DOWN }, { DOM_PK_ARROW_UP, Key::UP }, { DOM_PK_PAGE_UP, Key::PAGE_UP }, { DOM_PK_PAGE_DOWN, Key::PAGE_DOWN },
        { DOM_PK_HOME, Key::HOME }, { DOM_PK_END, Key::END },
        { DOM_PK_CAPS_LOCK, Key::CAPS_LOCK }, { DOM_PK_SCROLL_LOCK, Key::SCROLL_LOCK }, { DOM_PK_NUM_LOCK, Key::NUM_LOCK },
        { DOM_PK_PRINT_SCREEN, Key::PRINT_SCREEN }, { DOM_PK_PAUSE, Key::PAUSE },

        { DOM_PK_F1, Key::F1 }, { DOM_PK_F2, Key::F2 }, { DOM_PK_F3, Key::F3 }, { DOM_PK_F4, Key::F4 }, { DOM_PK_F5, Key::F5 }, { DOM_PK_F6, Key::F6 },
        { DOM_PK_F7, Key::F7 }, { DOM_PK_F8, Key::F8 }, { DOM_PK_F9, Key::F9 }, { DOM_PK_F10, Key::F10 }, { DOM_PK_F11, Key::F11 }, { DOM_PK_F12, Key::F12 },
        { DOM_PK_F13, Key::F13 }, { DOM_PK_F14, Key::F14 }, { DOM_PK_F15, Key::F15 }, { DOM_PK_F16, Key::F16 }, { DOM_PK_F17, Key::F17 }, { DOM_PK_F18, Key::F18 },
        { DOM_PK_F19, Key::F19 }, { DOM_PK_F20, Key::F20 }, { DOM_PK_F21, Key::F21 }, { DOM_PK_F22, Key::F22 }, { DOM_PK_F23, Key::F23 }, { DOM_PK_F24, Key::F24 },

        { DOM_PK_NUMPAD_0, Key::NP_0 }, { DOM_PK_NUMPAD_1, Key::NP_1 }, { DOM_PK_NUMPAD_2, Key::NP_2 },
        { DOM_PK_NUMPAD_3, Key::NP_3 }, { DOM_PK_NUMPAD_4, Key::NP_4 }, { DOM_PK_NUMPAD_5, Key::NP_5 },
        { DOM_PK_NUMPAD_6, Key::NP_6 }, { DOM_PK_NUMPAD_7, Key::NP_7 }, { DOM_PK_NUMPAD_8, Key::NP_8 },
        { DOM_PK_NUMPAD_9, Key::NP_9 }, { DOM_PK_NUMPAD_DECIMAL, Key::NP_DECIMAL },
        { DOM_PK_NUMPAD_DIVIDE, Key::NP_DIVIDE }, { DOM_PK_NUMPAD_MULTIPLY, Key::NP_MULTIPLY },
        { DOM_PK_NUMPAD_SUBTRACT, Key::NP_SUBTRACT }, { DOM_PK_NUMPAD_ADD, Key::NP_ADD },
        { DOM_PK_NUMPAD_ENTER, Key::NP_ENTER }, { DOM_PK_NUMPAD_EQUAL, Key::NP_EQUAL },

        { DOM_PK_SHIFT_LEFT, Key::LEFT_SHIFT }, { DOM_PK_CONTROL_LEFT, Key::LEFT_CONTROL },
        { DOM_PK_ALT_LEFT, Key::LEFT_ALT }, { DOM_PK_OS_LEFT, Key::LEFT_SUPER },
        { DOM_PK_SHIFT_RIGHT, Key::RIGHT_SHIFT }, { DOM_PK_CONTROL_RIGHT, Key::RIGHT_CONTROL },
        { DOM_PK_ALT_RIGHT, Key::RIGHT_ALT }, { DOM_PK_OS_RIGHT, Key::RIGHT_SUPER }, { DOM_PK_CONTEXT_MENU, Key::MENU },

        { DOM_PK_UNKNOWN, Key::NONE }
    };

#endif

    KeyState::KeyState() : held(false), released(false), pressed(false)
    {

    }

	KeyState::KeyState(bool held, bool released, bool pressed)
        : held(held), released(released), pressed(pressed)
    {

    }

    InputHandler::InputHandler(Platform* platform)
        : m_MousePos(-1, -1), m_Platform(platform), m_CaptureText(false), m_Caps(false), m_CapturedTextCursorPos(0)
    {
        uint8_t keysCount = static_cast<uint8_t>(Key::KEYS_COUNT);

        for (uint8_t i = 0; i < keysCount; i++)
        {
            m_Keys[i] = { false, false, false };
            m_KeyOldState[i] = false;
            m_KeyNewState[i] = false;
        }

        for (uint8_t i = 0; i < 8; i++)
        {
            m_Mouse[i] = { false, false, false };
            m_MouseOldState[i] = false;
            m_MouseNewState[i] = false;
        }
    }

    void InputHandler::GrabEvents()
    {
        m_ScrollDelta = 0;
        m_Platform->PollEvents();
    }

    void InputHandler::FlushBuffers()
    {
        UpdateState(m_Keys, m_KeyNewState, m_KeyOldState, static_cast<uint8_t>(Key::KEYS_COUNT));
		UpdateState(m_Mouse, m_MouseNewState, m_MouseOldState, 8);
    }

    void InputHandler::GrabText()
    {
        if (GetKeyState(Key::CAPS_LOCK).pressed)
			m_Caps = !m_Caps;

        if (!m_CaptureText)
            return;

        bool isUp = GetKeyState(Key::LEFT_SHIFT).held || GetKeyState(Key::RIGHT_SHIFT).held;

        for (const auto& [key, chars] : s_KeyboardUS)
        {
            if (GetKeyState(key).pressed)
            {
                if (m_Caps || isUp)
                    m_CapturedText.insert(m_CapturedTextCursorPos, 1, chars.second);
                else
                    m_CapturedText.insert(m_CapturedTextCursorPos, 1, chars.first);

                m_CapturedTextCursorPos++;
            }
        }

        if (GetKeyState(Key::BACKSPACE).pressed)
        {
            if (m_CapturedTextCursorPos > 0)
            {
                m_CapturedText.erase(m_CapturedTextCursorPos - 1, 1);
                m_CapturedTextCursorPos--;
            }
        }

        if (GetKeyState(Key::DEL).pressed)
        {
            if (m_CapturedTextCursorPos < m_CapturedText.length())
                m_CapturedText.erase(m_CapturedTextCursorPos, 1);
        }

        if (GetKeyState(Key::LEFT).pressed)
        {
            if (m_CapturedTextCursorPos > 0)
                m_CapturedTextCursorPos--;
        }

        if (GetKeyState(Key::RIGHT).pressed)
        {
            if (m_CapturedTextCursorPos < m_CapturedText.length())
                m_CapturedTextCursorPos++;
        }

        if (GetKeyState(Key::ENTER).pressed)
        {
            GameEngine::s_Engine->OnTextCapturingComplete(m_CapturedText);
            GameEngine::s_Engine->m_Console->HandleCommand(m_CapturedText);

            m_CapturedText.clear();
            m_CapturedTextCursorPos = 0;
        }

        GameEngine::s_Engine->m_Console->HandleHistoryBrowsing();
    }

    void InputHandler::SetCapturedText(const std::string& text)
    {
        m_CapturedText = text;
    }

    void InputHandler::SetCapturedTextCursorPosition(size_t pos)
    {
        m_CapturedTextCursorPos = pos;
    }

    void InputHandler::ClearCapturedText()
    {
        m_CapturedText.clear();
	    m_CapturedTextCursorPos = 0;
    }

    const KeyState& InputHandler::GetKeyState(Key key) const
    {
        return m_Keys[static_cast<uint8_t>(key)];
    }

    const KeyState& InputHandler::GetButtonState(Button button) const
    {
        return m_Mouse[static_cast<uint8_t>(button)];
    }

    void InputHandler::CaptureText(bool enable)
    {
        m_CaptureText = enable;
    }

    bool InputHandler::IsCapturingText() const
    {
        return m_CaptureText;
    }

    const Vector2i& InputHandler::GetMousePosition() const
    {
        return m_MousePos;
    }

    int InputHandler::GetMouseX() const
    {
        return m_MousePos.x;
    }

	int InputHandler::GetMouseY() const
    {
        return m_MousePos.y;
    }

    int InputHandler::GetScrollDelta() const
    {
        return m_ScrollDelta;
    }

    const std::string& InputHandler::GetCapturedText() const
    {
        return m_CapturedText;
    }

    size_t InputHandler::GetCapturedTextCursorPosition() const
    {
        return m_CapturedTextCursorPos;
    }

    bool InputHandler::IsCaps() const
    {
        return m_Caps;
    }

    void InputHandler::UpdateState(KeyState* data, bool* newState, bool* oldState, uint8_t count)
    {
        for (uint8_t i = 0; i < count; i++)
        {
            data[i].pressed = false;
            data[i].released = false;

            if (newState[i] != oldState[i])
            {
                if (newState[i])
                {
                    data[i].pressed = !data[i].held;
                    data[i].held = true;
                }
                else
                {
                    data[i].released = true;
                    data[i].held = false;
                }
            }

            oldState[i] = newState[i];
        }
    }
}
