#ifndef DirectX9_h__
#define DirectX9_h__

namespace Gae
{
	namespace Graphics
	{
		class Mesh;
		class Material;		
	}

	namespace Entities
	{
		class Light;
	}

	namespace Renderer
	{
		class DirectX9 : public Driver
		{
			// TODO Create base class for adding further specialization for DX9, DX10 & DX11

			struct ftorRenderMesh;

			friend class Application;
			friend class Universe;

			typedef Driver super;

			SINGLETON_DECL(DirectX9);

		public:
			void ComputeViewport(const Vector4f* av4fVertexes, int iVtxCount, RECT& outViewport) const;

		protected:
			enum E_SHADER_TYPE
			{
				ST_VERTEX,
				ST_PIXEL,
				ST_GEOMETRY
			};

			enum E_SHADER_LIST
			{
				SL_NONE = 0,
				SL_PARALLEL_LIGHT,
				SL_POINT_LIGHT,
				SL_POINT_BUMP,
				SL_POINT_DIFFUSE,
				SL_POINT_PARALLAX,
				SL_AMBIENT,
				SL_DIFFUSE,
				SL_COUNT
			};

			struct VtxShader
			{
				VtxShader() : pShader(NULL), pConstants(NULL) {}
				~VtxShader() {};

				IDirect3DVertexShader9* pShader;
				ID3DXConstantTable* pConstants;
			};

			struct PxlShader
			{
				PxlShader() : pShader(NULL), pConstants(NULL) {}
				~PxlShader() {};

				IDirect3DPixelShader9* pShader;
				ID3DXConstantTable* pConstants;
			};

			typedef std::vector< VtxShader > VecVtxShaders;
			typedef std::vector< PxlShader > VecPxlShaders;

			void AddShaderFromFile(const std::wstring& fFile, E_SHADER_TYPE eType);

			void SetCurrentLight(Gae::Entities::Light* pLight);

			bool InitMesh(Gae::Graphics::Mesh* pMesh);
			//bool InitTexture(Gae::Core::Resources::Texture* pTex, bool bUnique = false);
			bool InitTexture(Gae::Core::Resources::Texture* pTex, bool bMakeAlpha = false);
			void GenerateAndEmbeedHeightmap(Gae::Graphics::Material* pMat);
			bool ContainsHeightmap(Gae::Graphics::Material* pMat);
			bool UninitMesh(Gae::Graphics::Mesh* pMesh);
			void Render();
			void RenderGeometry();	//!< Renders the current render list. RenderStates, matrices, and pDev->Begin() must be configured prior to the call to this method. 


			void RenderWithShadows();
			void RenderWithShadowsFillZBuffer();
			void RenderWithShadowsCreateStencilBuffer();
			void Render(const Gae::Graphics::Mesh* pMesh, bool bFirstPass = true);
			void RenderOld(const Gae::Graphics::Mesh* pMesh, bool bFirstPass = true);
			void RenderFirstPass(const Gae::Graphics::Mesh* pMesh);
			void RenderBlendStages(const Gae::Graphics::Mesh* pMesh);
			void RenderBlendStagesOld(const Gae::Graphics::Mesh* pMesh);
			void Render(const Driver::VecRenderList& deqRenderList);

			/*	\brief	Draws a single triangle.
					It draws a single triangle of the specified color without culling.
			*/
			GAE_DEBUG_FN_DECL(void DrawDebug(const E_DEBUG_SHAPE eShape, const Gae::Graphics::Node* pNode, const DWORD dwColor) const);
			GAE_DEBUG_FN_DECL(void DrawDebugTriangles(const Vector4f* pv4fVertexes, UINT32 uCount) const);
			GAE_DEBUG_FN_DECL(void DrawDebugTriangle(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2, const Vector4f* pv4fVertex3) const);
			GAE_DEBUG_FN_DECL(void DrawDebugLines(const Vector4f* pv4fVertexes, UINT32 uCount) const);
			GAE_DEBUG_FN_DECL(void DrawDebugLine(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2) const);
			GAE_DEBUG_FN_DECL(void DrawDebugRect(const RECT* pRect, const DWORD dwColor) const);
			

		protected:

			/* DX Interface and device. */
			LPDIRECT3D9 m_pD3d9;
			LPDIRECT3DDEVICE9 m_pD3dDev;

			VecVtxShaders m_VecVtxShader;	//!< Vector of vertex shaders.
			VecPxlShaders m_VecPxlShader;	//!< Vector of vertex shaders.

#ifdef _DEBUG
			UINT32 m_uMeshesInDevice;
#endif
			// Input callbacks
			void KeyPressed(UINT uKeyCode);		//!< DIK_xxxxx

		private:

			void SetTwoSided(const Gae::Graphics::Material* pMat);
			void SetAlphaTest(const Gae::Graphics::Material* pMat);

			void RenderLight(const Gae::Entities::Light* pLight);
			void RenderWithLights();
			void RenderOnlyDiffuse();
			void RenderOnlyDiffuse(const Driver::VecRenderList& deqRenderList);
			void RenderOnlyDiffuse(const Gae::Graphics::Mesh* pMesh, bool bFirstPass = true);
			void SelectShader(const E_SHADER_LIST eShader) const;
			PxlShader* GetShaderPixel(const E_SHADER_LIST eShader) const;
			VtxShader* GetShaderVertex(const E_SHADER_LIST eShader) const;

			E_SHADER_LIST m_eLighting;

			// temp
			Gae::Entities::Light* m_pLight;

			//
			bool m_bRenderWithShadows;
			bool m_bRenderWithLights;
			bool m_bWireframe;

			struct ShadowVertex
			{
				D3DXVECTOR4 p;
				D3DCOLOR    color;

				enum FVF
				{
					FVF_Flags = D3DFVF_XYZRHW | D3DFVF_DIFFUSE
				};
			};
			// temp for debugging shadow implementations
			LPDIRECT3DVERTEXBUFFER9  m_pBigSquareVB;

			// temp for texture resources. Used to avoid duplicated texture.
			typedef std::map< UINT32, std::vector< Gae::Core::Resources::Texture* > > MapTextures;
			MapTextures m_MapTextures;

			enum E_DUMMY_TEXT
			{
				DT_SPECULAR,
				DT_NORMAL
			};

			IDirect3DTexture9* GetDummyTexture(E_DUMMY_TEXT eTexture) const;
			DWORD m_TexMinFilter;
			DWORD m_TexMagFilter;
			DWORD m_TexMipFilter;
			DWORD m_TexAddressing;
			DWORD m_TexMaxAnisotropy;
			DWORD m_TexMipLevel;
		};
	}
}

#endif // DirectX9_h__