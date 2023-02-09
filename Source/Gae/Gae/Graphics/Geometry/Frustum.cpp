#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			Frustum::Frustum()
			{
				m_uPlaneCount = 6;
				m_uCurrentMaxPlaneCount = DEFAULT_PLANE_COUNT;
				//m_aPlanes = new Plane[m_uPlaneCount];
				m_aPlanes.reserve(m_uCurrentMaxPlaneCount);	// 64 = default value to avoid memory fragmentation
				
				for(UINT32 i = 0; i < m_uCurrentMaxPlaneCount; ++i)
				{
					m_aPlanes.push_back(new Plane);
				}

#ifdef _DEBUG_DRAW
				m_v4fVertexes = new Vector4f[(m_uPlaneCount-2) * 2];
#endif
			}

			Frustum::Frustum(UINT32 uPlaneCount)
			{
				m_uPlaneCount = uPlaneCount;
				m_uCurrentMaxPlaneCount = DEFAULT_PLANE_COUNT;
				//m_aPlanes = new Plane[uPlaneCount];
				m_aPlanes.reserve(m_uCurrentMaxPlaneCount);
				for(UINT32 i = 0; i < m_uCurrentMaxPlaneCount; ++i)
				{
					m_aPlanes.push_back(new Plane);
				}
#ifdef _DEBUG
				m_v4fVertexes = new Vector4f[(m_uPlaneCount-2) * 2];
#endif
			}

			Frustum::~Frustum()
			{
				//GAE_DELETESAFEARRAY(m_aPlanes);
				for(UINT32 i = 0; i < m_uCurrentMaxPlaneCount; ++i)
				{
					GAE_DELETESAFE(m_aPlanes[i]);
				}
#ifdef _DEBUG
				GAE_DELETESAFEARRAY(m_v4fVertexes);
#endif
			}

			void Frustum::ReservePlanes(UINT32 uPlanes)
			{
				//GAE_DELETESAFEARRAY(m_aPlanes);
#ifdef _DEBUG
				GAE_DELETESAFEARRAY(m_v4fVertexes);
#endif
				if(uPlanes > m_uCurrentMaxPlaneCount)
				{
					GAE_ASSERTM(false, L"Please increment Frustum::DEFAULT_PLANE_COUNT number for the Frustum objects!");
					m_aPlanes.reserve(uPlanes);
					for(UINT32 i = m_uCurrentMaxPlaneCount; i < uPlanes; ++i)
					{
						m_aPlanes.push_back(new Plane);
					}
					m_uCurrentMaxPlaneCount = uPlanes;
				}
				m_uPlaneCount = uPlanes;

				//if(uPlanes == 0) return;
				//m_aPlanes = new Plane[uPlanes];

#ifdef _DEBUG
				m_v4fVertexes = new Vector4f[(m_uPlaneCount-2) * 2];
#endif
			}

			void Frustum::SetFromMatrixViewProjection(const Matrix4f& mtx44fViewProj)
			{
				GAE_ASSERTM(m_uPlaneCount == 6, L"Method only valid with a 6 planes frustum.");

				// \note I access the matrix elements with an offset to avoid col/row matrix order issues.
				// Left clipping plane
				const float* fMtx = mtx44fViewProj.data();

				m_aPlanes[PL_LEFT]->m_v3fNormal << fMtx[3] + fMtx[0], fMtx[7] + fMtx[4], fMtx[11] + fMtx[8];
				m_aPlanes[PL_LEFT]->m_fDistance = fMtx[15] + fMtx[12];

				// Right clipping plane 
				m_aPlanes[PL_RIGHT]->m_v3fNormal << fMtx[3] - fMtx[0], fMtx[7] - fMtx[4], fMtx[11] - fMtx[8];
				m_aPlanes[PL_RIGHT]->m_fDistance = fMtx[15] - fMtx[12];

				// Top clipping plane 
				m_aPlanes[PL_TOP]->m_v3fNormal << fMtx[3] - fMtx[1], fMtx[7] - fMtx[5], fMtx[11] - fMtx[9];
				m_aPlanes[PL_TOP]->m_fDistance = fMtx[15] - fMtx[13];

				// Bottom clipping plane 
				m_aPlanes[PL_BOTTOM]->m_v3fNormal << fMtx[3] + fMtx[1], fMtx[7] + fMtx[5], fMtx[11] + fMtx[9];
				m_aPlanes[PL_BOTTOM]->m_fDistance = fMtx[15] + fMtx[13];

				// Near clipping plane 
				m_aPlanes[PL_NEAR]->m_v3fNormal << fMtx[3] + fMtx[2], fMtx[7] + fMtx[6], fMtx[11] + fMtx[10];
				m_aPlanes[PL_NEAR]->m_fDistance = fMtx[15] + fMtx[14];

				// Far clipping plane 
				m_aPlanes[PL_FAR]->m_v3fNormal << fMtx[3] - fMtx[2], fMtx[7]	- fMtx[6], fMtx[11]	- fMtx[10];
				m_aPlanes[PL_FAR]->m_fDistance = fMtx[15] - fMtx[14];

				/*	// Clearer way but this can lead to matrix errors due to col/row major order.
				m_aPlanes[PL_LEFT].m_v4fNormalDistance		= mtx44fViewProj.col(0) + mtx44fViewProj.col(3);
				m_aPlanes[PL_RIGHT].m_v4fNormalDistance		= -mtx44fViewProj.col(0) + mtx44fViewProj.col(3);
				m_aPlanes[PL_TOP].m_v4fNormalDistance		= -mtx44fViewProj.col(1) + mtx44fViewProj.col(3);
				m_aPlanes[PL_BOTTOM].m_v4fNormalDistance	= mtx44fViewProj.col(1) + mtx44fViewProj.col(3);
				m_aPlanes[PL_FAR].m_v4fNormalDistance		= -mtx44fViewProj.col(2) + mtx44fViewProj.col(3);
				m_aPlanes[PL_NEAR].m_v4fNormalDistance		= mtx44fViewProj.col(2) + mtx44fViewProj.col(3);
				*/

				for(int i = 0; i < 6; ++i)
				{
					float fLength = 1.0f / m_aPlanes[i]->m_v3fNormal.norm();
					m_aPlanes[i]->m_v3fNormal.normalize();
					m_aPlanes[i]->m_fDistance *= fLength;
				}
			}

			bool Frustum::IsInside(const Vector3f& v3fPoint, UINT32 uPlane) const
			{
				GAE_ASSERTM(uPlane < m_uPlaneCount, L"Plane index out of range! Must be: 0 <= %u < %u", uPlane, m_uPlaneCount);

				return m_aPlanes[uPlane]->GetDistanceToPlane(v3fPoint) >= 0.0f;
			}

			bool Frustum::IsInside(const Vector3f& v3fPoint) const
			{
				// TODO Optimize by unrolling for loop
				for(UINT32 i = 0; i < m_uPlaneCount; ++i)
				{
					if(m_aPlanes[i]->GetDistanceToPlane(v3fPoint) < 0.0f) return false;
				}
				return true;
			}

			bool Frustum::IsInside(const Geometry::AABB& aabb) const
			{
				// TODO: Try the radar approach.
				// Early detection of frustum sphere - aabb sphere collision.
				if(m_BoundingSphere.GetRadius() > 0.0f && !aabb.GetBoundingSphere().Intersect(m_BoundingSphere))
				{
					return false;
				}

				const Vector3f& v3fMin = aabb.GetMin();
				const Vector3f& v3fMax = aabb.GetMax();

				// TODO Optimize by unrolling the for loop
				for(UINT32 i = 0; i < m_uPlaneCount; ++i)
				{
					if(!TestAABB(v3fMin, v3fMax, i)) return false;
				}

				return true;
			}

			void Frustum::GetClosestVtx(const AABB& aabb, UINT32 uPlane, Vector4f& v4fClosestVtx) const
			{
				Vector3f v3fTemp = v4fClosestVtx.block(0,0,3,1);

				GetClosestVtx(aabb, uPlane, v3fTemp);
				v4fClosestVtx << v3fTemp, 1.0f;
			}

			void Frustum::GetClosestVtx(const AABB& aabb, UINT32 uPlane, Vector3f& v3fClosestVtx) const
			{
				GAE_ASSERTM(uPlane < m_uPlaneCount, L"Incorrect plane index: [%u]", uPlane);

				Plane* pPlane = m_aPlanes.at(uPlane);
				const Vector3f& v3fMin  = aabb.GetMin();
				const Vector3f& v3fMax = aabb.GetMax();

				if(pPlane->m_v3fNormal.x() >= 0.0f)
				{
					v3fClosestVtx.x() = v3fMax.x();
					//minExtreme.x() = v3fMin.x();
				}
				else
				{
					v3fClosestVtx.x() = v3fMin.x();
					//minExtreme.x() = v3fMax.x();
				}

				if(pPlane->m_v3fNormal.y() >= 0.0f)
				{
					v3fClosestVtx.y() = v3fMax.y();
					//minExtreme.y() = v3fMin.y();
				}
				else
				{
					v3fClosestVtx.y() = v3fMin.y();
					//minExtreme.y() = v3fMax.y();
				}

				if(pPlane->m_v3fNormal.z() >= 0.0f)
				{
					v3fClosestVtx.z() = v3fMax.z();
					//minExtreme.z() = v3fMin.z();
				}
				else
				{
					v3fClosestVtx.z() = v3fMin.z();
					//minExtreme.z() = v3fMax.z();
				}
			}
			bool Frustum::TestAABB(const Vector3f& v3fMin, const Vector3f& v3fMax, UINT32 uPlane) const
			{
				GAE_ASSERTM(uPlane < m_uPlaneCount, L"Plane index out of range! Must be: 0 <= %u < %u", uPlane, m_uPlaneCount);

				static Vector3f minExtreme;
				static Vector3f maxExtreme;

				//Plane* pPlane = m_aPlanes[uPlane];
				Plane* pPlane = m_aPlanes.at(uPlane);

				if(pPlane->m_v3fNormal.x() >= 0.0f)
				{
					maxExtreme.x() = v3fMax.x();
					minExtreme.x() = v3fMin.x();
				}
				else
				{
					maxExtreme.x() = v3fMin.x();
					minExtreme.x() = v3fMax.x();
				}

				if(pPlane->m_v3fNormal.y() >= 0.0f)
				{
					maxExtreme.y() = v3fMax.y();
					minExtreme.y() = v3fMin.y();
				}
				else
				{
					maxExtreme.y() = v3fMin.y();
					minExtreme.y() = v3fMax.y();
				}

				if(pPlane->m_v3fNormal.z() >= 0.0f)
				{
					maxExtreme.z() = v3fMax.z();
					minExtreme.z() = v3fMin.z();
				}
				else
				{
					maxExtreme.z() = v3fMin.z();
					minExtreme.z() = v3fMax.z();
				}

				/*
				maxExtreme <<
					(pPlane->m_v3fNormal.x() >= 0.0f ? v3fMax.x() : v3fMin.x()),
					(pPlane->m_v3fNormal.y() >= 0.0f ? v3fMax.y() : v3fMin.y()),
					(pPlane->m_v3fNormal.z() >= 0.0f ? v3fMax.z() : v3fMin.z());

				minExtreme <<
					(pPlane->m_v3fNormal.x() < 0.0f ? v3fMax.x() : v3fMin.x()),
					(pPlane->m_v3fNormal.y() < 0.0f ? v3fMax.y() : v3fMin.y()),
					(pPlane->m_v3fNormal.z() < 0.0f ? v3fMax.z() : v3fMin.z());
				*/

				if (pPlane->GetDistanceToPlane(maxExtreme) < 0.0f)
				{
					return false;
				}

				return true;
			}

			bool Frustum::GetIntersection(UINT32 uPlane1, UINT32 uPlane2, UINT32 uPlane3, Vector3f* pv3fIntersection) const
			{
				/* . = dot product	* = cross product
					dX = distance of any point to the plane
					Nx = Plane normal
					if N2 * N3 = Vector 0 there is no vertex defined for the intersection

									d1 ( N2 * N3 ) + d2 ( N3 * N1 ) + d3 ( N1 * N2 ) 
					P = 	------------------------------------------------------------------------- 
													N1 . ( N2 * N3 )
				*/

				GAE_ASSERTM(uPlane1 < m_uPlaneCount, L"Plane out of bounds!");
				GAE_ASSERTM(uPlane2 < m_uPlaneCount, L"Plane out of bounds!");
				GAE_ASSERTM(uPlane3 < m_uPlaneCount, L"Plane out of bounds!");

				Plane& p1 = *m_aPlanes[uPlane1];
				Plane& p2 = *m_aPlanes[uPlane2];
				Plane& p3 = *m_aPlanes[uPlane3];
				Vector3f d1(-p1.m_fDistance, -p1.m_fDistance, -p1.m_fDistance);
				Vector3f d2(-p2.m_fDistance, -p2.m_fDistance, -p2.m_fDistance);
				Vector3f d3(-p3.m_fDistance, -p3.m_fDistance, -p3.m_fDistance);
				float fDotTemp = p1.m_v3fNormal.dot(p2.m_v3fNormal.cross(p3.m_v3fNormal));
				Vector3f fDen(fDotTemp, fDotTemp, fDotTemp);
				if(fDen == Vector3f::Zero())
				{
					*pv3fIntersection = Vector3f::Zero();
					return false;
				}

				Vector3f v3fNum = 
					(d1.cwise() * (p2.m_v3fNormal.cross(p3.m_v3fNormal))) +
					(d2.cwise() * (p3.m_v3fNormal.cross(p1.m_v3fNormal))) +
					(d3.cwise() * (p1.m_v3fNormal.cross(p2.m_v3fNormal)));

				(*pv3fIntersection) = v3fNum.cwise() / fDen;
				return true;
			}

#ifdef _DEBUG_DRAW
			// \TODO This has to be reviewed due to the new draw debug system. I think by using a 
			//		unit box transformed by the projection matrix give us the frustum shape. Try it.
			void Frustum::SetVertexes(const Vector4f* pv4fVertexes, bool bKeepColor/* = false*/)
			{
				if(bKeepColor)
				{
					for(UINT32 i = 0; i < (m_uPlaneCount-2)*2; ++i)
					{
						m_v4fVertexes[i] = *(pv4fVertexes+i);
					}
				}
				else
				{
					DWORD dColor[8] = 
					{
						/*
						D3DCOLOR_ARGB(64, 0, 0, 0),
						D3DCOLOR_ARGB(64, 0, 0, 0),
						D3DCOLOR_ARGB(64, 0, 0, 0),
						D3DCOLOR_ARGB(64, 0, 0, 0),
						D3DCOLOR_ARGB(64, 255,255,255),
						D3DCOLOR_ARGB(64, 255,255,255),
						D3DCOLOR_ARGB(64, 255,255,255),
						D3DCOLOR_ARGB(64, 255,255,255),
						*/
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
						D3DCOLOR_ARGB(255, 255,255,255),
					};

					if(m_uPlaneCount >= 2)
					{
						for(UINT32 i = 0; i < (m_uPlaneCount-2)*2; ++i)
						{
							m_v4fVertexes[i] = *(pv4fVertexes+i);
							m_v4fVertexes[i](3) = *(reinterpret_cast<float*>(&dColor[i%8]));
						}
					}
				}
			}

			void Frustum::DrawDebug(const Gae::Renderer::Driver* pRenderer) const
			{
				// Triangles (with alpha)
				//GAE_ASSERTM(m_uPlaneCount >= 3, L"This must never happen!!!");
				if(m_uPlaneCount < 3) return;

				const UINT32 uIdxNearStart = m_uPlaneCount - 2;	// Near and Far are always the latest two planes.

				// Planes
				for(UINT32 i = 0; i < uIdxNearStart; ++i)
				{
					pRenderer->DrawDebugTriangle(&m_v4fVertexes[i], &m_v4fVertexes[uIdxNearStart+((i+1) % (uIdxNearStart))], &m_v4fVertexes[(i+1) % uIdxNearStart]);
					pRenderer->DrawDebugTriangle(&m_v4fVertexes[i], &m_v4fVertexes[uIdxNearStart+i], &m_v4fVertexes[uIdxNearStart+((i+1) % (uIdxNearStart))]);
					pRenderer->DrawDebugLine(&m_v4fVertexes[i], &m_v4fVertexes[uIdxNearStart+i]);
				}

				/*
				// Plane's normals.
				// calculate the vertex's position for the plane.
				Vector4f v4fNormalDebugPosStart[6];
				Vector4f v4fNormalDebugPosEnd[6];
				float fColor = m_v4fVertexes[0].w();

				{
					// Near plane.
					v4fNormalDebugPosStart[PL_NEAR] = (m_v4fVertexes[4] + ((m_v4fVertexes[6] - m_v4fVertexes[4]).normalized() * ((m_v4fVertexes[6] - m_v4fVertexes[4]).norm() * 0.5f)));
					v4fNormalDebugPosEnd[PL_NEAR] = v4fNormalDebugPosStart[PL_NEAR] + m_aPlanes[PL_NEAR].m_v4fNormalDistance * 8.0f;
					v4fNormalDebugPosStart[PL_NEAR].w() = fColor;				
					v4fNormalDebugPosEnd[PL_NEAR].w() = fColor;

					v4fNormalDebugPosStart[PL_FAR] = (m_v4fVertexes[0] + ((m_v4fVertexes[2] - m_v4fVertexes[0]).normalized() * ((m_v4fVertexes[2] - m_v4fVertexes[0]).norm() * 0.5f)));
					v4fNormalDebugPosEnd[PL_FAR] = v4fNormalDebugPosStart[PL_FAR] + m_aPlanes[PL_FAR].m_v4fNormalDistance * 8.0f;
					v4fNormalDebugPosStart[PL_FAR].w() = fColor;				
					v4fNormalDebugPosEnd[PL_FAR].w() = fColor;
				}

				{
					Vector4f v4fHalfFar = m_v4fVertexes[0] + ((m_v4fVertexes[1] - m_v4fVertexes[0]) * 0.5f);
					Vector4f v4fHalfNear = m_v4fVertexes[4] + ((m_v4fVertexes[5] - m_v4fVertexes[4]) * 0.5f);

					v4fNormalDebugPosStart[PL_LEFT] = v4fHalfNear + ((v4fHalfFar - v4fHalfNear) * 0.5f);
					v4fNormalDebugPosEnd[PL_LEFT] = v4fNormalDebugPosStart[PL_LEFT] + m_aPlanes[PL_LEFT].m_v4fNormalDistance * 8.0f;
					v4fNormalDebugPosStart[PL_LEFT].w() = fColor;				
					v4fNormalDebugPosEnd[PL_LEFT].w() = fColor;
				}

				{
					Vector4f v4fHalfFar = m_v4fVertexes[3] + ((m_v4fVertexes[2] - m_v4fVertexes[3]) * 0.5f);
					Vector4f v4fHalfNear = m_v4fVertexes[7] + ((m_v4fVertexes[6] - m_v4fVertexes[7]) * 0.5f);

					v4fNormalDebugPosStart[PL_RIGHT] = v4fHalfNear + ((v4fHalfFar - v4fHalfNear) * 0.5f);
					v4fNormalDebugPosEnd[PL_RIGHT] = v4fNormalDebugPosStart[PL_RIGHT] + m_aPlanes[PL_RIGHT].m_v4fNormalDistance * 8.0f;
					v4fNormalDebugPosStart[PL_RIGHT].w() = fColor;				
					v4fNormalDebugPosEnd[PL_RIGHT].w() = fColor;
				}

				{
					Vector4f v4fHalfFar = m_v4fVertexes[0] + ((m_v4fVertexes[3] - m_v4fVertexes[0]) * 0.5f);
					Vector4f v4fHalfNear = m_v4fVertexes[4] + ((m_v4fVertexes[7] - m_v4fVertexes[4]) * 0.5f);

					v4fNormalDebugPosStart[PL_TOP] = v4fHalfNear + ((v4fHalfFar - v4fHalfNear) * 0.5f);
					v4fNormalDebugPosEnd[PL_TOP] = v4fNormalDebugPosStart[PL_TOP] + m_aPlanes[PL_TOP].m_v4fNormalDistance * 8.0f;
					v4fNormalDebugPosStart[PL_TOP].w() = fColor;				
					v4fNormalDebugPosEnd[PL_TOP].w() = fColor;
				}

				{
					Vector4f v4fHalfFar = m_v4fVertexes[1] + ((m_v4fVertexes[2] - m_v4fVertexes[1]) * 0.5f);
					Vector4f v4fHalfNear = m_v4fVertexes[5] + ((m_v4fVertexes[6] - m_v4fVertexes[5]) * 0.5f);

					v4fNormalDebugPosStart[PL_BOTTOM] = v4fHalfNear + ((v4fHalfFar - v4fHalfNear) * 0.5f);
					v4fNormalDebugPosEnd[PL_BOTTOM] = v4fNormalDebugPosStart[PL_BOTTOM] + m_aPlanes[PL_BOTTOM].m_v4fNormalDistance * 8.0f;
					v4fNormalDebugPosStart[PL_BOTTOM].w() = fColor;				
					v4fNormalDebugPosEnd[PL_BOTTOM].w() = fColor;
				}

				pRenderer->DrawDebugLine(&v4fNormalDebugPosStart[PL_NEAR], &v4fNormalDebugPosEnd[PL_NEAR]);
				pRenderer->DrawDebugLine(&v4fNormalDebugPosStart[PL_FAR], &v4fNormalDebugPosEnd[PL_FAR]);
				pRenderer->DrawDebugLine(&v4fNormalDebugPosStart[PL_LEFT], &v4fNormalDebugPosEnd[PL_LEFT]);
				pRenderer->DrawDebugLine(&v4fNormalDebugPosStart[PL_RIGHT], &v4fNormalDebugPosEnd[PL_RIGHT]);
				pRenderer->DrawDebugLine(&v4fNormalDebugPosStart[PL_TOP], &v4fNormalDebugPosEnd[PL_TOP]);
				pRenderer->DrawDebugLine(&v4fNormalDebugPosStart[PL_BOTTOM], &v4fNormalDebugPosEnd[PL_BOTTOM]);
				*/

			}

#endif
		}
	}
}