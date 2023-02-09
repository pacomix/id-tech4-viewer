#ifndef VertexTypes_h__
#define VertexTypes_h__

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			enum E_FormatFlags
			{
				VT_UNDEFINED	=	0x00000000,
				VT_COLOR		=	0x00000001,
				VT_NORMAL		=	0x00000002,
				VT_UV_COORD		=	0x00000004,
				VT_TANGENT		=	0x00000008,
				VT_BINORMAL		=	0x00000010
			};

			struct Vertex
			{
				Vector3f m_v3fPosition;
			};

			struct VertexCol : Vertex
			{
				DWORD m_dwColor;
			};

			struct VertexNorm : Vertex
			{
				Vector3f m_v3fNormal;
			};

			struct VertexUV : Vertex
			{
				Vector2f m_v2fUV;
			};

			struct VertexColNorm : Vertex
			{
				DWORD m_dwColor;
				Vector3f m_v3fNormal;
			};

			struct VertexColUV : Vertex
			{
				DWORD m_dwColor;
				Vector2f m_v2fUV;
			};

			struct VertexNormUV : Vertex
			{
				Vector3f m_v3fNormal;
				Vector2f m_v2fUV;
			};

			struct VertexColNormUV : Vertex
			{
				DWORD m_dwColor;
				Vector3f m_v3fNormal;
				Vector2f m_v2fUV;
			};

			struct VertexColNormUVTB : Vertex
			{
				DWORD m_dwColor;
				Vector3f m_v3fNormal;
				Vector2f m_v2fUV;
				Vector3f m_v3fTangent;
				Vector3f m_v3fBinormal;
			};
		}
	}
}

#endif // VertexTypes_h__