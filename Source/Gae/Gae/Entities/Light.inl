namespace Gae
{
	namespace Entities
	{
		using namespace Gae::Graphics;

		/* inline */ bool Light::IsOn() const
		{
			return m_bOn;
		}

		/* inline */ void Light::Set(bool bOn)
		{
			m_bOn = bOn;
		}

		/* inline */ void Light::SetShadowVolume(const Geometry::ShadowVolume* pShadowVolume)
		{
			GAE_ASSERTM(NULL != pShadowVolume, L"Invalida shadow volume. NULL!!!");
			GAE_ASSERTM(NULL == m_pShadowVolume, L"A shadow volume was previously assigned. Is this ok?");

			m_pShadowVolume = pShadowVolume;
		}

		/* inline */ const Geometry::ShadowVolume* Light::GetShadowVolume() const
		{
			return m_pShadowVolume;
		}

		/* inline */ const Vector3f& Light::GetOrigin() const
		{
			return m_v3fOrigin;
		}

		/* inline */ const Vector3f& Light::GetCenter() const
		{
			return m_v3fCenter;
		}

		/* inline */ const Vector3f& Light::GetColor() const
		{
			return m_v3fColor;
		}

		/* inline */ const Vector3f& Light::GetRadius() const
		{
			return m_v3fRadius;
		}

		/* inline */ void Light::SetOrigin(const Vector3f& v3fOrigin)
		{
			m_v3fOrigin = v3fOrigin;
			this->SetPosition(m_v3fOrigin);
		}

		/* inline */ void Light::SetCenter(const Vector3f& v3fCenter)
		{
			m_v3fCenter = v3fCenter;
		}

		/* inline */ void Light::SetColor(const Vector3f& v3fColor)
		{
			m_v3fColor = v3fColor;
		}

		/* inline */ void Light::SetRadius(const Vector3f& v3fRadius)
		{
			m_v3fRadius = v3fRadius;
			this->SetScale(m_v3fRadius);
		}

		/* inline */ void Light::AddMesh(const Mesh* pMesh)
		{
			GAE_ASSERTM(NULL != pMesh, L"Invalid mesh! NULL!!!");
			GAE_ASSERTM(std::find(m_VecMesh.begin(), m_VecMesh.end(), pMesh) == m_VecMesh.end(), L"Mesh was already added to this light!");

			m_VecMesh.push_back(pMesh);

			if(pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
			{
				m_VecMeshOpaque.push_back(pMesh);
			}
			else if(pMesh->GetMaterial()->FlagEnabled(Material::M_DECAL))
			{
				m_VecMeshDecal.push_back(pMesh);
			}
			else if(pMesh->GetMaterial()->FlagEnabled(Material::M_BLEND))
			{
				m_VecMeshTranslucent.push_back(pMesh);
			}
			else
			{
				m_VecMeshOpaque.push_back(pMesh);
			}
		}

		void Light::ComputeMeshOrder()
		{
			m_VecMesh.clear();
			for each(const Gae::Graphics::Mesh* pMesh in m_VecMeshOpaque)
			{
				m_VecMesh.push_back(pMesh);
			}
			
			for each(const Gae::Graphics::Mesh* pMesh in m_VecMeshDecal)
			{
				m_VecMesh.push_back(pMesh);
			}
			
			for each(const Gae::Graphics::Mesh* pMesh in m_VecMeshTranslucent)
			{
				m_VecMesh.push_back(pMesh);
			}

			GAE_ASSERTM(!m_VecMesh.empty(), L"No hay ninguna mesh asociada a esta luz!!!");
		}

		const Light::VecMesh& Light::GetMeshes() const
		{
			return m_VecMesh;
		}

		void Light::ComputeAABB()
		{
			//const Vector3f v3fPos(m_v3fOrigin + m_v3fCenter);
			// TODO: This works but it shouldn't!!!!!
			const Vector3f v3fPos(m_v3fOrigin);
			m_AABB.SetValues(v3fPos - m_v3fRadius, v3fPos + m_v3fRadius);
		}

		const Gae::Graphics::Geometry::AABB& Light::GetAABB() const
		{
			return m_AABB;
		}

		RECT& Light::GetAABBRectForModification()
		{
			return m_ScreenRect;
		}

		const RECT* Light::GetAABBRect() const
		{
			return &m_ScreenRect;
		}
	}
}