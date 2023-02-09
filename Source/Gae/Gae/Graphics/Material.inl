namespace Gae
{
	namespace Graphics
	{
		void Material::SetMapDiffuse(Gae::Core::Resources::Texture* pTexture)
		{
			GAE_ASSERTM(NULL != pTexture, L"Incorrect texture!\n");
			m_pMapDiffuse = pTexture;
		}

		void Material::SetMapSpecular(Gae::Core::Resources::Texture* pTexture)
		{
			GAE_ASSERTM(NULL != pTexture, L"Incorrect texture!\n");
			m_pMapSpecular = pTexture;
		}

		void Material::SetMapHeight(Gae::Core::Resources::Texture* pTexture)
		{
			GAE_ASSERTM(NULL != pTexture, L"Incorrect texture!\n");
			m_pMapHeight = pTexture;
		}

		void Material::SetMapNormal(Gae::Core::Resources::Texture* pTexture)
		{
			GAE_ASSERTM(NULL != pTexture, L"Incorrect texture!\n");
			m_pMapNormal = pTexture;
		}

		/*
		void Material::SetAlphaTest(bool bTrue)
		{
			if(bTrue)
			{
				m_uFlags |= M_TRANSLUCENT;
			}
			else
			{
				m_uFlags &= ~M_TRANSLUCENT;
			}
		}

		void Material::SetDecal(bool bTrue)
		{
			if(bTrue)
			{
				m_uFlags |= M_DECAL;
			}
			else
			{
				m_uFlags &= ~M_DECAL;
			}
		}

		void Material::SetBlend(bool bTrue)
		{
			if(bTrue)
			{
				m_uFlags |= M_BLEND;
			}
			else
			{
				m_uFlags &= ~M_BLEND;
			}
		}
		*/

		DWORD Material::GetBlendSrc() const
		{
			return m_dwBlendSrc;
		}

		DWORD Material::GetBlendDst() const
		{
			return m_dwBlendDst;
		}

		void Material::SetBlendSrcDst(const E_BLEND_TYPE eBlendSrc, const E_BLEND_TYPE eBlendDst, bool bDirectX/* = true*/)
		{
			//GAE_ASSERTM(FlagEnabled(M_BLEND), L"Blending is not set for this material!!!");
			m_eBlendSrc = eBlendSrc;
			m_eBlendDst = eBlendDst;

			m_dwBlendSrc = GetRendererBlendValue(m_eBlendSrc);
			m_dwBlendDst = GetRendererBlendValue(m_eBlendDst);

		}

		DWORD Material::GetRendererBlendValue(const E_BLEND_TYPE eBlendType)
		{
			switch(eBlendType)
			{
				case BT_ONE:
					return D3DBLEND_ONE;
					break;
				case BT_ZERO:
					return D3DBLEND_ZERO;
					break;
				case BT_DST_COLOR:
					return D3DBLEND_DESTCOLOR;
					break;
				case BT_ONE_MINUS_DST_COLOR:
					return D3DBLEND_INVDESTCOLOR;
					break;
				case BT_SRC_ALPHA:
					return D3DBLEND_SRCALPHA;
					break;
				case BT_ONE_MINUS_SRC_ALPHA:
					return D3DBLEND_INVSRCALPHA;
					break;
				case BT_DST_ALPHA:
					return D3DBLEND_DESTALPHA;
					break;
				case BT_ONE_MINUS_DST_ALPHA:
					return D3DBLEND_INVDESTALPHA;
					break;
				case BT_SRC_ALPHA_SATURATE:
					return D3DBLEND_SRCALPHASAT;
					break;
				case BT_SRC_COLOR:
					return D3DBLEND_SRCCOLOR;
					break;
				case BT_ONE_MINUS_SRC_COLOR:
					return D3DBLEND_INVSRCCOLOR;
					break;
				default:
					GAE_ASSERTM(false, L"Unsupported blending type!!!");
			}
			return D3DBLEND_ONE;
		}

		/*
		bool Material::IsAlphaTest() const
		{
			return (m_uFlags & M_TRANSLUCENT) != 0;
		}

		bool Material::IsDecal() const
		{
			return (m_uFlags & M_DECAL) != 0;
		}

		bool Material::IsBlend() const
		{
			return (m_uFlags & M_BLEND) != 0;
		}
		*/

		Texture* Material::GetMapDiffuse() const
		{
			return m_pMapDiffuse;
		}

		Texture* Material::GetMapSpecular() const
		{
			return m_pMapSpecular;
		}

		Texture* Material::GetMapHeight() const
		{
			return m_pMapHeight;
		}

		Texture* Material::GetMapNormal() const
		{
			return m_pMapNormal;
		}

		void Material::SetAlphaTestValue(float fAlphaTest)
		{
			m_fAlphaTest = fAlphaTest;
		}

		void Material::SetPolygonOffset(DWORD dwPolygonOffset)
		{
			m_dwPolygonOffset = dwPolygonOffset;
		}

		DWORD Material::GetPolygonOffset() const
		{
			return m_dwPolygonOffset;
		}
	}
}