/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#pragma once

#ifndef DGE_TIMER_HPP
#define DGE_TIMER_HPP

#include <chrono>

namespace def
{
	using TimePoint = std::chrono::system_clock::time_point;

	class Timer
	{
	public:
		Timer();

		// Returns difference between 2 frames in seconds
		float GetDeltaTime() const;

		// Returns the number of frames per second
		// We update frames count in the title bar only every second so the returned value is more precise in here than in the title bar
		int GetFPS() const;

		friend class GameEngine;

	protected:
		void Update();
		float GetTicks();
		void ResetTicks();

	private:
		TimePoint m_TimeStart;
		TimePoint m_TimeEnd;

		// Storing the difference between 2 frames
		float m_DeltaTime;

		// Is used for updating frames count in the title bar
		float m_TickTimer;

	};
}

#endif
