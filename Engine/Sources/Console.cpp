#include "Pch.hpp"
#include "Console.hpp"

namespace def
{
    Console::Console() : m_BackgroundColour(0, 0, 255, 100), m_PickedHistoryCommand(0)
    {
    }

    void Console::Clear()
    {
        m_PickedHistoryCommand = 0;
        m_History.clear();
    }

    void Console::HandleCommand(const std::string& command)
    {
        if (!IsShown())
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
        if (!IsShown() || m_History.empty())
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
            
            GameEngine::s_Engine->m_Input->SetCapturedText(command);
            GameEngine::s_Engine->m_Input->SetCapturedTextCursorPosition(command.length());
        }
    }

    void Console::Draw()
    {
        if (!IsShown())
            return;

        GameEngine* e = GameEngine::s_Engine;

        int currentLayer = e->m_PickedLayer;
        e->PickLayer(0);

        e->FillTextureRectangle({ 0, 0 }, e->m_Window->GetScreenSize(), m_BackgroundColour);

        int printCount = std::min(e->m_Window->GetScreenHeight() / 22, (int)m_History.size());
        int start = m_History.size() - printCount;

        for (size_t i = start; i < m_History.size(); i++)
        {
            auto& entry = m_History[i];

            e->DrawTextureString({ 10, 10 + (int(i) - start) * 20 }, "> " + entry.command);
            e->DrawTextureString({ 10, 20 + (int(i) - start) * 20 }, entry.output, entry.outputColour);
        }

        int x = e->m_Input->GetCapturedTextCursorPosition() * 8 + 36;
        int y = e->m_Window->GetScreenHeight() - 18;

        e->DrawTextureString({ 20, y }, "> " + e->m_Input->GetCapturedText(), YELLOW);
        e->DrawTextureLine({ x, y }, { x, y + 8 }, RED);

        e->PickLayer(currentLayer);
    }

    void Console::Show(bool show)
    {
        Layer& layer = GameEngine::s_Engine->m_Layers[0];

        layer.visible = show;
        layer.update = show;

        GameEngine::s_Engine->m_Input->CaptureText(show);
    }

    void Console::SetBackgroundColour(const Pixel& colour)
    {
        m_BackgroundColour = colour;
    }

    bool Console::IsShown() const
    {
        return GameEngine::s_Engine->m_Layers[0].visible;
    }
}
