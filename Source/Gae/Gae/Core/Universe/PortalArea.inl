namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			inline void PortalArea::AddMesh(const Gae::Graphics::Mesh* pMesh)
			{
				// Return if the mesh is already added but show a warning before.
				GAE_ASSERTM(std::find(m_VecMesh.begin(), m_VecMesh.end(), pMesh) == m_VecMesh.end(), L"Mesh was already added!");

				m_VecMesh.push_back(pMesh);
			}

			inline bool PortalArea::IsMeshAdded(const Gae::Graphics::Mesh* pMesh) const
			{
				return (std::find(m_VecMesh.begin(), m_VecMesh.end(), pMesh) != m_VecMesh.end());
			}

			inline void PortalArea::AddPortal(Portal* pPortal)
			{
				if(std::find(m_VecPortal.begin(), m_VecPortal.end(), pPortal) != m_VecPortal.end()) return;

				m_VecPortal.push_back(pPortal);

			}

			inline void PortalArea::DelMesh(const Gae::Graphics::Mesh* pMesh)
			{
				GAE_ASSERTM(std::find(m_VecMesh.begin(), m_VecMesh.end(), pMesh) != m_VecMesh.end(), L"Mesh doesn't exist in this portal area!!");
				DeqMesh::iterator itFound = std::find(m_VecMesh.begin(), m_VecMesh.end(), pMesh);
				if(itFound == m_VecMesh.end()) return;
				m_VecMesh.erase(itFound);
			}

			inline const PortalArea::DeqMesh& PortalArea::GetMeshes() const
			{
				return m_VecMesh;
			}

			inline const PortalArea::VecPortal& PortalArea::GetPortals() const
			{
				return m_VecPortal;
			}			

			/*
#ifdef _DEBUG

			inline const DeqMesh& PortalArea::GetMeshesPortal() const
			{
				return m_DeqPortalMeshes;
			}
#endif
			*/
		}
	}
}