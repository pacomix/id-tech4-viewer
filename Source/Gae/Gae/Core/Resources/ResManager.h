#ifndef ResManager_h__
#define ResManager_h__

#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		class Mesh;
	}

	namespace Core
	{
		namespace Resources
		{
			class ResManager
			{
				SINGLETON_DECL(ResManager);

			public:
				Resource* Load(const wchar_t* pFilename);
				Resource* GetDefaultMesh() const;

			private:
				Resource* Load(UINT32 uResCrc);

				typedef std::map< UINT32, Resource* > MapResources;
				MapResources m_MapRes;

			};
		}
	}
}

#endif // ResManager_h__