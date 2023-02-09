#ifndef Parser_h__
#define Parser_h__

#ifdef _DEBUG
	//#define _DEBUG_PARSER
	#define _DEBUG_REPLACE
#endif

namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			class Parser
			{
			public:
				Parser();
				~Parser();
			
				bool UseFile(const std::wstring& wsFilename, bool bToLower = FALSE, bool bIgnoreComments = TRUE);
				bool UseString(const std::wstring& wsText, bool bToLower = FALSE, bool bIgnoreComments = TRUE);

				const std::wstring& GetFileText() const;

				void SkipSpaces();
				const wchar_t& GetNextChar();
				
				// Return next token. This token will be delimited
				// by using the current delimiters set.
				const std::wstring& GetNextToken();

				// Return the next specified token.
				// The token will be returned if it is in between
				// one of the current delimiters.
				const std::wstring& GetNextToken(const std::wstring& strToken);

				const std::wstring& GetNextString();
				const std::wstring& GetNextNumber();
				const wchar_t& GetNextDelimiter();
				const wchar_t& GetNextDelimiter(const wchar_t& cDelimiter);
				
				const wchar_t&			GetCurrentChar() const;
				const std::wstring&	GetToken() const;
				const wchar_t&			GetDelimiter() const;
				const std::wstring&	GetString() const;

				// Replaces all occurences of the current token with the given replacement string.
				// The read pointer will pointing then just after
				// the first replaced token.
				void ReplaceAll(const std::wstring& strReplacement);

				void SetDelimitersToken(const std::wstring& strDelimiters);
				void SetDelimiterString(const wchar_t& cStringDelimiter);

				void Reset();

				static const std::wstring TAG_END;
				static const wchar_t TAG_CHAR_END;

			private:
				void Init();
				static const wchar_t TAG_SPACE;
				static const wchar_t TAG_STRING;
				std::wstring m_wsDelimiters;
				std::wstring m_wsDelimitersWithSpace;
				wchar_t m_cDelimiterString;

				std::wstring m_wsFileText;
				std::wstring m_wsCurrentToken;
				std::wstring m_wsCurrentString;
				wchar_t m_wsCurrentDelimiter;
				//char m_wsCurrentChar;
				size_t m_stReadPointer;

			};
		}
	}
}

#include "Parser.inl"

#endif // Parser_h__