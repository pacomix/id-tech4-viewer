#ifndef Frustum_h__
#define Frustum_h__

namespace Gae
{
	namespace Renderer
	{
		class Driver;
	}
	namespace Graphics
	{
		namespace Geometry
		{
			class Plane;

			class Frustum
#ifdef _DEBUG_DRAW
				:	public Gae::Renderer::IDrawDebug
#endif
			{
			public:
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW

				enum E_PLANE
				{
					PL_TOP = 0,
					PL_BOTTOM,
					PL_LEFT,
					PL_RIGHT,
					PL_NEAR,
					PL_FAR
				};

				Frustum();
				Frustum(UINT32 uPlaneCount);
				virtual ~Frustum();

				void SetFromMatrixViewProjection(const Matrix4f& mtx44fViewProj);

				// TODO: In order to not create and destroy memory. Lets create the planes inside a vector and add a warning
				//	every time it needs to be expanded. Create the vector with a item reserve of 64 elements in example.
				void ReservePlanes(UINT32 uPlanes);

				bool IsInside(const Vector3f& v3fPoint) const;
				bool IsInside(const Geometry::AABB& aabb) const;
				bool IsInside(const Vector3f& v3fPoint, UINT32 uPlane) const;

				bool GetIntersection(UINT32 uPlane1, UINT32 uPlane2, UINT32 uPlane3, Vector3f* pv3fIntersection) const;

				inline const UINT32 GetPlaneCount() const
				{
					return m_uPlaneCount;
				}

				inline const Plane& GetPlane(UINT32 uPlane) const
				{
					GAE_ASSERTM(uPlane < m_uPlaneCount, L"Plane index out of range! Must be: 0 <= %u < %u", uPlane, m_uPlaneCount);

					return *m_aPlanes[uPlane];
				}

				inline void SetPlane(UINT32 uPlane, const Vector4f& v4fNormalDistance) const
				{
					GAE_ASSERTM(uPlane < m_uPlaneCount, L"Plane index out of range! Must be: 0 <= %u < %u", uPlane, m_uPlaneCount);

					m_aPlanes[uPlane]->m_v4fNormalDistance = v4fNormalDistance;
				}

				inline void SetPlane(UINT32 uPlane, const Vector3f& v3fNormal, float fDistance) const
				{
					GAE_ASSERTM(uPlane < m_uPlaneCount, L"Plane index out of range! Must be: 0 <= %u < %u", uPlane, m_uPlaneCount);

					m_aPlanes[uPlane]->m_v3fNormal = v3fNormal;
					m_aPlanes[uPlane]->m_fDistance = fDistance;
				}

				inline Sphere& GetBoundingSphere()
				{
					return m_BoundingSphere;
				}

				// Returns (in the param) the closest vertex of the aabb to the specified plane.
				void GetClosestVtx(const AABB& aabb, UINT32 uPlane, Vector3f& v3fClosestVtx) const;
				void GetClosestVtx(const AABB& aabb, UINT32 uPlane, Vector4f& v4fClosestVtx) const;

#ifdef _DEBUG_DRAW
				void SetVertexes(const Vector4f* pv4fVertexes, bool bKeepColors = false);
				void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif

			private:
				bool TestAABB(const Vector3f& v3fMin, const Vector3f& v3fMax, UINT32 uPlane) const;
#define DEFAULT_PLANE_COUNT 64
				UINT32 m_uCurrentMaxPlaneCount;
				std::vector< Plane* > m_aPlanes;
				UINT32 m_uPlaneCount;

				Sphere m_BoundingSphere;

#ifdef _DEBUG_DRAW
				//Vector4f m_v4fVertexes[8];
				Vector4f* m_v4fVertexes;
#endif

				
			};
		}
	}
}
#endif // Frustum_h__