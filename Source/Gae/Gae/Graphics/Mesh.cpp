#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		GAE_CONTAINER_IMPL(Mesh, Gae::Entities::Light, Light);
		//GAE_CONTAINER_IMPL(Mesh, Gae::Core::Universe::Portal, Portal);

		Mesh::Mesh() 
			: super()
			, m_pVertexBuffer(NULL)
			, m_pIndexBuffer(NULL)
			, m_pMaterial(NULL)
		{
		}

		void Mesh::ComputeAABB()
		{
			Vector3f v3fMin(FLT_MAX, FLT_MAX, FLT_MAX);
			Vector3f v3fMax(FLT_MIN, FLT_MIN, FLT_MIN);
			//GAE_ASSERTM(m_AABB.GetMin() == v3fMin || m_AABB.GetMax() == v3fMax, L"It seems the AABB was already calculated. Are you sure this method's call is correct?");

			// Iterate through the vertex buffer
			char* pVertexStart = (char*)(m_pVertexBuffer->GetData());

			size_t stVtxLength = m_pVertexBuffer->GetSizeVertex();
			UINT32 uVtxCount = (UINT32)m_pVertexBuffer->GetSizeData() / stVtxLength;

			v3fMin << *((float*)pVertexStart), *((float*)(pVertexStart + 4)), *((float *)(pVertexStart + 8));
			v3fMax << *((float*)pVertexStart), *((float*)(pVertexStart + 4)), *((float *)(pVertexStart + 8));
			for(UINT32 i = 0; i < uVtxCount; ++i, pVertexStart += stVtxLength)
			{
				Vector3f v3fTemp(*((float*)pVertexStart), *((float*)(pVertexStart + 4)), *((float *)(pVertexStart + 8)));

				if(v3fTemp.x() < v3fMin.x())	v3fMin.x() = v3fTemp.x();
				if(v3fTemp.y() < v3fMin.y())	v3fMin.y() = v3fTemp.y();
				if(v3fTemp.z() < v3fMin.z())	v3fMin.z() = v3fTemp.z();

				if(v3fTemp.x() > v3fMax.x())	v3fMax.x() = v3fTemp.x();
				if(v3fTemp.y() > v3fMax.y())	v3fMax.y() = v3fTemp.y();
				if(v3fTemp.z() > v3fMax.z())	v3fMax.z() = v3fTemp.z();
			}

			m_AABB.SetValues(v3fMin, v3fMax);
		}

		Mesh::~Mesh()
		{
			GAE_DELETESAFE(m_pVertexBuffer);
			GAE_DELETESAFE(m_pIndexBuffer);

			if(m_pMaterial) 
			{
				GAE_DELETESAFE(m_pMaterial);
			}
		}

#ifdef _DEBUG_DRAW
		void Mesh::DrawDebug(const Gae::Renderer::Driver* pRenderer) const
		{
			m_AABB.DrawDebug(pRenderer);

			/*
			for each(const RECT& rect in m_VecRect)
			{
				pRenderer->DrawDebugRect(&rect, D3DCOLOR_XRGB(255, 0, 255));
			}
			*/
			for each(const Portal* pPortal in m_VecPortal)
			{
				pRenderer->DrawDebugRect(&pPortal->GetClippedPortalRect(), D3DCOLOR_XRGB(255, 0, 255));
			}

		}
#endif
	}
}