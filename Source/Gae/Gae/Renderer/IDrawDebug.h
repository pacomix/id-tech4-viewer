#ifndef IDrawDebug_h__
#define IDrawDebug_h__

namespace Gae
{
	namespace Renderer
	{
		class Driver;

		class IDrawDebug
		{
		public:
			virtual void DrawDebug(const Gae::Renderer::Driver* pRenderer) const = 0;

			inline void SetDrawDebug(bool bVal)
			{
				m_bDraw = bVal;
			}

			inline bool IsDrawDebug() const
			{
				return m_bDraw;
			}

		private:
			bool m_bDraw;	//!< Activate/Deactivate the call of DrawDebug method.
		};
	}
}
#endif // IDrawDebug_h__