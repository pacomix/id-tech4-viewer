#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			ShadowVolume::ShadowVolume() : m_uCountCaps(0), m_uCountCapsFront(0), m_uPlaneBits(0), m_pMesh(NULL)
			{

			}

			ShadowVolume::~ShadowVolume()
			{
				GAE_DELETESAFE(m_pMesh);
			}


#ifdef _DEBUG_DRAW
			void ShadowVolume::DrawDebug(const Gae::Renderer::Driver* pRenderer) const
			{
			}
#endif // _DEBUG
		}
	}
}