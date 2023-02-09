namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			void ShadowVolume::SetCountCaps(const UINT32 uVal)
			{
				m_uCountCaps = uVal;
			}

			void ShadowVolume::SetCountCapsFront(const UINT32 uVal)
			{
				m_uCountCapsFront = uVal;
			}

			void ShadowVolume::SetPlaneBits(const UINT32 uVal)
			{
				m_uPlaneBits = uVal;
			}

			void ShadowVolume::SetMesh(Mesh* pMesh)
			{
				GAE_ASSERTM(NULL != pMesh, L"Given mesh is not valid! NULL!");
				m_pMesh = pMesh;
			}

			const UINT32 ShadowVolume::GetCountCaps() const
			{
				return m_uCountCaps;
			}

			const UINT32 ShadowVolume::GetCountCapsFront() const
			{
				return m_uCountCapsFront;
			}

			const UINT32 ShadowVolume::GetPlaneBits() const
			{
				return m_uPlaneBits;
			}

			const Mesh* ShadowVolume::GetMesh() const
			{
				return m_pMesh;
			}
		}
	}
}