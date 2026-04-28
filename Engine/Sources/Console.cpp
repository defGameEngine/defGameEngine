/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

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
            if (!command.empty())
            {
                m_History.push_back({ command, output.str(), colour, true });
                m_PickedHistoryCommand = m_History.size();
            }
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

        int currentLayer = e->m_CurrentLayer;
        e->SetLayer(0);

        e->FillTextureRectangle({ 0, 0 }, e->m_Window->GetScreenSize(), m_BackgroundColour);

        int lineHeight = 10;
        int screenHeight = e->m_Window->GetScreenHeight();

        int inputY = screenHeight - lineHeight - 5;
        int historyBottomY = inputY - lineHeight;

        int offset = historyBottomY;

        for (int i = (int)m_History.size() - 1; i >= 0; i--)
        {
            const auto& entry = m_History[i];

            int lines = 1;

            for (char c : entry.output)
            {
                if (c == '\n')
                    lines++;
            }

            if (entry.isCommand)
            {
                if (offset < 0)
                    break;

                e->DrawTextureString({ 10, offset }, "> " + entry.command, WHITE);
                offset -= lineHeight;
            }

            for (int l = 0; l < lines; l++)
            {
                if (offset < 0) break;

                std::string text;

                if (lines == 1)
                    text = entry.output;
                else
                {
                    size_t start = 0;
                    size_t end = entry.output.find('\n');

                    for (int j = 0; j <= l; j++)
                    {
                        start = (j == 0 ? 0 : end + 1);
                        end = entry.output.find('\n', start);
                    }

                    text = entry.output.substr(
                        start,
                        end == std::string::npos ? std::string::npos : end - start
                    );
                }

                e->DrawTextureString({ 10, offset }, text, entry.outputColour);
                offset -= lineHeight;
            }
        }

        std::string input = e->m_Input->GetCapturedText();

		// Draw an input prompt
        e->DrawTextureString({ 10, inputY }, "> " + input, YELLOW);

        int cursorX = 10 + 8 * (2 + (int)input.size());

        // Draw a cursor
        e->DrawTextureLine({ cursorX, inputY }, { cursorX, inputY + 8 }, RED);

        e->SetLayer(currentLayer);
    }

    void Console::Show(bool show)
    {
        Layer& layer = *GameEngine::s_Engine->m_Layers[0];

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
        return GameEngine::s_Engine->m_Layers[0]->visible;
    }
}
