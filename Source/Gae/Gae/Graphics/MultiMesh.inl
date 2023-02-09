namespace Gae
{
	namespace Graphics
	{
		inline bool MultiMesh::Init()
		{
		}

		inline void MultiMesh::AddMesh(Mesh* pMesh)
		{
			GAE_ASSERT(NULL != pMesh);
			GAE_ASSERT(std::find(m_VecMeshes.begin(), m_VecMeshes.end(), pMesh) == m_VecMeshes.end());

			m_VecMeshes.push_back(pMesh);
		}

		inline void MultiMesh::SetMeshCount(int iCount)
		{
			m_VecMeshes.reserve(iCount);
		}

		inline const MultiMesh::VecMeshes& MultiMesh::GetMeshes() const
		{
			return m_VecMeshes;
		}

		inline bool MultiMesh::Uninit()
		{
			for each(Mesh* pMesh in m_VecMeshes)
			{
				GAE_DELETESAFE(pMesh);
			}

			m_VecMeshes.clear();

			return TRUE;
		}
	}
}