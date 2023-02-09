namespace Gae
{
	namespace Graphics
	{

		inline void Mesh::SetVertexBuffer(Geometry::VertexBuffer* pVB)
		{
			GAE_ASSERT(NULL != pVB);

			m_pVertexBuffer = pVB;
		}

		inline void Mesh::SetIndexBuffer(Geometry::IndexBuffer* pIB)
		{
			GAE_ASSERT(NULL != pIB);

			m_pIndexBuffer = pIB;
		}

		inline void Mesh::SetMaterial(Material* pMaterial)
		{
			GAE_ASSERTM(NULL != pMaterial, L"No Material!!!");
			m_pMaterial = pMaterial;
		}

		inline Material* Mesh::GetMaterial() const
		{
			return m_pMaterial;
		}

		inline Geometry::VertexBuffer* Mesh::GetVertexBuffer() const
		{
			return m_pVertexBuffer;
		}

		inline Geometry::IndexBuffer* Mesh::GetIndexBuffer() const
		{
			return m_pIndexBuffer;
		}

		inline bool Mesh::IsUsingTransparency() const
		{
			return FALSE;//;m_pTexture ? m_pTexture->IsAlpha() : FALSE;
		}

		inline const Geometry::AABB& Mesh::GetAABB() const
		{
			return m_AABB;
		}

#ifdef _DEBUG
		inline void Mesh::ToDebugOutput()
		{
			if(!(m_pVertexBuffer || m_pIndexBuffer)) return;

			m_pVertexBuffer->DebugOutput();
			m_pIndexBuffer->DebugOutput();
		}
#endif

	}
}