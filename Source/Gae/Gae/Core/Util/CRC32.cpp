#include "stdafx.h"
#include "crc32.h"

namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			CRC32::TTable CRC32::m_TABLE;
			static const UINT32 POLY32 = 0xedb88320L;
			CRC32::TTable::TTable()
			{
				for (UINT32 i=0; i<256; ++i)
				{
					UINT32 _crc = i;
					for (int j=0; j<8; ++j)
					{
						if (_crc&1)
							_crc = (_crc>>1)^POLY32;
						else
							_crc>>=1;
					}
					T[i] = _crc;
				}
			}
		}
	}
}