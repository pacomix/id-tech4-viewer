#include "stdafx.h"

namespace Gae
{
	namespace Entities
	{
		bool Modifier::Init(IEntity* pEntity)
		{
			GAE_ASSERT(NULL != pEntity);

			m_pParent = pEntity;

			return TRUE;
		}
	}
}