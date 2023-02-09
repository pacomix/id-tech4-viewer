#ifndef Resource_h__
#define Resource_h__

namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			class Resource : public ResRefCount
			{

			public:
				typedef ResRefCount super;
				friend class ResManager;	// The owner.

				Resource() : super() {}
				virtual ~Resource(){}

				inline void SetTAG(const std::wstring& wsTag)
				{
					m_wsTAG = wsTag;
					m_uTagCRC32 = Gae::Core::Util::CRC32::CRC32(m_wsTAG.c_str());
				}

				inline const std::wstring& GetTAG() const
				{
					return m_wsTAG;
				}

				inline UINT32 GetTagCRC32() const
				{
					return m_uTagCRC32;
				}

			private:
				std::wstring m_wsTAG;
				UINT32 m_uTagCRC32;
			};
		}
	}
}

#endif // Resource_h__