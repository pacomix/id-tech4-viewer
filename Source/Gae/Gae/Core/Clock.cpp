#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		double Clock::m_dEllapsedSeconds = 0.0;
		double Clock::m_dEllapsedMilliseconds = 0.0;
		UINT64 Clock::m_ui64EllapsedTicks = 0;
		UINT64 Clock::m_ui64CurrentTicks = 0;
		UINT64 Clock::m_ui64PreviousTicks = 0;
		UINT64 Clock::m_u64TicksPerSecond = 0;

		SINGLETON_IMPL(Clock);

		bool Clock::Init()
		{
			if(FALSE == QueryPerformanceFrequency((LARGE_INTEGER*)&m_u64TicksPerSecond))
			{
				return FALSE;
			}

			QueryPerformanceCounter((LARGE_INTEGER*)&m_ui64CurrentTicks);


			m_dEllapsedSeconds = 0.0;
			m_dEllapsedMilliseconds = 0.0;
			m_ui64EllapsedTicks = 0;

			m_ui64PreviousTicks = m_ui64CurrentTicks;
			m_fSpeed = 1.0f;
			return true;
		}

		void Clock::Update()
		{
			m_ui64PreviousTicks = m_ui64CurrentTicks;
			QueryPerformanceCounter((LARGE_INTEGER*)&m_ui64CurrentTicks);
			m_ui64EllapsedTicks = m_ui64CurrentTicks - m_ui64PreviousTicks;
			m_dEllapsedSeconds = (m_ui64EllapsedTicks / (double)m_u64TicksPerSecond) * m_fSpeed;
			m_dEllapsedMilliseconds = m_dEllapsedSeconds / 1000.0;
		}

		bool Clock::Uninit()
		{
			return true;
		}
	}
}