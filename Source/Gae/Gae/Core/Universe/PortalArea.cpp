#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			GAE_CONTAINER_IMPL( PortalArea, Gae::Entities::Light, Light );

			//int PortalArea::GetVisibleMeshes(const Entities::Camera& camera, VecMesh& vecMesh) /*const*/
			//int PortalArea::GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh) const
			//int PortalArea::GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh, const Portal* pPortal) const
			//int PortalArea::GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh, const RECT& rect) const
			int PortalArea::GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh, const Portal* pPortal) const
			{
				//if(m_bVisited) return 0;
				if(m_bVisited)
				{
					GAE_TRACE(L"Revisiting portal :)\n");
				}

				int iTriangles = 0;
				UINT64 u64CurrentTicks = Gae::Core::Clock::GetCurrentTicks();

				// Add the visible meshes of this area.
				for each(const Mesh* pMesh in m_VecMesh)
				{
					// \note To add a mirror effect for a portal, a viewer position when the mesh was drawn must also be added to the mesh.
					//if(pMesh->GetDrawnTick() == u64CurrentTicks /*&& pMesh->WasDrawnFrom(v3fViewerPos)*/) continue;

					// \TODO Maybe this could be optimized by comparing the portal's screen rect and the mesh's screen rect.
					if(pFrustum->IsInside(pMesh->GetAABB()))
					{
						iTriangles += pMesh->GetIndexBuffer()->GetCount() / 3;
						if(pMesh->GetDrawnTick() != u64CurrentTicks)
						{	// Clear the rects and the portals this mesh is seen through if
							// we are populating the meshes of this area for the first time
							// in the loop.
							const_cast<Mesh*>(pMesh)->ClearVecRect();
							const_cast<Mesh*>(pMesh)->ClearVecPortal();
							vecMesh.push_back(pMesh);
						}

						// It is visible so we mark the time when the mesh was added to the draw list in order avoiding adding it more than one time.
						(const_cast<Mesh*>(pMesh))->SetDrawnTick(u64CurrentTicks);
						//(const_cast<Mesh*>(pMesh))->AddPortal(const_cast<Core::Universe::Portal*>(pPortal));
						// TODO: Priority 1. The problem with this is that if the mesh can be seen through more than one portal
						//				only the first portal clipped AABB rect is used for the scissor test (m_bVisited bool), so... 
						//				if a light affects the mesh and can also be seen through more than one portal the mesh
						//				is not lighted correctly 'cos we are scissoring the mesh against the clipped rect of the
						//				first portal.
						//			Solution 1. Already implemented but not working for some reason :D. Add the portal's screen rect
						//					from where the mesh can be seen to the mesh's rect vector. Yes!! It works! the problem is
						//					when two or more of the screen rect of the portals overlaps each other. Then the overlapped
						//					zone is lighted two times. Try to also compose a mask by using the portal's geometry and light
						//					volume.
						//			Solution 2. Use the AABB screen rect of the mesh instead the screen rect of the portal.
						if(NULL == pPortal)
						{
							static const RECT _rect = {0, 0, ApplicationConfig::m_RendererConfig.m_dwWidth, ApplicationConfig::m_RendererConfig.m_dwHeight};
							(const_cast<Mesh*>(pMesh))->AddRect(_rect);
						}
						else
						{
							(const_cast<Mesh*>(pMesh))->AddRect(pPortal->GetClippedPortalRect());
						}
						
						(const_cast<Mesh*>(pMesh))->AddPortal(pPortal);
						// pMesh->AddViewer(v3fViewerPos);
						continue;
					}
				}

				if(m_bVisited) return 0;
				m_bVisited = true;
				
				// Traverse the visible areas
				for each(const Portal* pPortal in m_VecPortal)
				{	// For each portal in this area clip the frustum against the portal mesh...
					const Geometry::Frustum* pFrustumClipped = pPortal->GetClippedFrustum(pFrustum, v3fViewerPos, pPortal->GetPositiveSideArea() == this);
						
					// ...and if we had a valid frustum...
					if(pFrustumClipped)
					{	// ...use this new clipped frustum to populate the meshes behind the portal.
						//iTriangles += (pPortal->GetPositiveSideArea() == this ? pPortal->GetNegativeSideArea() : pPortal->GetPositiveSideArea())->GetVisibleMeshes(pFrustumClipped, v3fViewerPos, vecMesh);
						//iTriangles += (pPortal->GetPositiveSideArea() == this ? pPortal->GetNegativeSideArea() : pPortal->GetPositiveSideArea())->GetVisibleMeshes(pFrustumClipped, v3fViewerPos, vecMesh, pPortal);
						//iTriangles += (pPortal->GetPositiveSideArea() == this ? pPortal->GetNegativeSideArea() : pPortal->GetPositiveSideArea())->GetVisibleMeshes(pFrustumClipped, v3fViewerPos, vecMesh, pPortal->GetClippedPortalRect());
						iTriangles += (pPortal->GetPositiveSideArea() == this ? pPortal->GetNegativeSideArea() : pPortal->GetPositiveSideArea())->GetVisibleMeshes(pFrustumClipped, v3fViewerPos, vecMesh, pPortal);
					}
				}

				m_bVisited = false;

				return iTriangles;
			}

#ifdef _DEBUG_DRAW
			void PortalArea::DrawDebug(const Gae::Renderer::Driver* pRenderer) const
			{
				for(VecPortal::const_iterator it = m_VecPortal.begin(), itEnd = m_VecPortal.end(); it != itEnd; ++it)
				{
					const Portal* pPortal = *it;
					Gae::Graphics::Mesh* pMesh = pPortal->GetMesh();
					const Vector4f* pv4f = (const Vector4f*)pMesh->GetVertexBuffer()->GetData();

					for(UINT32 i = 0; i < pMesh->GetVertexBuffer()->GetCount(); ++i)
					{
						pRenderer->DrawDebugLine(pv4f + i, (pv4f + ((i + 1) % pMesh->GetVertexBuffer()->GetCount())));
					}

					pRenderer->DrawDebugRect(&pPortal->GetClippedPortalRect(), D3DCOLOR_XRGB(255, 255, 255));
					
				}
			}
		#endif
		}
	}
}