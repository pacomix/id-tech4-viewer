namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			inline VertexBuffer::VertexBuffer(UINT32 uFormatFlags, UINT32 uVertexCount)
				: m_uFormat(uFormatFlags)
				, m_uVertexCount(uVertexCount)
				, m_pData(NULL)
				, m_pVB(NULL)
				, m_sizeData(0)
				, m_pVertexDeclaration(NULL)
			{
				//GAE_ASSERTM(m_uFormat != VT_UNDEFINED, L"Incorrect format specified!");
				GAE_ASSERTM(!(m_uFormat & ~(VT_COLOR | VT_NORMAL | VT_UV_COORD | VT_TANGENT | VT_BINORMAL)), L"Incorrect format specified!");

				// Compute the vertex size based on the format flags.
				if(m_uFormat == (VT_COLOR | VT_NORMAL | VT_UV_COORD | VT_TANGENT | VT_BINORMAL))
				{
					m_sizeVertex = sizeof(VertexColNormUVTB);
				}
				else if(m_uFormat == (VT_COLOR | VT_NORMAL | VT_UV_COORD))
				{
					m_sizeVertex = sizeof(VertexColNormUV);
				}
				else if(m_uFormat == (VT_COLOR | VT_NORMAL))
				{
					m_sizeVertex = sizeof(VertexColNorm);
				}
				else if(m_uFormat == (VT_NORMAL | VT_UV_COORD))
				{
					m_sizeVertex = sizeof(VertexNormUV);
				}
				else if(m_uFormat == VT_COLOR)
				{
					m_sizeVertex = sizeof(VertexCol);
				}
				else if(m_uFormat == VT_NORMAL)
				{
					m_sizeVertex = sizeof(VertexNorm);
				}
				else if(m_uFormat == VT_UNDEFINED)
				{
					m_sizeVertex = sizeof(Vertex);
				}
				else
				{
					GAE_ASSERTM(false, L"Not supported vertex format!");
				}
				
				m_sizeData = m_sizeVertex * m_uVertexCount;
				
				m_pData = (void**) operator new[](m_sizeData);
				std::memset(m_pData, 0, m_sizeData);

			}

			inline UINT32 VertexBuffer::GetFormat() const
			{
				return m_uFormat;
			}

			inline void VertexBuffer::SetData(VecVertex& vecVertex)
			{
				GAE_ASSERTM(NULL != m_pData, L"Vertex Buffer doesn't exist!!! Impossible!!!");
				GAE_ASSERTM(vecVertex.size() == m_uVertexCount, L"Wrong vertex count!!! Passed vector doesn't correspond with this VertexBuffer!");

				size_t stCurrentOffset = 0;
				for(VecVertex::iterator it = vecVertex.begin(), itEnd = vecVertex.end(); it != itEnd; ++it)
				{
					std::memcpy(((char*)m_pData)+stCurrentOffset, *it, m_sizeVertex);
					stCurrentOffset += m_sizeVertex;
				}
			}

			inline void** VertexBuffer::GetData() const
			{
				return m_pData;
			}

			inline size_t VertexBuffer::GetSizeVertex() const
			{
				return m_sizeVertex;
			}

			inline size_t VertexBuffer::GetSizeData() const
			{
				return m_sizeData;
			}

			inline UINT32 VertexBuffer::GetCount() const
			{
				return (UINT32) (m_sizeData / m_sizeVertex);
			}

			inline bool VertexBuffer::IsInitialized() const
			{
				return NULL != m_pVB;
			}

			inline VertexBuffer::~VertexBuffer()
			{
				GAE_DELETESAFEARRAY(m_pData);
			}
#ifdef _DEBUG
			inline void VertexBuffer::DebugOutput()
			{
				if((m_uFormat & VT_COLOR) && (m_uFormat & VT_NORMAL) && (m_uFormat & VT_UV_COORD))
				{
					GAE_TRACE(L"----- VERTEX START -----\n");
					for(char *i = (char*)m_pData, *iEnd = ((char*)m_pData)+m_sizeData; i != iEnd; ++i)
					{
						VertexColNormUV* pVertexCol = reinterpret_cast<VertexColNormUV*>(i);
						GAE_TRACE(L"Pos::RGB::Normal::UV -> [%f, %f, %f]::[%u, %u, %u]::[%f, %f, %f]::[%f, %f]\n", 
							pVertexCol->m_v3fPosition(0),
							pVertexCol->m_v3fPosition(1),
							pVertexCol->m_v3fPosition(2),
							(pVertexCol->m_dwColor & 0xFF000000) > 24,
							(pVertexCol->m_dwColor & 0x00FF0000) > 16,
							(pVertexCol->m_dwColor & 0x0000FF00) > 8,
							pVertexCol->m_v3fNormal(0),
							pVertexCol->m_v3fNormal(1),
							pVertexCol->m_v3fNormal(2),
							pVertexCol->m_v2fUV(0),
							pVertexCol->m_v2fUV(1)
							);
					}
					GAE_TRACE(L"----- VERTEX END -----\n");
				}
				else if((m_uFormat & VT_COLOR) && (m_uFormat & VT_NORMAL))
				{
					GAE_TRACE(L"----- VERTEX START -----\n");
					for(char *i = (char*)m_pData, *iEnd = ((char*)m_pData)+m_sizeData; i != iEnd; ++i)
					{
						VertexColNorm* pVertexCol = reinterpret_cast<VertexColNorm*>(i);
						GAE_TRACE(L"Pos::RGB::Normal -> [%f, %f, %f]::[%f, %f, %f]::[%f, %f, %f]\n", 
							pVertexCol->m_v3fPosition(0),
							pVertexCol->m_v3fPosition(1),
							pVertexCol->m_v3fPosition(2),
							(pVertexCol->m_dwColor & 0xFF000000) > 24,
							(pVertexCol->m_dwColor & 0x00FF0000) > 16,
							(pVertexCol->m_dwColor & 0x0000FF00) > 8,
							pVertexCol->m_v3fNormal(0),
							pVertexCol->m_v3fNormal(1),
							pVertexCol->m_v3fNormal(2)
							);
					}
					GAE_TRACE(L"----- VERTEX END -----\n");
				}
				else if((m_uFormat & VT_COLOR))
				{
					GAE_TRACE(L"----- VERTEX START -----\n");
					for(char *i = (char*)m_pData, *iEnd = ((char*)m_pData)+m_sizeData; i != iEnd; ++i)
					{
						VertexCol* pVertexCol = reinterpret_cast<VertexCol*>(i);
						GAE_TRACE(L"Pos::RGB -> [%f, %f, %f]::[%f, %f, %f]\n", 
							pVertexCol->m_v3fPosition(0),
							pVertexCol->m_v3fPosition(1),
							pVertexCol->m_v3fPosition(2),
							(pVertexCol->m_dwColor & 0xFF000000) > 24,
							(pVertexCol->m_dwColor & 0x00FF0000) > 16,
							(pVertexCol->m_dwColor & 0x0000FF00) > 8
							);
					}
					GAE_TRACE(L"----- VERTEX END -----\n");
				}
				else
				{
					GAE_TRACE(L"Not implemented yet!");
				}
			}
#endif
		}
	}
}