namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			inline UINT32 ResRefCount::GetRefCount() const
			{
				return m_uCount;
			}

			inline ResRefCount* ResRefCount::operator =(ResRefCount* pResOther)
			{
				pResOther->IncRefCount();
				return pResOther;
			}

			inline void ResRefCount::IncRefCount()
			{
				m_uCount++;
			}

			inline void ResRefCount::DecRefCount()
			{
				m_uCount--;
			}

		}
	}
}