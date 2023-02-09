namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			inline void Texture::SetFilename(const std::wstring& wsFilename)
			{
				GAE_ASSERTM(!wsFilename.empty(), L"Empty texture filename.");
				m_wsFilename.resize(wsFilename.size());
				m_wsFilename = wsFilename;
				SetTAG(wsFilename);
			}

			inline void Texture::SetFilename(const std::string& strFilename)
			{
				std::wstring wsFilename(strFilename.length(), L' ');
				std::copy(strFilename.begin(), strFilename.end(), wsFilename.begin());
				SetFilename(wsFilename);
			}

			inline const Vector2f& Texture::GetTexelSize() const
			{
				return m_v2fTexelSize;
			}

			inline bool Texture::IsAlpha() const
			{
				return m_bIsAlpha;
			}

#ifdef _DEBUG
			inline void Texture::ToDebugOutput()
			{
				GAE_TRACE(L"Texture: [%s]\n", m_wsFilename.c_str());
			}
#endif
		}
	}
}