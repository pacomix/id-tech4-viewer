#include "stdafx.h"
#include "Light.h"

namespace Gae
{
	namespace Entities
	{
		using namespace Gae::Graphics;

		Light::Light() : m_eType(LT_POINT), m_pShadowVolume(NULL), m_bOn(true), m_v3fOrigin(Vector3f::Zero()), m_v3fCenter(Vector3f::Zero()), m_v3fColor(Vector3f::Zero()), m_v3fRadius(Vector3f::Zero())
		{
		}

		Light::~Light()
		{

		}

		
		bool Light::Init()
		{
			m_eType = LT_POINT;
			m_bOn = true;

			SetPosition(m_v3fOrigin + m_v3fCenter);
			SetScale(m_v3fRadius);
			UpdateWorldTransform();

			ResetClippedAABB();

			return true;
		}

		bool Light::Update()
		{
			return false;
		}

		bool Light::Uninit()
		{
			m_bOn = true;
			m_pShadowVolume = NULL;
			return true;
		}

		bool Light::IsMeshAffected(const Mesh* pMesh)
		{
			return Gae::Graphics::Geometry::IntersectionAABBEllipsoid(pMesh->GetAABB(), m_v3fOrigin + m_v3fCenter, m_v3fRadius);
		}

		void Light::ResetClippedAABB()
		{
			m_ScreenRect.top = 0;
			m_ScreenRect.left = 0;
			m_ScreenRect.bottom = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight-1;
			m_ScreenRect.right = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth-1;
		}

		void Light::ComputeClippedAABB(const Entities::Camera* pCam)
		{
			// TODO: This is not always valid. Invalid screen rect are calculated sometimes. Spend more time on this issue.
			GAE_ASSERTM(pCam, L"Invalid camera! NULL!");

			Vector4f v4fAABMin;
			Vector4f v4fAABMax;
			v4fAABMin << m_AABB.GetMin(), 1.0f;
			v4fAABMax << m_AABB.GetMax(), 1.0f;

			// If we are too close (inside) to the light volume the light volume covers the whole screen. So just set the
			// viewport values and return;
			/*
			if((pCam->GetWorldPositionDirect() - GetWorldPositionDirect()).squaredNorm() - m_v3fRadius.squaredNorm() <= 0.0f)
			{
				ResetClippedAABB();
				return;
			}
			*/

			// Transform the light AABB vertexes to screen space values and calculate the maximum and minimum.
			Vector4f v4fVtx[8] = 
			{
				Vector4f(v4fAABMin),											// 0 - Bottom Down Left Vertex
				Vector4f(v4fAABMax),											// 1 - Top Up Right
				Vector4f(v4fAABMin.x(), v4fAABMin.y(), v4fAABMax.z(), 1.0f),	// 2 - Top Down Left
				Vector4f(v4fAABMin.x(), v4fAABMax.y(), v4fAABMin.z(), 1.0f),	// 3 - Bottom Up Left
				Vector4f(v4fAABMax.x(), v4fAABMax.y(), v4fAABMin.z(), 1.0f),	// 4 - Bottom Up Right
				Vector4f(v4fAABMax.x(), v4fAABMin.y(), v4fAABMax.z(), 1.0f),	// 5 - Top Down Right
				Vector4f(v4fAABMax.x(), v4fAABMin.y(), v4fAABMin.z(), 1.0f),	// 6 - Bottom Down Right
				Vector4f(v4fAABMin.x(), v4fAABMax.y(), v4fAABMax.z(), 1.0f),	// 7 - Top Up Left
			};

			//Gae::Core::Application::Singleton().GetRenderer()->ComputeViewport(v4fVtx, 8, m_ScreenRect);

			// \note Toooooooooooooooooooooooooooo slow. By using the light volume stencil mask and activating this the FPS goes down.
			// We have to clip the AABB against the frustum in order to always get valid projected vertexes.
			// Indexes of the AABB faces vertexes (planes) to be clipped and tested. 6 faces * 4 vertexes each one = 24 indexes
			int aiIdx[24] =
			{
				0, 2, 7, 3, // Left face
				6, 5, 1, 4, // Right face
				3, 7, 1, 4, // Up face
				0, 2, 5, 6, // Down face
				2, 5, 1, 7, // Top face
				0, 6, 4, 3 // Bottom face
			};

			DWORD dwScreenWidth = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth;
			DWORD dwScreenHeight = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight;

			// Transform the vertexes
			const Matrix4f& mtx44fViewProj = pCam->GetViewProjectionMatrix();
			LONG fTop = 0;
			LONG fBottom = dwScreenHeight-1;
			LONG fLeft = dwScreenWidth-1;
			LONG fRight = 0;

			for(int i = 0; i < 24; i += 4)
			{
				Vector3f av3fPolygonToClip[4];
				Vector3f av3fPolygonClipped[64];	// Big enough number
				int iClippedVertexes;
				int iPolygonToClipVertexCount = 4;

				for(UINT32 k = 0; k < 4; av3fPolygonToClip[k] = *reinterpret_cast<const Vector3f*>(&v4fVtx[aiIdx[i + k]]), ++k);

				for(UINT32 p = 0; p < pCam->GetFrustum().GetPlaneCount(); p++)
				{
					pCam->GetFrustum().GetPlane(p).ClipPolygon(av3fPolygonToClip, &iPolygonToClipVertexCount, av3fPolygonClipped, & iClippedVertexes);

					for(int j = 0; j < iClippedVertexes; ++j)
					{
						Vector4f v4fVtxClipped(av3fPolygonClipped[j].x(), av3fPolygonClipped[j].y(), av3fPolygonClipped[j].z(), 1.0f);
						v4fVtxClipped = mtx44fViewProj * v4fVtxClipped;
						v4fVtxClipped /= v4fVtxClipped.w();
						v4fVtxClipped *= 0.5f;
						v4fVtxClipped.cwise() += 0.5f;

						LONG lX = (LONG)(v4fVtxClipped.x() * dwScreenWidth);
						LONG lY = (LONG)(v4fVtxClipped.y() * dwScreenHeight);

						if(fTop < lY) fTop = lY;
						if(fBottom > lY) fBottom = lY;
						if(fLeft > lX) fLeft = lX;
						if(fRight < lX) fRight = lX;
					}
				}
			}

			/*
			for(int i = 0; i < 8; ++i)
			{
				v4fVtx[i] = mtx44fViewProj * v4fVtx[i];
				v4fVtx[i] /= v4fVtx[i].w();
				//if(v4fVtx[i].z() < -1.0f || v4fVtx[i].z() > 1.0f) continue;
				v4fVtx[i] *= 0.5f;
				v4fVtx[i].cwise() += 0.5f;

				LONG lX = (LONG)(v4fVtx[i].x() * dwScreenWidth);
				LONG lY = (LONG)(v4fVtx[i].y() * dwScreenHeight);

				if(fTop < lY) fTop = lY;
				if(fBottom > lY) fBottom = lY;
				if(fLeft > lX) fLeft = lX;
				if(fRight < lX) fRight = lX;
			}
			*/

			// Adapt the values in order we do not have outside of screen values.
			if(fTop > (LONG)dwScreenHeight-1) fTop = dwScreenHeight-1;
			if(fBottom < 0L) fBottom = 0L;

			if(fRight > (LONG)dwScreenWidth-1) fRight = dwScreenWidth-1;
			if(fLeft < 0L) fLeft = 0L;

			// Invert the y-clip coordinate in order to match the directX screen coordinates system (y = 0 = top)
			fTop = dwScreenHeight-1 - fTop;
			fBottom = dwScreenHeight-1 - fBottom;

			/*
			if(fLeft > fRight)
			{
				LONG fAux = fLeft;
				fLeft = fRight;
				fRight = fAux;
			}

			if(fTop < fBottom)
			{
				LONG fAux = fTop;
				fTop = fBottom;
				fBottom = fAux;
			}
			*/

			m_ScreenRect.top = fTop;
			m_ScreenRect.left = fLeft;
			m_ScreenRect.right = fRight;
			m_ScreenRect.bottom = fBottom;
			
		}

		void Light::DrawVolume(const Gae::Renderer::Driver* pRenderer) const
		{
			DWORD dwColor = 0;

			/* For some reason when generating this temporary node is not the same as our light.
			TODO - Investigate this further and create a copy constructor.
			NOTE - This was causing the light volumes getting displaced with the camera position. The correct way is to use the 'this' pointer.
			Node node;
			node.SetWorldTransform(GetWorldTransformDirect());
			node.SetPosition(m_v3fOrigin + m_v3fCenter);
			node.SetScale(m_v3fRadius);
			node.UpdateWorldTransform();
			*/

			//pRenderer->DrawDebug(Gae::Renderer::DirectX9::DS_SPHERE, &node, dwColor);

			pRenderer->DrawDebug(Gae::Renderer::DirectX9::DS_SPHERE, this, dwColor);
		}

#ifdef _DEBUG_DRAW
		void Light::DrawDebug(const Gae::Renderer::Driver* pRenderer) const
		{
			DWORD dwColor = D3DCOLOR_XRGB((int)(m_v3fColor.x() * 256.0f), (int)(m_v3fColor.y() * 256.0f), (int)(m_v3fColor.z() * 256.0f));
			
			/* For some reason when generating this temporary node is not the same as our light.
			TODO - Investigate this further and create a copy constructor.
			NOTE - This was causing the light volumes getting displaced with the camera position. The correct way is to use the 'this' pointer.
			Node node;
			node.SetPosition(m_v3fOrigin + m_v3fCenter);
			node.SetScale(m_v3fRadius);
			node.UpdateWorldTransform();
			*/
			
			//pRenderer->DrawDebug(Gae::Renderer::DirectX9::DS_SPHERE, &node, dwColor);

			pRenderer->DrawDebug(Gae::Renderer::DirectX9::DS_SPHERE, this, dwColor);
			pRenderer->DrawDebugRect(&m_ScreenRect, D3DCOLOR_XRGB(255, 255, 0));
			m_AABB.DrawDebug(pRenderer);
		}
#endif
	}
}