#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{

		GAE_CONTAINER_IMPL(Material, Gae::Graphics::Material::BlendStage, BlendStage);

		Material::Material()
			: m_pMapDiffuse(NULL)
			, m_pMapHeight(NULL)
			, m_pMapNormal(NULL)
			, m_pMapSpecular(NULL)
			, m_fAlphaTest(0.0f)
			, m_uFlags(0)
			, m_eBlendSrc(BT_ONE)
			, m_eBlendDst(BT_ONE)
			, m_dwPolygonOffset(0)
		{
			SetBlendSrcDst(BT_ONE, BT_ONE);
		}

		Material::~Material()
		{

		}
	}
}