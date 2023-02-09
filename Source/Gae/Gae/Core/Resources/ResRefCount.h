#ifndef ResRefCount_h__
#define ResRefCount_h__

/*

		OF COURSE THIS IS NOT WORKING. I HAVE AGAIN TO REVIEW HOW TO
		CREATE AN AUTOMATIC REFERENCE COUNTER.
*/


namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			class Resource;


			class ResRefCount
			{
				friend class Resource;	// The owner.

				UINT32 GetRefCount() const;

				virtual ResRefCount* operator =(ResRefCount* pResOther);

			public:
				ResRefCount(){}
				~ResRefCount(){}

				void IncRefCount();
				void DecRefCount();

				UINT32 m_uCount;
			};
		}
	}
}

#include "ResRefCount.inl"

#endif // ResRefCount_h__