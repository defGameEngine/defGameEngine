/*-----------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the GNU General Public License v3.0.
 *  See LICENSE file in the project root for license information.
 *----------------------------------------------------------------*/

#include "Pch.hpp"
#include "Timer.hpp"

namespace def
{
	Timer::Timer()
	{
		m_TimeStart = std::chrono::system_clock::now();
		m_TimeEnd = m_TimeStart;

		m_DeltaTime = 0.0f;
		m_TickTimer = 0.0f;
	}

	float Timer::GetDeltaTime() const
	{
		return m_DeltaTime;
	}

	int Timer::GetFPS() const
	{
		return int(1.0f / m_DeltaTime);
	}

	void Timer::Update()
	{
		m_TimeEnd = std::chrono::system_clock::now();

		m_DeltaTime = std::chrono::duration<float>(m_TimeEnd - m_TimeStart).count();
		m_TimeStart = m_TimeEnd;

		m_TickTimer += m_DeltaTime;
	}

	float Timer::GetTicks()
	{
		return m_TickTimer;
	}

	void Timer::ResetTicks()
	{
		m_TickTimer = 0.0f;
	}
}
