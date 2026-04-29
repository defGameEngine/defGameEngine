/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_STATE_HPP
#define DGE_STATE_HPP

#include "Pch.hpp"
#include "defGameEngine.hpp"

namespace def
{
	class GameEngine;
	
	struct State
	{
		State(GameEngine* context);

		virtual bool OnCreate() = 0;
		virtual bool OnSet() = 0;
		virtual bool OnUpdate(float deltaTime) = 0;

		GameEngine& context;
	};
}

#endif
