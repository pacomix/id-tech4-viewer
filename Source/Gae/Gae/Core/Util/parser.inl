namespace Gae
{
	namespace Core
	{
		namespace Util
		{
			inline const std::wstring& Parser::GetFileText() const
			{
				return m_wsFileText;
			}

			inline void Parser::Reset()
			{
				m_stReadPointer = 0;
			}

			inline void Parser::SkipSpaces()
			{
				if(m_stReadPointer == std::wstring::npos) return;
				while(GetCurrentChar() == TAG_SPACE && GetNextChar() == TAG_SPACE);
				//m_stReadPointer--;	// We have to substract 1 because the last call to GetNextwchar_t leaves the read pointer one step over the first non-space wchar_tacter.
			}

			inline const wchar_t& Parser::GetNextChar()
			{
				if(m_stReadPointer+1 >= m_wsFileText.size()) 
				{
					m_stReadPointer = m_wsFileText.size();
					return TAG_CHAR_END;
				}
				return m_wsFileText.at(++m_stReadPointer);
			}

			inline const wchar_t& Parser::GetCurrentChar() const
			{
				if(m_stReadPointer == m_wsFileText.size()) 
				{
					return TAG_CHAR_END;
				}
				return m_wsFileText.at(m_stReadPointer);
			}

			inline const std::wstring& Parser::GetToken() const
			{
				return m_wsCurrentToken;
			}

			inline const wchar_t& Parser::GetDelimiter() const
			{
				return m_wsCurrentDelimiter;
			}

			inline const wchar_t& Parser::GetNextDelimiter(const wchar_t& cDelimiter)
			{
				m_stReadPointer = m_wsFileText.find(cDelimiter, m_stReadPointer);
				if(m_stReadPointer == std::wstring::npos)
				{
					return TAG_CHAR_END;
				}
				m_wsCurrentDelimiter = m_wsFileText.at(m_stReadPointer++);
				return m_wsCurrentDelimiter;
			}

			inline const std::wstring& Parser::GetString() const
			{
				return m_wsCurrentString;
			}

			inline void Parser::SetDelimitersToken(const std::wstring& strDelimiters)
			{
				m_wsDelimiters.reserve(strDelimiters.size());
				m_wsDelimiters = strDelimiters;
				m_wsDelimitersWithSpace = m_wsDelimiters + TAG_SPACE;
			}

			inline void Parser::SetDelimiterString(const wchar_t& cStringDelimiter)
			{
				m_cDelimiterString = cStringDelimiter;
			}
		}
	}
}