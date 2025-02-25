#include "Pch.hpp"
#include "Platform.hpp"

def::Platform::Platform()
{
}

void def::Platform::SetWindow(std::shared_ptr<Window> window)
{
    m_Window = window;
}

void def::Platform::SetInputHandler(std::shared_ptr<InputHandler> input)
{
    m_Input = input;
}
