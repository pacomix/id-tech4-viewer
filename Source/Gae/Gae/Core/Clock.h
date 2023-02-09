#ifndef Clock_h__
#define Clock_h__

namespace Gae
{
	namespace Core
	{
		class Clock
		{
			SINGLETON_DECL(Clock);

		public:
			friend class Gae::Core::Application;	// Only Application can update the clock.

			static inline double GetEllapsedSeconds();
			static inline double GetEllapsedMilliseconds();
			static inline UINT64 GetEllapsedTicks();
			static inline UINT64 GetCurrentTicks();

			inline void SetSpeed(float fSpeed);

		private:
			void Update();

			static double m_dEllapsedSeconds;
			static double m_dEllapsedMilliseconds;
			static UINT64 m_ui64EllapsedTicks;

			static UINT64 m_ui64CurrentTicks;
			static UINT64 m_ui64PreviousTicks;

			static UINT64 m_u64TicksPerSecond;

			float m_fSpeed;

		};
	}
}
#include "Clock.inl"
#endif // Clock_h__