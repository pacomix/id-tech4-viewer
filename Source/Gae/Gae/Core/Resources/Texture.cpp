#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			Texture::Texture() 
				: m_pTexture(NULL)
				, m_bIsAlpha(FALSE)
				, m_fTexelsPerSide(0.0f)
				, super()
			{
				m_wsFilename.clear();
			}

			Texture::~Texture()
			{
			}
		}
	}
}