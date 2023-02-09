#include "stdafx.h"

namespace Gae
{
	namespace Entities
	{
		Entity3D::Entity3D()
			: m_pMesh(NULL)
		{
		}

		bool Entity3D::Init()
		{
			if(!m_pMesh)
			{
				m_pMesh = static_cast<Gae::Graphics::Mesh*>(Gae::Core::Resources::ResManager::Singleton().GetDefaultMesh());
			}

			return NULL != m_pMesh;
		}

		bool Entity3D::Update()
		{
			static FLOAT fRotStep = 0.0174533f;
			static Quaternionf q = Quaternionf(AngleAxisf(fRotStep, Vector3f::UnitY()));

			AddRotation(q);
			return true;
		}

		bool Entity3D::Uninit()
		{
			if(m_pMesh)
			{
				GAE_DELETESAFE(m_pMesh);
			}
			return TRUE;
		}

		Entity3D::~Entity3D()
		{
		}
	}
}