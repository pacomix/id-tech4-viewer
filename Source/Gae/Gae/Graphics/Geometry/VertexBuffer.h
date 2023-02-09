#ifndef VertexBuffer_h__
#define VertexBuffer_h__

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			struct Vertex; 

			class VertexBuffer
			{

			public:
				typedef std::vector< Vertex* > VecVertex;

				friend class Gae::Renderer::DirectX9;
				friend class Gae::Renderer::OpenGL;

				VertexBuffer(UINT32 uFormatFlags, UINT32 uVertexCount);
				~VertexBuffer();
				UINT32 GetFormat() const;
				void** GetData() const;
				size_t GetSizeVertex() const;
				size_t GetSizeData() const;
				UINT32 GetCount() const;

				bool IsInitialized() const;

				// TODO: Add a serializer to load data format from the engine.
				void SetData(VecVertex& vecVertex);

#ifdef _DEBUG
				void DebugOutput();
#endif

			protected:
				UINT32 m_uFormat;
				size_t m_sizeVertex;
				size_t m_sizeData;
				UINT32 m_uVertexCount;
				void** m_pData;				

				// DirectX Vertex Buffer.
				// \note Is created inside the DirectX9 renderer class.
				LPDIRECT3DVERTEXBUFFER9 m_pVB;
				IDirect3DVertexDeclaration9* m_pVertexDeclaration;

				// OpenGL Vertex Buffer
				// TODO: Implement the openGL one.

			private:
				void* CreateBuffer();

			};
		}
	}
}

#include "VertexBuffer.inl"

#endif // VertexBuffer_h__