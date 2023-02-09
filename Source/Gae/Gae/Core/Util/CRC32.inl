namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			inline UINT32 CRC32::set(std::string const& _str)
			{
				char const* psz = _str.c_str();
				GAE_ASSERTM(psz, L"Empty string!!");
				return set(psz);
			}

			inline UINT32 CRC32::set(std::wstring const& _str)
			{
				wchar_t const* pwsz = _str.c_str();
				GAE_ASSERTM(pwsz, L"Empty string!!");
				return set(pwsz);
			}

			inline UINT32 CRC32::set(char const* _psz)
			{
				GAE_ASSERTM(_psz!=0, L"Empty string!! NULL!!");

				UINT32  uRet = 0xffffffff;
				UINT8 const* x = (UINT8 const*)_psz;
				while (*x)
				{
					uRet = (uRet>>8) ^ m_TABLE.T[ ((UINT8)uRet)^(*x++) ]; 
				}

				return uRet ^ 0xffffffff;
			}

			inline UINT32 CRC32::set(wchar_t const* _pwsz)
			{
				GAE_ASSERTM(_pwsz!=0, L"Empty string!! NULL");

				UINT32  uRet = 0xffffffff;
				UINT8 const * x = (UINT8 const*)_pwsz;
				while ((*x) || (*(x+1)))
				{
					uRet = (uRet>>8) ^ m_TABLE.T[ ((UINT8)uRet)^(*x++) ];
					uRet = (uRet>>8) ^ m_TABLE.T[ ((UINT8)uRet)^(*x++) ]; 
				}
				return uRet ^ 0xffffffff;
			}

			inline void CRC32::Add (UINT8 _x)
			{ 
				m_crc = (m_crc>>8) ^ m_TABLE.T[ ((UINT8)m_crc)^_x ]; 
			}

			inline void CRC32::Add (void const* _buffer, int _size)
			{
				GAE_ASSERTM(_buffer!=0, L"NULL buffer!!");
				GAE_ASSERTM(_size>=0, L"Size can not be 0!!");
				UINT8 const* x = (UINT8 const*)_buffer;
				while (_size--)
				{
					m_crc = (m_crc>>8) ^ m_TABLE.T[ ((UINT8)m_crc)^(*x++) ];
				}
			}

			inline void CRC32::AddSZ (char const* _sz)
			{ 
				GAE_ASSERTM(_sz != NULL, L"Empty string!! NULL");
				UINT8 const* x = (UINT8 const*)_sz;
				while (*x)
				{
					m_crc = (m_crc>>8) ^ m_TABLE.T[ ((UINT8)m_crc)^(*x++) ]; 
				}
			}
		}
	}
}