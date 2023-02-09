#ifndef IndexBuffer_h__
#define IndexBuffer_h__

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			class IndexBuffer
			{
			public:
				typedef std::vector< UINT32 > VecIndex;

				friend class Gae::Renderer::DirectX9;
				friend class Gae::Renderer::OpenGL;

				IndexBuffer(UINT32 uIndexCount);
				~IndexBuffer();

				void** GetData() const;
				UINT32 GetCount() const;

				bool IsInitialized() const;

				// TODO: This SetData method should not be here.
				// The vector should be created inside this class
				// by using a serializer object instead setting
				// it from outside.
				void SetData(VecIndex& pData);

#ifdef _DEBUG
				void DebugOutput();
#endif

			protected:
				void** m_pData;
				size_t m_sizeData;
				size_t m_sizeIndex;
				UINT32 m_uIndexCount;

				// DirectX9 Index Buffer object
				LPDIRECT3DINDEXBUFFER9 m_pIB;

				// OpenGL Index Buffer
				// TODO: Implement the openGL one.
			};
		}
	}
}

#include "IndexBuffer.inl"

#endif // IndexBuffer_h__