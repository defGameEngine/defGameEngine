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

    void Console::Print(const std::string& text, const Pixel& colour)
    {
        m_History.push_back({ "", text, colour, false });
    }

    void Console::HandleCommand(const std::string& command)
    {
        if (!IsShown())
            return;

        std::stringstream output;
        Pixel colour = WHITE;

        if (GameEngine::s_Engine->OnConsoleCommand(command, output, colour))
        {
            m_History.push_back({ command, output.str(), colour, true });
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
            do
            {
                if (m_PickedHistoryCommand > 0)
                {
                    m_PickedHistoryCommand--;
                    moved = m_History[m_PickedHistoryCommand].isCommand;
                }
            }
            while (!moved && m_PickedHistoryCommand > 0);
        }

        if (GameEngine::s_Engine->m_Input->GetKeyState(Key::DOWN).pressed)
        {
            do
            {
                if (m_PickedHistoryCommand < m_History.size() - 1)
                {
                    m_PickedHistoryCommand++;
                    moved = m_History[m_PickedHistoryCommand].isCommand;
                }
            }
            while (!moved && m_PickedHistoryCommand < m_History.size() - 1);
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
        int offset = 10;

        for (int i = m_History.size() - printCount; i < m_History.size(); i++)
        {
            auto& entry = m_History[i];

            if (entry.isCommand)
            {
                e->DrawTextureString({ 10, offset }, "> " + entry.command);
                offset += 10;
            }

            e->DrawTextureString({ 10, offset }, entry.output, entry.outputColour);
            offset += 10;
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
