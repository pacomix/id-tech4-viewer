namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			inline Gae::Graphics::Mesh* Portal::GetMesh() const
			{
				return m_pMesh;
			}

			inline void Portal::SetPositiveSideAreaIndex(const int iPositiveSideAreaIndex)
			{
				m_iPositiveSideArea = iPositiveSideAreaIndex;
			}

			inline void Portal::SetNegativeSideAreaIndex(const int iNegativeSideAreaIndex)
			{
				m_iNegativeSideArea = iNegativeSideAreaIndex;
			}

			inline void Portal::SetPositiveSideArea(PortalArea* pPositiveSideArea)
			{
				GAE_ASSERT(NULL != pPositiveSideArea);
				m_pPositiveSideArea = pPositiveSideArea;
			}

			inline void Portal::SetNegativeSideArea(PortalArea* pNegativeSideArea)
			{
				GAE_ASSERT(NULL != pNegativeSideArea);
				m_pNegativeSideArea = pNegativeSideArea;
			}

			inline int Portal::GetPositiveSideAreaIndex() const
			{
				return m_iPositiveSideArea;
			}

			inline int Portal::GetNegativeSideAreaIndex() const
			{
				return m_iNegativeSideArea;
			}

			inline const PortalArea* Portal::GetPositiveSideArea() const
			{
				return m_pPositiveSideArea;
			}

			inline const PortalArea* Portal::GetNegativeSideArea() const
			{
				return m_pNegativeSideArea;
			}

			const Gae::Graphics::Geometry::Plane& Portal::GetPlane() const
			{
				return *m_pPlane;
			}
		}
	}
}