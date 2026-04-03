/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "Layer.hpp"

namespace def
{
    Layer::~Layer()
    {
        if (pixels)
            delete pixels;
    }

    bool Layer::OnCreate() { return true; }
    bool Layer::OnUpdate(float deltaTime) { return true; }

    GameEngine& Layer::Context()
    {
        return *GameEngine::s_Engine;
    }
}