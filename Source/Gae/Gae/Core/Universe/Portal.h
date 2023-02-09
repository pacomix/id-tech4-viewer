#ifndef Portal_h__
#define Portal_h__

namespace Gae
{
	namespace Entities
	{
		class Camera;
	}

	namespace Graphics
	{
		class Mesh;

		namespace Geometry
		{
			class Frustum;
			class Plane;
			class AABB;
		}
	}

	namespace Core
	{
		namespace Universe
		{
			class PortalArea;

			/*! \brief Geometry that represents a portal.
			*/
			class Portal
			{
			public:
#ifdef _DEBUG
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW
#endif
				Portal();
				virtual ~Portal();

				// 
				void SetMesh(Gae::Graphics::Mesh* pMesh);
				Gae::Graphics::Mesh* GetMesh() const;

				void SetPositiveSideAreaIndex(const int iPositiveSideArea);
				void SetNegativeSideAreaIndex(const int iNegativeSideArea);

				void SetPositiveSideArea(PortalArea* pPositiveSideArea);
				void SetNegativeSideArea(PortalArea* pNegativeSideArea);

				int GetPositiveSideAreaIndex() const;
				int GetNegativeSideAreaIndex() const;

				const PortalArea* GetPositiveSideArea() const;
				const PortalArea* GetNegativeSideArea() const;

				inline const Gae::Graphics::Geometry::Frustum* GetClippedFrustum() const
				{
					return m_pFrustum;
				}

				const Gae::Graphics::Geometry::Frustum* GetClippedFrustum(const Gae::Graphics::Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, bool bPositive) const;
				inline const Gae::Graphics::Geometry::Plane& GetPlane() const;

				inline const RECT& GetClippedPortalRect() const
				{
					return m_Viewport;
				}
#ifdef _DEBUG
					
				mutable Vector4f m_v4fDebugVertex[4];
				void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif
			private:
				int m_iPositiveSideArea;
				int m_iNegativeSideArea;

				PortalArea* m_pPositiveSideArea;
				PortalArea* m_pNegativeSideArea;

				Gae::Graphics::Geometry::Plane* m_pPlane;

				Gae::Graphics::Mesh* m_pMesh;
				Gae::Graphics::Geometry::AABB* m_pAABB;
				Gae::Graphics::Geometry::Frustum* m_pFrustum;

				mutable RECT m_Viewport;	//!< Viewport of this portal. It is usually set when traversing and clipping the frustum against the portal.

			};
		}
	}
}

#include "Portal.inl"
#endif // Portal_h__