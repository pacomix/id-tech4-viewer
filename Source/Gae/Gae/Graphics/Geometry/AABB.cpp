#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			AABB::AABB()
			{
				m_v3fMin = Vector3f(FLT_MAX, FLT_MAX, FLT_MAX);
				m_v3fMax = Vector3f(FLT_MIN, FLT_MIN, FLT_MIN);
				m_BoundingSphere.SetValues(Vector3f::Zero(), 1.0f);
			}

			AABB::~AABB()
			{
			}

#ifdef _DEBUG_DRAW
			void AABB::DrawDebug(const Gae::Renderer::Driver* pRenderer) const
			{
				static Gae::Entities::Node _node;
				_node.SetPosition(GetCenter());
				float fScaleX = (m_v3fMax.x() - m_v3fMin.x());// * 0.5f;
				float fScaleY = (m_v3fMax.y() - m_v3fMin.y());// * 0.5f;
				float fScaleZ = (m_v3fMax.z() - m_v3fMin.z());// * 0.5f;
				_node.SetScale(Vector3f(fScaleX, fScaleY, fScaleZ));
				float fColor = (float)D3DCOLOR_ARGB(0, 0, 0, 255);

				pRenderer->DrawDebug(Gae::Renderer::DirectX9::DS_BOX, &_node, (DWORD)fColor);
				return;
				/*
				Vector4f v4fVertexes[8] = 
				{
					// Near
					Vector4f(m_v3fMin(0), m_v3fMax(1), m_v3fMin(2), fColor),
					Vector4f(m_v3fMin(0), m_v3fMin(1), m_v3fMin(2), fColor),
					Vector4f(m_v3fMax(0), m_v3fMin(1), m_v3fMin(2), fColor),
					Vector4f(m_v3fMax(0), m_v3fMax(1), m_v3fMin(2), fColor),

					// Far
					Vector4f(m_v3fMin(0), m_v3fMax(1), m_v3fMax(2), fColor),
					Vector4f(m_v3fMin(0), m_v3fMin(1), m_v3fMax(2), fColor),
					Vector4f(m_v3fMax(0), m_v3fMin(1), m_v3fMax(2), fColor),
					Vector4f(m_v3fMax(0), m_v3fMax(1), m_v3fMax(2), fColor),
				};
				*/
				Vector4f v4fVertexes[8];

				// Near
				v4fVertexes[0] = Vector4f(m_v3fMin(0), m_v3fMax(1), m_v3fMin(2), fColor);
				v4fVertexes[1] = Vector4f(m_v3fMin(0), m_v3fMin(1), m_v3fMin(2), fColor);
				v4fVertexes[2] = Vector4f(m_v3fMax(0), m_v3fMin(1), m_v3fMin(2), fColor);
				v4fVertexes[3] = Vector4f(m_v3fMax(0), m_v3fMax(1), m_v3fMin(2), fColor);

				// Far
				v4fVertexes[4] = Vector4f(m_v3fMin(0), m_v3fMax(1), m_v3fMax(2), fColor);
				v4fVertexes[5] = Vector4f(m_v3fMin(0), m_v3fMin(1), m_v3fMax(2), fColor);
				v4fVertexes[6] = Vector4f(m_v3fMax(0), m_v3fMin(1), m_v3fMax(2), fColor);
				v4fVertexes[7] = Vector4f(m_v3fMax(0), m_v3fMax(1), m_v3fMax(2), fColor);

				// Triangles (with alpha)
				// Far plane.
				pRenderer->DrawDebugTriangle(&v4fVertexes[3], &v4fVertexes[1], &v4fVertexes[2]);
				pRenderer->DrawDebugTriangle(&v4fVertexes[3], &v4fVertexes[0], &v4fVertexes[1]);

				// Near Plane
				pRenderer->DrawDebugTriangles(&v4fVertexes[4], 1);
				pRenderer->DrawDebugTriangle(&v4fVertexes[4], &v4fVertexes[7], &v4fVertexes[6]);

				// Left plane
				pRenderer->DrawDebugTriangle(&v4fVertexes[0], &v4fVertexes[5], &v4fVertexes[1]);
				pRenderer->DrawDebugTriangle(&v4fVertexes[0], &v4fVertexes[4], &v4fVertexes[5]);

				// Right plane
				pRenderer->DrawDebugTriangle(&v4fVertexes[7], &v4fVertexes[2], &v4fVertexes[6]);
				pRenderer->DrawDebugTriangle(&v4fVertexes[7], &v4fVertexes[3], &v4fVertexes[2]);

				// Top plane
				pRenderer->DrawDebugTriangle(&v4fVertexes[3], &v4fVertexes[4], &v4fVertexes[0]);
				pRenderer->DrawDebugTriangle(&v4fVertexes[3], &v4fVertexes[7], &v4fVertexes[4]);

				// Bottom plane
				pRenderer->DrawDebugTriangle(&v4fVertexes[6], &v4fVertexes[1], &v4fVertexes[5]);
				pRenderer->DrawDebugTriangle(&v4fVertexes[6], &v4fVertexes[2], &v4fVertexes[1]);

				// Lines.
				// Far plane.
				pRenderer->DrawDebugLines(&v4fVertexes[0], 3);
				pRenderer->DrawDebugLine(&v4fVertexes[3], &v4fVertexes[0]);

				// Near plane.
				pRenderer->DrawDebugLines(&v4fVertexes[4], 3);
				pRenderer->DrawDebugLine(&v4fVertexes[7], &v4fVertexes[4]);

				// Left, right, top and bottom planes.
				pRenderer->DrawDebugLine(&v4fVertexes[0], &v4fVertexes[4]);
				pRenderer->DrawDebugLine(&v4fVertexes[1], &v4fVertexes[5]);
				pRenderer->DrawDebugLine(&v4fVertexes[2], &v4fVertexes[6]);
				pRenderer->DrawDebugLine(&v4fVertexes[3], &v4fVertexes[7]);

				/*
				// Extent
				pRenderer->DrawDebugLine(&v4fVertexes[1], &v4fVertexes[7]);
				*/
			}
#endif
		}
	}
}