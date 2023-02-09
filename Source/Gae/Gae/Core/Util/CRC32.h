#ifndef CRC32_h__
#define CRC32_h__

#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			class CRC32
			{
			public:
				CRC32() { Reset (); }
				inline void Reset() { m_crc = 0xffffffffL; }

				// Different crc32. Support for wstring.
				explicit CRC32(UINT32 v) : m_crc(v) {}
				CRC32(std::string const& s) : m_crc(set(s)) {}
				CRC32(std::wstring const& s) : m_crc(set(s)) {}
				CRC32(char const*        s) : m_crc(set(s)) {}
				CRC32(wchar_t const*     s) : m_crc(set(s)) {}

				// Implicit cast to u32
				inline operator UINT32() const { return m_crc; }

				// Comparison operators
				inline bool operator ==(CRC32 const& other) const { return m_crc == other.m_crc; }
				inline bool operator < (CRC32 const& other) const { return m_crc <  other.m_crc; }

				// Step by step build
				void Add  (UINT8 _x);
				void Add  (void const* _buffer, int _size);
				void AddSZ(char const* _sz);

				// Set
				void SetRawCRC(UINT32 crc) { m_crc = crc; }

			private:
				UINT32 set(std::string  const& _str);
				UINT32 set(std::wstring const& _str);
				UINT32 set(char const*         _psz);
				UINT32 set(wchar_t const*      _pwsz);

				struct TTable
				{
					TTable();
					UINT32 T[ 256 ];
				};
				static TTable m_TABLE;
				UINT32 m_crc;
			};
		}
	}
}

#include "CRC32.inl"

#endif // CRC32_h__