#include "Pch.hpp"
#include "Platform.hpp"

def::Platform::Platform()
{
}

void def::Platform::SetWindow(Window* window)
{
    m_Window = window;
}

void def::Platform::SetInputHandler(InputHandler* input)
{
    m_Input = input;
}
