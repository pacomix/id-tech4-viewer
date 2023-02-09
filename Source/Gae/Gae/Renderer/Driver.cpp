#include "stdafx.h"
#include "Driver.h"

namespace Gae
{
	namespace Renderer
	{
		Driver::Driver()
		{
		}

		Driver::~Driver()
		{
		}

		bool Driver::Init()
		{
			m_pRendererCfg = &(Gae::Core::Application::Singleton().GetApplicationConfig()->m_RendererConfig);

			m_mtx44fView = Matrix4f::Identity();
			m_mtx44fProj = Matrix4f::Identity();
			m_mtx44fViewProj = Matrix4f::Identity();
			
			return TRUE;
		}

		void Driver::AddToRender(const VecRenderList& deqRenderList)
		{
			m_DeqRender.insert(m_DeqRender.end(), deqRenderList.begin(), deqRenderList.end());
		}

		void Driver::AddToRender(const Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERT(NULL != pMesh);
			GAE_ASSERTM(std::find(m_DeqRender.begin(), m_DeqRender.end(), pMesh) == m_DeqRender.end(), L"Mesh is already in the render list!!!");

			// \note Basic implementation.
			// TODO: Make more lists sorted by material, shader, etc...
			m_DeqRender.push_back(pMesh);
		}

		bool Driver::InitMesh(Gae::Graphics::Mesh* pMesh)
		{
			GAE_TRACE(L"Initializing Mesh: [%s]\n", pMesh->GetTAG().c_str());

			return TRUE;
		}

		bool Driver::UninitMesh(Gae::Graphics::Mesh* pMesh)
		{
			GAE_TRACE(L"Uninitializing Mesh: [%s]\n", pMesh->GetTAG().c_str());

			return TRUE;
		}

		void Driver::AddToRenderStatic(const Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERT(NULL != pMesh);
			GAE_ASSERTM(std::find(m_DeqRenderStatic.begin(), m_DeqRenderStatic.end(), pMesh) == m_DeqRenderStatic.end(), L"Mesh is already in the render list!!!");

			// Check for transparency.
			if(pMesh->GetMaterial())
			{
				//if(pMesh->GetMaterial()->IsAlphaTest() || pMesh->GetMaterial()->IsBlend() || pMesh->GetMaterial()->IsDecal())
				//if(pMesh->GetMaterial()->FlagEnabled(Material::M_TRANSLUCENT | Material::M_DECAL | Material::M_BLEND))
				//if(!pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
				//if(!pMesh->GetMaterial()->FlagEnabled(Material::M_DECAL | Material::M_BLEND))
				using namespace Gae::Graphics;
				if(pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
				{
					m_DeqRenderStatic.push_back(pMesh);
				}
				else if(pMesh->GetMaterial()->FlagEnabled(Material::M_DECAL))
				{
					m_DeqRenderStaticDecal.push_back(pMesh);
				}
				else if(pMesh->GetMaterial()->FlagEnabled(Material::M_BLEND))
				{
					m_DeqRenderStaticTranslucent.push_back(pMesh);
				}
				else
				{
					m_DeqRenderStatic.push_back(pMesh);
				}
			}
			else
			{
				//m_DeqRenderStaticTranslucent.push_back(pMesh);
			}
		}

		void Driver::AddToRenderStatic(const VecRenderList& deqRenderList)
		{
			for(VecRenderList::const_iterator it = deqRenderList.begin(), itEnd = deqRenderList.end(); it != itEnd; ++it)
			{
				const Gae::Graphics::Mesh* pMesh = *it;

				AddToRenderStatic(pMesh);
			}
		}

		void Driver::AddToRenderShadowStatic(const Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERT(NULL != pMesh);
			GAE_ASSERTM(std::find(m_DeqRenderShadowStatic.begin(), m_DeqRenderShadowStatic.end(), pMesh) == m_DeqRenderShadowStatic.end(), L"Mesh is already in the render list!!!");
	
			m_DeqRenderShadowStatic.push_back(pMesh);
		}

		void Driver::AddToRenderShadowStatic(const VecRenderList& deqRenderList)
		{
			for(VecRenderList::const_iterator it = deqRenderList.begin(), itEnd = deqRenderList.end(); it != itEnd; ++it)
			{
				const Gae::Graphics::Mesh* pMesh = *it;
				m_DeqRenderShadowStatic.push_back(pMesh);
			}
		}

		void Driver::SetRenderStatic(const VecRenderList& deqRenderList)
		{
			ClearRenderLists();
			AddToRenderStatic(deqRenderList);
		}

		void Driver::SetRenderShadowStatic(const VecRenderList& deqRenderList)
		{
			m_DeqRenderShadowStatic.clear();
			AddToRenderShadowStatic(deqRenderList);
		}

		void Driver::SetRenderLight(const VecRenderLight& deqRenderLight)
		{
			m_DeqRenderLight.clear();
			AddToRenderLight(deqRenderLight);
		}

		void Driver::AddToRenderLight(const Gae::Entities::Light* pLight)
		{
			GAE_ASSERT(NULL != pLight);
			GAE_ASSERTM(std::find(m_DeqRenderLight.begin(), m_DeqRenderLight.end(), pLight) == m_DeqRenderLight.end(), L"Light is already in the render lights list!!!");
			
			m_DeqRenderLight.push_back(pLight);
		}

		void Driver::AddToRenderLight(const VecRenderLight& deqRenderLight)
		{
			for(VecRenderLight::const_iterator it = deqRenderLight.begin(), itEnd = deqRenderLight.end(); it != itEnd; ++it)
			{
				const Gae::Entities::Light* pLight = *it;
				m_DeqRenderLight.push_back(pLight);
			}
		}

		void Driver::ComputeViewport(const Vector4f* av4fVertexes, int iVtxCount, RECT& outViewport) const
		{
			DWORD dwScreenWidth = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth;
			DWORD dwScreenHeight = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight;

			LONG fTop = 0;
			LONG fBottom = dwScreenHeight-1;
			LONG fLeft = dwScreenWidth-1;
			LONG fRight = 0;

			for(int i = 0; i < iVtxCount; ++i)
			{
				Vector4f v4fTemp(m_mtx44fViewProj * av4fVertexes[i]);
				v4fTemp /= v4fTemp.w();
				v4fTemp *= 0.5f;
				v4fTemp.cwise() += 0.5f;

				LONG lX = (LONG)(v4fTemp.x() * dwScreenWidth);
				LONG lY = (LONG)(v4fTemp.y() * dwScreenHeight);

				if(fTop < lY) fTop = lY;
				if(fBottom > lY) fBottom = lY;
				if(fLeft > lX) fLeft = lX;
				if(fRight < lX) fRight = lX;
			}

			// Adapt the values in order we do not have out of screen values.
			outViewport.top = std::min(fTop, (LONG)dwScreenHeight-1);
			outViewport.left = std::max(fLeft, 0L);
			outViewport.right = std::min(fRight, (LONG)dwScreenWidth-1);
			outViewport.bottom = std::max(fBottom, 0L);

			/*
			if(outViewport.top < outViewport.bottom)
			{
				LONG lAux = outViewport.top;
				outViewport.top = outViewport.bottom;
				outViewport.bottom = lAux;
			}

			if(outViewport.right < outViewport.left)
			{
				LONG lAux = outViewport.right;
				outViewport.right = outViewport.left;
				outViewport.left = lAux;
			}
			*/
		}

#ifdef _DEBUG_DRAW
		void Driver::DrawDebugTriangles(const Vector4f* pv4fVertexes, UINT32 uCount) const
		{
			GAE_ASSERTM(false, L"Not implemented yet!");
		}

		void Driver::DrawDebugTriangle(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2, const Vector4f* pv4fVertex3) const
		{
			GAE_ASSERTM(false, L"Not implemented yet!");
		}

		void Driver::DrawDebugLines(const Vector4f* pv4fVertexes, UINT32 uCount) const
		{
			GAE_ASSERTM(false, L"Not implemented yet!");
		}

		void Driver::DrawDebugLine(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2) const
		{
			GAE_ASSERTM(false, L"Not implemented yet!");
		}

		void Driver::DrawDebugRect(const RECT *pRect, const DWORD dwColor) const
		{
			GAE_ASSERTM(false, L"Not implemented yet!");
		}
#endif
	}
}