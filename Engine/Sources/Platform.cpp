/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "Platform.hpp"

namespace def
{
    Platform::Platform(GameEngine* engine) : m_Engine(engine)
    {
    }

    void Platform::SetWrapMethod(Sprite::WrapMethod wrapMethod)
    {
        m_WrapMethod = wrapMethod;
    }

    void Platform::SetSampleMethod(Sprite::SampleMethod sampleMethod)
    {
        m_SampleMethod = sampleMethod;
    }

    void Platform::SetWindow(std::shared_ptr<Window> window)
    {
        m_Window = window;
    }

    void Platform::SetInputHandler(std::shared_ptr<InputHandler> input)
    {
        m_Input = input;
    }
}
