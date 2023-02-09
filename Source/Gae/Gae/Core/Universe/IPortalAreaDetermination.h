#ifndef IPortalAreaDetermination_h__
#define IPortalAreaDetermination_h__

namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			class IPortalAreaDetermination
			{
			public:
				virtual int GetAreaPortal(const Vector3f& v3fPos) const = 0;
			};
		}
	}
}


#endif // IPortalAreaDetermination_h__