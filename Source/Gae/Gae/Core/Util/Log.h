#ifndef Log_h__
#define Log_h__
#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			static void Log(const wchar_t* pStr, ...)
			{
				va_list args;
				va_start(args, pStr);

				std::wstring strOut;

				// Obtains the size of the string to be printed out and fill the wstring object
				strOut.resize(_vscwprintf(pStr, args) + 1);
				_vsnwprintf_s(const_cast<wchar_t*>(strOut.data()), strOut.size(), _TRUNCATE, pStr, args);

				va_end(args);

				// Print out the string in the debug output.
				OutputDebugString(strOut.c_str());

				std::wstring str2(strOut.length(), L' ');
				std::copy(strOut.begin(), strOut.end(), str2.begin());

				std::cout << str2.c_str();
			}

			static std::wstring LogString(const wchar_t* pStr, ...)
			{
				va_list args;
				va_start(args, pStr);

				std::wstring strOut;

				// Obtains the size of the string to be printed out and fill the wstring object
				strOut.resize(_vscwprintf(pStr, args) + 1);
				_vsnwprintf_s(const_cast<wchar_t*>(strOut.data()), strOut.size(), _TRUNCATE, pStr, args);

				va_end(args);

				return strOut;
			}
		}
	}
}


#endif // Log_h__