#include "stdafx.h"
#include "ResManager.h"

#include "Graphics/Mesh.h"

namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			SINGLETON_IMPL(ResManager);

			bool ResManager::Init()
			{
				static bool bFirstInit = TRUE;

				if(!bFirstInit) return FALSE;
				
				bFirstInit = FALSE;

				return TRUE;
			}

			Resource* ResManager::Load(const wchar_t* pFilename)
			{
				GAE_ASSERTM(pFilename, L"Invalid file: [%s]", pFilename);
				// TODO: Identify shared resource types. IE: Mesh, Texture, Animation, etc...

				UINT32 uResCrc32 = Gae::Core::Util::CRC32(pFilename);
				MapResources::iterator itFound;// = std::find(m_MapRes.begin(), m_MapRes.end(), uResCrc32);
				if(itFound == m_MapRes.end())
				{	// Resource wasn't previously loaded, so... load it!
				}
				return NULL;
			}

			Resource* ResManager::GetDefaultMesh() const
			{
				return Gae::Graphics::Geometry::Procedural::Singleton().GetCube();
			}

			bool ResManager::Uninit()
			{
				// TODO: throw assert if not all the resources where destroyed...
				return TRUE;
			}

		}
	}
}