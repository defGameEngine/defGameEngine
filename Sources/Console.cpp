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
