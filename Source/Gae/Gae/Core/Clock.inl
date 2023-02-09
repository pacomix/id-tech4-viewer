namespace Gae
{
	namespace Core
	{
		void Clock::SetSpeed(float fSpeed)
		{
			GAE_ASSERTM(fSpeed > 0.0f, L"Incorrect speed. 0 < Speed. Value set: %f", fSpeed);
			m_fSpeed = fSpeed;
		}

		double Clock::GetEllapsedSeconds()
		{
			return m_dEllapsedSeconds;
		}

		double Clock::GetEllapsedMilliseconds()
		{
			return m_dEllapsedMilliseconds;
		}

		UINT64 Clock::GetEllapsedTicks()
		{
			return m_ui64EllapsedTicks;
		}

		UINT64 Clock::GetCurrentTicks()
		{
			return m_ui64CurrentTicks;
		}
	}
}