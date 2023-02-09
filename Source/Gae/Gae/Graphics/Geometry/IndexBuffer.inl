namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			inline IndexBuffer::IndexBuffer(UINT32 uIndexCount)
				: m_pData(NULL)
				, m_pIB(NULL)
				, m_uIndexCount(uIndexCount)
				, m_sizeIndex(sizeof(UINT32))
				, m_sizeData(sizeof(UINT32) * uIndexCount)
			{
				m_pData = (void**) operator new[](m_sizeData);
				std::memset(m_pData, 0, m_sizeData);

				GAE_ASSERT(m_pData);
			}

			inline void IndexBuffer::SetData(VecIndex& vecData)
			{
				GAE_ASSERTM(m_sizeData == vecData.size() * sizeof(UINT32), L"Incorrect number of indexes in the vector for this index buffer!!!");
				std::memcpy(m_pData, &vecData[0], m_sizeData);
			}

			inline void** IndexBuffer::GetData() const
			{
				return m_pData;
			}

			inline UINT32 IndexBuffer::GetCount() const
			{
				return m_uIndexCount;
			}

			inline bool IndexBuffer::IsInitialized() const
			{
				return NULL != m_pIB;
			}

			/*
			inline void** IndexBuffer::GetData() const
			{
				return m_pData;
			}
			*/

			inline IndexBuffer::~IndexBuffer()
			{
				GAE_DELETESAFEARRAY(m_pData);
			}

#ifdef _DEBUG
			inline void IndexBuffer::DebugOutput()
			{
				GAE_TRACE(L"----- INDEX START -----\n");
				int iCounter = 0;
				for(char *i = (char*)m_pData, *iEnd = ((char*)m_pData)+m_sizeData; i != iEnd; i += sizeof(UINT32), ++iCounter)
				{
					GAE_TRACE(L"Idx[%u] -> [%u]\n", iCounter, *(reinterpret_cast<UINT32*>(i)));
				}
				GAE_TRACE(L"----- VERTEX END -----\n");
			}
#endif
		}
	}
}