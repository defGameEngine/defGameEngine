#pragma once

#include "Layer.hpp"

namespace def
{
    Layer::~Layer()
    {
        if (pixels)
            delete[] pixels;
    }

    bool Layer::OnCreate() { return true; }
    bool Layer::OnUpdate(float deltaTime) { return true; }

    GameEngine& Layer::Context()
    {
        return *GameEngine::s_Engine;
    }
}