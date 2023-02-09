#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			const std::wstring Parser::TAG_END = L"";
			const wchar_t Parser::TAG_SPACE = L' ';
			const wchar_t Parser::TAG_STRING = L'"';
			const wchar_t Parser::TAG_CHAR_END = 0L;

			Parser::Parser()
			{
				Init();
			}

			void Parser::Init()
			{
				m_stReadPointer = 0;
				m_wsCurrentDelimiter = 0;
				m_wsCurrentToken.reserve(2048);
				m_wsCurrentToken = TAG_END;
				m_cDelimiterString = TAG_STRING;
				m_wsDelimiters.clear();
				m_wsDelimitersWithSpace.clear();
				m_wsFileText.clear();
			}

			bool Parser::UseFile(const std::wstring& wsFilename, bool bToLower/* = false */, bool bIgnoreComments/* = true */)
			{
				Init();
				std::ifstream FileLevel;

				FileLevel.open(wsFilename.c_str(), std::ios::ate);
				GAE_ASSERTM(TRUE == FileLevel.is_open(), L"The file can not be opened: [%s]\n", wsFilename.c_str());
				if(!FileLevel.is_open()) return false;

				// Gets the file size. 
				size_t iSize = FileLevel.tellg();
				FileLevel.seekg(0);

				// Reserve memory for the full file content.
				m_wsFileText.clear();
				m_wsFileText.reserve(iSize);

				// Reserve some space for each line.
				std::string wsTemp;
				wsTemp.reserve(2048);
				while(!FileLevel.eof())
				{	// Retrieve each line of the file and get rid of the comment-outs.
					getline(FileLevel, wsTemp);

					if(bIgnoreComments)
					{
						if(wsTemp.size() >= 2)
						{	// Ignore the line if it starts with a line comment-out
							if(wsTemp.at(0) == '/' && wsTemp.at(1) == '/') continue;

							// Check for a comment-out block. 
							// If we have this content in the file:
							//
							//		/* comment */
							//		Hello! this is not a comment-out block.
							//		/* This is more commented-out content
							//			even more commented-out content
							// even more comment */ useful content /* blablabla */
							//
							// This code will store:
							//
							//		Hello! This is not a comment-out block.
							//		useful content
							//
							size_t stStartPos = 0;
							size_t stEndPos = 0;
							std::string wsCommentFree;
							while((stStartPos = wsTemp.find("/*", stEndPos)) != std::wstring::npos)
							{	// If we didn't reach the end of the line and the line contains a 
								// start comment-out block, store the content since the last end
								// comment-out block characters ("*/") until the beginning of the
								// start comment-out block characters ("/*").
								wsCommentFree += wsTemp.substr(stEndPos, stStartPos-stEndPos);
								stEndPos = stStartPos + 2;

								while((stEndPos = wsTemp.find("*/", stEndPos)) == std::wstring::npos)
								{	// Continue reading until we do not find a end comment-out block
									// characters ("*/")
									stEndPos = 0;
									getline(FileLevel, wsTemp);
									if(FileLevel.eof())
									{
										break;
									}
								}

								// We found the end block comment or we reached the end of the file.
								GAE_ASSERTM(FALSE == FileLevel.eof(), L"End of file! The end comment block characters '*/' were not found!!");
								stEndPos += 2;
							}

							// It means that we arrived to the end of the line without finding
							// any more block comment start chars. Store the rest of the line.
							wsTemp = wsCommentFree + wsTemp.substr(stEndPos);

							// Ignore the comment if it is in the middle of the string.
							stStartPos = wsTemp.find("//", 0);
							if(stStartPos != std::wstring::npos)
							{
								wsTemp = wsTemp.substr(0, stStartPos);
							}

						}
					}
					wsTemp += "\n";	// Get line doesn't add a new line wchar_t character so we have to add it.
					m_wsFileText.append(wsTemp.begin(), wsTemp.end());
					wsTemp.clear();
				}

				if(bToLower)
				{	// to lower case.
					std::locale loc;
					for(size_t i = 0; i < m_wsFileText.size(); ++i)
					{
						m_wsFileText[i] = std::tolower(m_wsFileText[i], loc);
					}
				}
				FileLevel.close();

				return TRUE;
			}

			bool Parser::UseString(const std::wstring& wsText, bool bToLower /* = false */, bool bIgnoreComments/* = TRUE */)
			{
				Init();
				m_wsFileText = wsText;
				return TRUE;
			}

			Parser::~Parser()
			{
			}

			const std::wstring& Parser::GetNextToken()
			{
				GAE_ASSERTM(FALSE == m_wsDelimiters.empty(), L"In order to detect a token at least one delimiter must be set!\n");

				SkipSpaces();

				// Sets the read pointer at the beginning of a valid token.
				m_stReadPointer = m_wsFileText.find_first_not_of(m_wsDelimitersWithSpace, m_stReadPointer);
				if(m_stReadPointer == std::wstring::npos)
				{
					m_wsCurrentToken = TAG_END;
					return TAG_END;
				}

				size_t stAux = m_wsFileText.find_first_of(m_wsDelimitersWithSpace, m_stReadPointer);
				m_wsCurrentToken = m_wsFileText.substr(m_stReadPointer, stAux - m_stReadPointer);

				m_stReadPointer = stAux;

#ifdef _DEBUG_PARSER
				GAE_TRACE(L"Token: [%s]\n", m_wsCurrentToken.c_str());
#endif
				return m_wsCurrentToken;
			}

			const std::wstring& Parser::GetNextToken(const std::wstring& strToken)
			{
				GAE_ASSERTM(FALSE == m_wsDelimiters.empty(), L"In order to detect a token at least one delimiter must be set!\n");

				// Skips until we find the token or reaches the end of the file.
				while(GetNextToken() != TAG_END && GetToken() != strToken);
				
				if(GetToken() == strToken)
				{
#ifdef _DEBUG_PARSER
					GAE_TRACE(L"Token: [%s]\n", m_wsCurrentToken.c_str());
#endif
					return GetToken();
				}
				else
				{
					return TAG_END;
				}
			}

			const std::wstring& Parser::GetNextString()
			{
				// Sets the read pointer at the beginning of a valid string token.
				m_stReadPointer = m_wsFileText.find(m_cDelimiterString, m_stReadPointer);
				if(m_stReadPointer == std::wstring::npos)
				{
					m_wsCurrentString = TAG_END;
					return TAG_END;
				}
				m_stReadPointer++;	// Sets the read pointer at the beginning of the string content.

				// Sets the read pointer at the end of the string.
				size_t stAux = m_wsFileText.find(m_cDelimiterString, m_stReadPointer);
				GAE_ASSERTM(m_stReadPointer != std::wstring::npos, L"Error! Impossible to find an end string delimiter!\n");

				m_wsCurrentString = m_wsFileText.substr(m_stReadPointer, stAux - m_stReadPointer);
				m_stReadPointer = stAux+1;	// Sets the read pointer just after the string end delimiter.

#ifdef _DEBUG_PARSER
				GAE_TRACE(L"Token: [%s]\n", m_wsCurrentString.c_str());
#endif
				return m_wsCurrentString;
			}

			const std::wstring& Parser::GetNextNumber()
			{
				GAE_ASSERTM(FALSE == m_wsDelimiters.empty(), L"In order to detect a token at least one delimiter must be set!\n");

				// Skips until we find the token or reaches the end of the file.
				static const std::wstring strNumbers = L"+-.0123456789";
				while(GetNextToken() != TAG_END && strNumbers.find_first_of(GetToken().at(0)) == std::wstring::npos);

				if(GetToken() != TAG_END)
				{
#ifdef _DEBUG_PARSER
					GAE_TRACE(L"Token: [%s]\n", m_wsCurrentToken.c_str());
#endif
					return GetToken();
				}
				else
				{
					return TAG_END;					
				}

			}

			const wchar_t& Parser::GetNextDelimiter()
			{
				GAE_ASSERTM(FALSE == m_wsDelimiters.empty(), L"In order to detect a token at least one delimiter must be set!\n");

				SkipSpaces();

				// Sets the read pointer at the beginning of a valid delimiter.
				m_stReadPointer = m_wsFileText.find_first_of(m_wsDelimiters, m_stReadPointer);

				if(m_stReadPointer == std::wstring::npos)
				{
					m_wsCurrentDelimiter = TAG_CHAR_END;
					return TAG_CHAR_END;
				}

				m_wsCurrentDelimiter = m_wsFileText.at(m_stReadPointer++);

#ifdef _DEBUG_PARSER
				GAE_TRACE(L"Delimiter: [%c]\n", m_wsCurrentDelimiter);
#endif
				return m_wsCurrentDelimiter;
			}

			void Parser::ReplaceAll(const std::wstring& strReplacement)
			{
				Reset();

				while((m_stReadPointer = m_wsFileText.find(m_wsCurrentToken, m_stReadPointer)) != std::wstring::npos)
				{
					m_wsFileText.replace(m_stReadPointer, m_wsCurrentToken.size(), strReplacement);
				}

				m_stReadPointer = m_wsFileText.find(strReplacement, 0);
#ifdef _DEBUG_REPLACE
				GAE_TRACE(L"File after ReplaceAll:\n%s\n", m_wsFileText.c_str());
#endif
			}

		}
	}
}