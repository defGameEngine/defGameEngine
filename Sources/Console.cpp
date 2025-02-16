#include "Console.hpp"

namespace def
{
    Console::Console(Layer* layer) : m_Layer(layer), m_BackgroundColour(0, 0, 255, 100), m_PickedHistoryCommand(0)
    {
    }

    void Console::Clear()
    {
        m_PickedHistoryCommand = 0;
        m_History.clear();
    }

    void Console::HandleCommand(const std::string& command)
    {
        // Tells us that the console is enabled
        if (!m_Layer->visible)
            return;

        std::stringstream output;
        Pixel colour = WHITE;

        if (GameEngine::s_Engine->OnConsoleCommand(command, output, colour))
        {
            m_History.push_back({ command, output.str(), colour });
            m_PickedHistoryCommand = m_History.size();
        }
    }

    void Console::HandleHistoryBrowsing()
    {
        if (!m_Layer->visible || m_History.empty())
            return;

        bool moved = false;

        if (GameEngine::s_Engine->m_Input->GetKeyState(Key::UP).pressed)
        {
            if (m_PickedHistoryCommand > 0)
            {
                m_PickedHistoryCommand--;
                moved = true;
            }
        }

        if (GameEngine::s_Engine->m_Input->GetKeyState(Key::DOWN).pressed)
        {
            if (m_PickedHistoryCommand < m_History.size() - 1)
            {
                m_PickedHistoryCommand++;
                moved = true;
            }
        }

        if (moved)
        {
            const std::string& command = m_History[m_PickedHistoryCommand].command;
            
            GameEngine::s_Engine->m_Input->SetInputText(command);
            GameEngine::s_Engine->m_Input->SetTextCursorPosition(command.length());
        }
    }

    void Console::Show(bool show)
    {
        m_Layer->visible = show;
        m_Layer->update = show;
        GameEngine::s_Engine->m_Input->CaptureText(show);
    }

    void Console::SetBackgroundColour(const Pixel& colour)
    {
        m_BackgroundColour = colour;
    }
}
