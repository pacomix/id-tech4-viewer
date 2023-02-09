#include "stdafx.h"
#include "Graphics/Geometry/Procedural.h"
#include "Graphics/Mesh.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			SINGLETON_IMPL(Procedural);

			bool Procedural::Init()
			{
				static bool bFirstInit = TRUE;

				if(!bFirstInit) return FALSE;

				m_pMeshCube = CreateCube();

				bFirstInit = FALSE;

				return TRUE;
			}

			bool Procedural::Uninit()
			{
				return TRUE;
			}

			MultiMesh* Procedural::CreateCube() const
			{
				using namespace Gae::Graphics;
				using namespace Gae::Graphics::Geometry;

				MultiMesh* pMultiMesh = new MultiMesh();
				Mesh* pMesh = new Mesh();
				VertexBuffer* pVB = new VertexBuffer(VT_COLOR, 8);
				IndexBuffer* pIB = new IndexBuffer(24);

				// Create the vectors of index and vertex data.
				VertexBuffer::VecVertex vecVertex;	vecVertex.reserve(8);
				IndexBuffer::VecIndex vecIndex;		vecIndex.reserve(24);

				
				for(int i = 0; i < 8; ++i)
				{	// Create the vertex.
					vecVertex.push_back(new VertexCol);
				}

				for(int i = 0; i < 24; ++i)
				{	// Create the triangle strip indexes.
					vecIndex.push_back(0);
				}

				{	// Assign cube's vertex data.
					// Front side.
					static_cast<VertexCol*>(vecVertex.at(0))->m_v3fPosition << -0.5f, 1.0f, -0.5f;
					static_cast<VertexCol*>(vecVertex.at(1))->m_v3fPosition << -0.5f, 0.0f, -0.5f;
					static_cast<VertexCol*>(vecVertex.at(2))->m_v3fPosition << +0.5f, 1.0f, -0.5f;
					static_cast<VertexCol*>(vecVertex.at(3))->m_v3fPosition << +0.5f, 0.0f, -0.5f;

					// Back side.
					static_cast<VertexCol*>(vecVertex.at(4))->m_v3fPosition << +0.5f, 1.0f, +0.5f;
					static_cast<VertexCol*>(vecVertex.at(5))->m_v3fPosition << +0.5f, 0.0f, +0.5f;
					static_cast<VertexCol*>(vecVertex.at(6))->m_v3fPosition << -0.5f, 1.0f, +0.5f;
					static_cast<VertexCol*>(vecVertex.at(7))->m_v3fPosition << -0.5f, 0.0f, +0.5f;	

					for(int i = 0; i < 8; ++i)
					{	// Vertex color.
						static_cast<VertexCol*>(vecVertex.at(i))->m_dwColor = 0xFFFFFFFF;
					}
				}

				{	// Assign cube's index data.

					// Four sides.
					vecIndex.at(0) = 0;
					vecIndex.at(1) = 1;
					vecIndex.at(2) = 2;
					vecIndex.at(3) = 3;

					vecIndex.at(4) = 4;
					vecIndex.at(5) = 5;

					vecIndex.at(6) = 6;
					vecIndex.at(7) = 7;

					vecIndex.at(8) = 0;
					vecIndex.at(9) = 1;

					// Top side.
					vecIndex.at(10) = 1;	// Degenerate triangle.
					vecIndex.at(11) = 6;
					vecIndex.at(12) = 6;

					vecIndex.at(13) = 6;
					vecIndex.at(14) = 0;
					vecIndex.at(15) = 4;
					vecIndex.at(16) = 2;

					// Bottom side.
					vecIndex.at(17) = 2;	// Degenerate triangle.
					vecIndex.at(18) = 1;
					vecIndex.at(19) = 1;

					vecIndex.at(20) = 1;
					vecIndex.at(21) = 7;
					vecIndex.at(22) = 3;
					vecIndex.at(23) = 5;
				}

				pVB->SetData(vecVertex);
				pIB->SetData(vecIndex);
				pMesh->SetVertexBuffer(pVB);
				pMesh->SetIndexBuffer(pIB);

				//Destroy the intermediate vertex and index vector.
				while(!vecVertex.empty())
				{
					VertexBuffer::VecVertex::iterator it = vecVertex.begin();
					GAE_DELETESAFE(*it);
					vecVertex.erase(it);
				}
				vecIndex.clear();

				pMultiMesh->AddMesh(pMesh);
				return pMultiMesh;
			}

			MultiMesh* Procedural::GetCube() const
			{
				return m_pMeshCube;
			}
		}
	}
}