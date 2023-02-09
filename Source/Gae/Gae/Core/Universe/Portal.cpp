#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			Portal::Portal() : m_iPositiveSideArea(-1), m_iNegativeSideArea(-1), m_pMesh(NULL)
			{
				m_pPlane = new Gae::Graphics::Geometry::Plane;
				m_pFrustum = new Gae::Graphics::Geometry::Frustum;
			}

			Portal::~Portal()
			{
				GAE_DELETESAFE(m_pFrustum);
				GAE_DELETESAFE(m_pPlane);
			}

			void Portal::SetMesh(Gae::Graphics::Mesh* pMesh)
			{
				GAE_ASSERTM(NULL == m_pMesh, L"Portal has an already defined Mesh!!");
				GAE_ASSERTM(NULL != pMesh, L"Invalid Mesh!");
				m_pMesh = pMesh;

				size_t stVtx = m_pMesh->GetVertexBuffer()->GetSizeVertex();
				char* pData = reinterpret_cast<char*>(m_pMesh->GetVertexBuffer()->GetData());
				const Vector3f& pVtx1 = *reinterpret_cast<Vector3f*>(pData + (stVtx * 0));
				const Vector3f& pVtx2 = *reinterpret_cast<Vector3f*>(pData + (stVtx * 1));
				const Vector3f& pVtx3 = *reinterpret_cast<Vector3f*>(pData + (stVtx * 2));

				m_pPlane->SetFromPoints(pVtx1, pVtx2, pVtx3);
			}

			const Gae::Graphics::Geometry::Frustum* Portal::GetClippedFrustum(const Gae::Graphics::Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, bool bPositive) const
			{
				using namespace Gae::Graphics::Geometry;

				if(m_pFrustum == pFrustum)
				{
					return NULL;	// We are trying to clip the same portal's frustum!!! return without reseting the frustum!!!
									// \note If we are using this method by using the clipped frustum of a portal it is possible 
									// that a programmer tries to clip the frustum against the same portal when traversing the portal list,
									// so just return no Frustum in case this happens.
				}

				m_pFrustum->ReservePlanes(0);	// Reset the new Frustum's planes.

				if(!pFrustum->IsInside(m_pMesh->GetAABB()))
				{	// If the portal is not visible skip it. General AABB test.
					return NULL;
				}

				if(bPositive)
				{
					if(m_pPlane->GetDistanceToPlane(v3fViewerPos) < 0.0f)
					{	// If we are in the wrong side of the portal just skip it. It sometimes happens we can face the portal from behind if the area is not a convex one.
						return NULL;
					}
				}
				else
				{
					if(m_pPlane->GetDistanceToPlane(v3fViewerPos) >= 0.0f)
					{	// If we are in the wrong side of the portal just skip it. It sometimes happens we can face the portal from behind if the area is not a convex one.
						return NULL;
					}
				}
				
			#define MAX_CLIPPED_VTX 64
				static Vector3f av3fClippedVtx[MAX_CLIPPED_VTX];
				static Vector3f av3fPlanesVtx[MAX_CLIPPED_VTX];
				int iIdxClippedVtx = m_pMesh->GetVertexBuffer()->GetCount();

				UINT32 uPortalVtxCount = m_pMesh->GetVertexBuffer()->GetCount();
				const Vector4f* pv4f = reinterpret_cast<const Vector4f*>(m_pMesh->GetVertexBuffer()->GetData()); // Vertex + colour
				
				GAE_ASSERTM(MAX_CLIPPED_VTX > uPortalVtxCount * 2, L"Panic! The new Frustum could have more vertexes than actually allowed!");	// Typically a common portal mesh will generate one additional vertex as much.
				if(bPositive)
				{	// TODO: The frustum vertexes can be optimized for RELEASE build by storing it in the mesh without color, thus,
					//	by only having the position information. It can be copied then to the array by using a memcpy function.
					for(UINT32 i = 0; i < uPortalVtxCount; av3fClippedVtx[i] = *reinterpret_cast<const Vector3f*>(pv4f + i), ++i);
				}
				else
				{
					for(UINT32 i = uPortalVtxCount, uArrayIdx = 0; i; av3fClippedVtx[uArrayIdx++] = *reinterpret_cast<const Vector3f*>(pv4f + --i));
				}

				for(UINT32 i = 0; i < pFrustum->GetPlaneCount(); ++i)
				{	// Clip the portal polygon against frustum's planes.
					pFrustum->GetPlane(i).ClipPolygon(av3fClippedVtx, &iIdxClippedVtx, av3fClippedVtx, &iIdxClippedVtx);
				}

				// We need at least a polygon (3 vertexes as minimum!) in order we can get a valid frustum.
				if(iIdxClippedVtx < 3)
				{
					ZeroMemory((void*)&m_Viewport, sizeof(RECT));	// Reset the viewport to 0.
					return NULL;
				}

				// We have a valid frustum. Now calculate the screen viewport of the just clipped portal.
				Vector4f av4fClipRectPortal[64];
				for(int i = 0; i < iIdxClippedVtx; (av4fClipRectPortal[i] = Vector4f(av3fClippedVtx[i].x(), av3fClippedVtx[i].y(), av3fClippedVtx[i].z(), 1.0f)), ++i);
				Gae::Core::Application::Singleton().GetRenderer()->ComputeViewport(av4fClipRectPortal, iIdxClippedVtx, m_Viewport);
				

#ifdef _DEBUG
				if(::GetAsyncKeyState('L'))
				{
					m_pFrustum->ReservePlanes(4+2);	// +2 = For visual debug near and far planes needs to be added in order to calculate the frustum's vertexes.
				}
				else
				{
					m_pFrustum->ReservePlanes(iIdxClippedVtx + 2);	
				}
#else
				if(!::GetAsyncKeyState('L'))
				{
					m_pFrustum->ReservePlanes(4);	// In Release (with no frustum's visual debug) we only really need the other planes.
				}
				else
				{
					m_pFrustum->ReservePlanes(iIdxClippedVtx); // In Release (with no frustum's visual debug) we only really need the other planes.
				}
#endif
				if(!::GetAsyncKeyState('L'))
				{
					Gae::Renderer::Driver* pRenderer = Gae::Core::Application::Singleton().GetRenderer();
					Vector2i av3fViewport[4] = 
					{
						Vector2i(m_Viewport.left, m_Viewport.bottom),
						Vector2i(m_Viewport.right, m_Viewport.bottom),
						Vector2i(m_Viewport.right, m_Viewport.top),
						Vector2i(m_Viewport.left, m_Viewport.top),
					};
					const Matrix4f& m44fProjection = pRenderer->GetProjectionMatrix();
					Matrix4f m44fViewInv = pRenderer->GetViewMatrix().inverse().transpose();	// Current view matrix.
					Vector3f rayOrigin(m44fViewInv.transpose().block(0,3,3,1));	// Take the source point.

					DWORD dwScreenWidth = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth;
					DWORD dwScreenHeight = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight;

					for(int i = 0; i < 4; ++i)
					{
						// The next line obtains the coordinates in the range -1.f +1.f
						Vector3f v3fClip1((((2.0f * av3fViewport[i].x()) / dwScreenWidth) - 1) / m44fProjection(0,0), -(((2.0f * av3fViewport[i].y()) / dwScreenHeight) - 1) / m44fProjection(1,1), 1.0f);
						Vector3f v3fClip2((((2.0f * av3fViewport[(i+1)%4].x()) / dwScreenWidth) - 1) / m44fProjection(0,0), -(((2.0f * av3fViewport[(i+1)%4].y()) / dwScreenHeight) - 1) / m44fProjection(1,1), 1.0f);

						Vector3f rayDir1((m44fViewInv.block(0,0,3,3).transpose() * v3fClip1).normalized());	//Transform the clip coordinates.
						Vector3f rayDir2((m44fViewInv.block(0,0,3,3).transpose() * v3fClip2).normalized());	//Transform the clip coordinates.
						rayDir1 = v3fViewerPos + rayDir1;
						rayDir2 = v3fViewerPos + rayDir2;

						(const_cast<Plane&>(m_pFrustum->GetPlane(i))).SetFromPoints(rayDir1, rayDir2, v3fViewerPos);

					}
				}
				else
				{
					for(int i = 0; i < iIdxClippedVtx; ++i)
					{	// Add the planes to the new frustum.
						Vector3f& v3fStart = av3fClippedVtx[i];
						Vector3f& v3fEnd = av3fClippedVtx[(i+1) % iIdxClippedVtx];

						// Build a plane by using the clipped vertexes.
						(const_cast<Plane&>(m_pFrustum->GetPlane(i))).SetFromPoints(v3fEnd, v3fStart, v3fViewerPos);
					}
				}

#ifdef _DEBUG
				{	// Add the Far and Near plane in order to calculate later the frustum's debug vertexes.
					const Gae::Graphics::Geometry::Frustum& camFrustum = Gae::Core::Universe::Manager::Singleton().GetActiveCamera().GetFrustum();
					m_pFrustum->SetPlane(iIdxClippedVtx+0, camFrustum.GetPlane(Frustum::PL_NEAR).m_v4fNormalDistance);
					m_pFrustum->SetPlane(iIdxClippedVtx+1, camFrustum.GetPlane(Frustum::PL_FAR).m_v4fNormalDistance);
				}

				// Calculate the vertexes for visual debugging of the new frustum
				int uVertexFar = 0;
				int uVertexNear = iIdxClippedVtx;	// Remember, iIdxClippedVtx contains the plane count.
				Vector4f* v4fFrustumVtx = new Vector4f[iIdxClippedVtx * 2];
				UINT32 uIdxVtx = 0;
				DWORD fColor = D3DCOLOR_ARGB(64, 255, 255, 0);
				for(int i = 0; i < iIdxClippedVtx; ++i)
				{	// Far plane against every plane
					Vector3f v3fVtx;
					if(!m_pFrustum->GetIntersection(iIdxClippedVtx + 1, i, (i+1)%(iIdxClippedVtx), &v3fVtx))
					{
						GAE_WARNINGM(false, L"There is no intersection! FAIL!!");
					}
					GAE_ASSERTM(i < uVertexNear, L"Wrong index!");
					v4fFrustumVtx[i] << v3fVtx, *reinterpret_cast<float*>(&fColor);

					// Near plane against every plane
					if(!m_pFrustum->GetIntersection(iIdxClippedVtx, i, (i+1) % iIdxClippedVtx, &v3fVtx))
					{
						GAE_WARNINGM(false, L"There is no intersection! FAIL!!");
					}
					GAE_ASSERTM(i+uVertexNear < uVertexNear*2, L"Wrong index!");
					v4fFrustumVtx[i + uVertexNear] << v3fVtx, *reinterpret_cast<float*>(&fColor);
				}
				m_pFrustum->SetVertexes(v4fFrustumVtx, true);

				GAE_DELETESAFEARRAY(v4fFrustumVtx);
#endif

				return m_pFrustum;
			#undef MAX_CLIPPED_VTX
			}
		}
	}
}