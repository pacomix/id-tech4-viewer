#include "stdafx.h"
#include "DirectX9.h"

namespace Gae
{
	namespace Renderer
	{

		SINGLETON_IMPL(DirectX9);

		bool DirectX9::Init()
		{
			if(!super::Init()) return FALSE;

			m_pLight = NULL;
			m_eLighting = SL_POINT_BUMP;
			m_bRenderWithShadows = false;
			m_bRenderWithLights = true;
			m_bWireframe = false;

			// best filtering results I have had
			m_TexMinFilter = D3DTEXF_ANISOTROPIC;
			m_TexMagFilter = D3DTEXF_LINEAR;
			m_TexMipFilter = D3DTEXF_LINEAR;
			m_TexAddressing = D3DTADDRESS_WRAP;
			m_TexMaxAnisotropy = 16;
			m_TexMipLevel = 0;

			/* DirectX Object. */
			if((m_pD3d9=Direct3DCreate9(D3D_SDK_VERSION)) == NULL){
				MessageBox(m_pRendererCfg->m_hWnd, L"You need at least DirectX 9.0c to run the application. ", L"INCORRECT DirectX VERSION", MB_OK);
				return FALSE;
			}

			if (!D3DXCheckVersion(D3D_SDK_VERSION, D3DX_SDK_VERSION)){
				MessageBox(m_pRendererCfg->m_hWnd, L"You need correct DirectX version update to run the application. ", L"INCORRECT D3DX VERSION", MB_OK);
				return FALSE;
			}

			// Gets the current window width/height.
			RECT WndRect;
			GetWindowRect(m_pRendererCfg->m_hWnd, &WndRect);
			
			/* Presentation parameters. */
			D3DPRESENT_PARAMETERS D3dPp;
			memset(&D3dPp, 0, sizeof(D3dPp));
			D3dPp.BackBufferCount=1;
			D3dPp.BackBufferFormat=D3DFMT_A8R8G8B8;
			D3dPp.BackBufferHeight=WndRect.bottom - WndRect.top;
			D3dPp.BackBufferWidth=WndRect.right - WndRect.left;
			D3dPp.hDeviceWindow=m_pRendererCfg->m_hWnd;
			D3dPp.SwapEffect=/*D3DSWAPEFFECT_COPY*/D3DSWAPEFFECT_DISCARD;
			D3dPp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
			D3dPp.EnableAutoDepthStencil = TRUE;
			D3dPp.AutoDepthStencilFormat = D3DFMT_D24S8;
			D3dPp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;

			D3dPp.Windowed = m_pRendererCfg->m_uFlags & RendererConfig::RCF_WINDOWED;
			D3dPp.FullScreen_RefreshRateInHz=0;

			if(FAILED(this->m_pD3d9->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_HAL, m_pRendererCfg->m_hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &D3dPp, &m_pD3dDev)))
			{
				MessageBox(m_pRendererCfg->m_hWnd, L"You need at least a 100% compatible DirectX 9.0c graphics card. ", L"NOT SUPPORTED DEVICE", MB_OK);
				return FALSE;
			}
			else
			{
 				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
 				m_pD3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);	// No lights. They will be defined in the shaders.
				m_pD3dDev->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
				m_pD3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);

				m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
				m_pD3dDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255,255,255));
				m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
				//m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				//m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				//m_pD3dDev->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);

				m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

				// Define default light
				D3DLIGHT9 lightDefault;
				memset(&lightDefault, 0, sizeof(D3DLIGHT9));
				lightDefault.Diffuse.r = 1.0f;
				lightDefault.Diffuse.g = 1.0f;
				lightDefault.Diffuse.b = 1.0f;
				lightDefault.Position = *(reinterpret_cast<const D3DVECTOR*>(Vector3f(0.0f, 100.0f, 0.0f).data()));
				lightDefault.Attenuation0 = 0.01f;
				//lightDefault.Direction = *(reinterpret_cast<const D3DVECTOR*>(Vector3f(1.0f, -1.0f, 1.0f).normalized().data()));
				/*
				Vector3f lightDir; lightDir(0) = 1.0f; lightDir(1) = -1.0f; lightDir(2) = 1.0f; lightDir.normalize();
				lightDefault.Direction.x = lightDir(0);
				lightDefault.Direction.y = lightDir(1);
				lightDefault.Direction.z = lightDir(2);
				*/

				lightDefault.Type = D3DLIGHT_POINT;
				lightDefault.Range = 2000.0f;
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetLight(0, &lightDefault));
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->LightEnable(0, FALSE));
 
				// Create the shaders.
				AddShaderFromFile(L"shaders/ParallelLighting", ST_VERTEX);
				AddShaderFromFile(L"shaders/PointLighting", ST_VERTEX);
				AddShaderFromFile(L"shaders/PointBumpLighting", ST_VERTEX);
				AddShaderFromFile(L"shaders/PointDiffuseLighting", ST_VERTEX);
				AddShaderFromFile(L"shaders/PointParallaxLighting", ST_VERTEX);
				AddShaderFromFile(L"shaders/Ambient", ST_VERTEX);
				AddShaderFromFile(L"shaders/Diffuse", ST_VERTEX);
				AddShaderFromFile(L"shaders/ParallelLighting", ST_PIXEL);
				AddShaderFromFile(L"shaders/PointLighting", ST_PIXEL);
				AddShaderFromFile(L"shaders/PointBumpLighting", ST_PIXEL);
				AddShaderFromFile(L"shaders/PointDiffuseLighting", ST_PIXEL);
				AddShaderFromFile(L"shaders/PointParallaxLighting", ST_PIXEL);
				AddShaderFromFile(L"shaders/Ambient", ST_PIXEL);
				AddShaderFromFile(L"shaders/Diffuse", ST_PIXEL);

				// Set the default shaders
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexShader(m_VecVtxShader.at(2).pShader));
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetPixelShader(m_VecPxlShader.at(2).pShader));



				// TEST. Big square
				{
					m_pD3dDev->CreateVertexBuffer( 4*sizeof(ShadowVertex),
						D3DUSAGE_WRITEONLY, ShadowVertex::FVF_Flags,
						D3DPOOL_MANAGED, &m_pBigSquareVB, NULL );

					// Get the width & height of the back-buffer.
					LPDIRECT3DSURFACE9 pBackBuffer = NULL;
					D3DSURFACE_DESC d3dsd;
					m_pD3dDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
					pBackBuffer->GetDesc( &d3dsd );
					pBackBuffer->Release();
					float sx = (float)d3dsd.Width;
					float sy = (float)d3dsd.Height;

					// Set the size of the big square shadow
					ShadowVertex *v;

					m_pBigSquareVB->Lock( 0, 0, (void**)&v, 0 );
					{
						v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
						v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
						v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
						v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
						v[0].color = 0xcf000000;
						v[1].color = 0xcf000000;
						v[2].color = 0xcf000000;
						v[3].color = 0xcf000000;
					}
					m_pBigSquareVB->Unlock();
				}

				SelectShader(m_eLighting);
			}

#ifdef _DEBUG
			m_uMeshesInDevice = 0;
#endif // _DEBUG

#ifdef _DEBUG
			// Stats
			D3DCAPS9 DevCaps;
			m_pD3dDev->GetDeviceCaps(&DevCaps);
			GAE_TRACE(L"----------- D3DEV CAPABILITIES ---------\n");
			GAE_TRACE(L"\tMAX PRIMITIVE COUNT: [%u]\n", DevCaps.MaxPrimitiveCount);
			GAE_TRACE(L"\tMAX VERTEX INDEX: [%u]\n", DevCaps.MaxVertexIndex);
			GAE_TRACE(L"----------------------------------------\n");
#endif

			// Register the input callbacks
			GAE_INPUT_REGISTER_EVENT(OnKeyPress, this, &DirectX9::KeyPressed);

			return TRUE;
		}

		void DirectX9::AddShaderFromFile(const std::wstring& fFile, E_SHADER_TYPE eType)
		{
			ID3DXBuffer* pShaderCompiled = NULL;
			ID3DXBuffer* pShaderErrors = NULL;
			ID3DXConstantTable* pShaderConstants = NULL;

			if(ST_PIXEL == eType)
			{	// Pixel shaders.
#				ifdef GAE_USE_PRECOMPILED_SHADER
				const std::wstring strTemp = fFile + L".psc";
#				else
				const std::wstring strTemp = fFile + L".ps";
#				endif
				const wchar_t* wszFilename = strTemp.c_str();
				IDirect3DPixelShader9* pPS = NULL;
#ifdef _DEBUG
				GAE_ASSERT_D3D_FN(D3D_OK, D3DXCompileShaderFromFile(wszFilename, NULL, NULL, "main", D3DXGetPixelShaderProfile(m_pD3dDev), D3DXSHADER_DEBUG, &pShaderCompiled, &pShaderErrors, &pShaderConstants));
#else
				GAE_ASSERT_D3D_FN(D3D_OK, D3DXCompileShaderFromFile(wszFilename, NULL, NULL, "main", D3DXGetPixelShaderProfile(m_pD3dDev), D3DXSHADER_SKIPVALIDATION, &pShaderCompiled, &pShaderErrors, &pShaderConstants));
#endif
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreatePixelShader((DWORD*)pShaderCompiled->GetBufferPointer(), &pPS));
				

				PxlShader shader;
				shader.pShader = pPS;
				shader.pConstants = pShaderConstants;

				m_VecPxlShader.push_back(shader);
			}
			else if(ST_VERTEX == eType)
			{	// Vertex shaders
#			ifdef GAE_USE_PRECOMPILED_SHADER
				const std::wstring strTemp = fFile + L".vsc";
#			else
				const std::wstring strTemp = fFile + L".vs";
#			endif
				const wchar_t* wszFilename = strTemp.c_str();
				IDirect3DVertexShader9* pVS = NULL;
#ifdef _DEBUG
				GAE_ASSERT_D3D_FN(D3D_OK, D3DXCompileShaderFromFile(wszFilename, NULL, NULL, "main", D3DXGetVertexShaderProfile(m_pD3dDev), D3DXSHADER_DEBUG, &pShaderCompiled, &pShaderErrors, &pShaderConstants));
#else
				GAE_ASSERT_D3D_FN(D3D_OK, D3DXCompileShaderFromFile(wszFilename, NULL, NULL, "main", D3DXGetVertexShaderProfile(m_pD3dDev), D3DXSHADER_SKIPVALIDATION, &pShaderCompiled, &pShaderErrors, &pShaderConstants));
#endif
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexShader((DWORD*)pShaderCompiled->GetBufferPointer(), &pVS));

				VtxShader shader;
				shader.pShader = pVS;
				shader.pConstants = pShaderConstants;

				m_VecVtxShader.push_back(shader);
			}
			else
			{
				GAE_ASSERTM(ST_GEOMETRY == eType, L"Geometry shaders are not supported!");
			}
		}

		DirectX9::PxlShader* DirectX9::GetShaderPixel(const E_SHADER_LIST eShader) const
		{
			DirectX9::PxlShader* pShader = NULL;
			switch(eShader)
			{
			case SL_PARALLEL_LIGHT:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(0));
				break;
			case SL_POINT_LIGHT:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(1));
				break;
			case SL_POINT_BUMP:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(2));
				break;
			case SL_POINT_DIFFUSE:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(3));
				break;
			case SL_POINT_PARALLAX:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(4));
				break;
			case SL_AMBIENT:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(5));
				break;
			case SL_DIFFUSE:
				pShader = const_cast<PxlShader*>(&m_VecPxlShader.at(6));
				break;
			}
			GAE_ASSERTM(pShader, L"Invalid shader!");
			return pShader;
		}

		DirectX9::VtxShader* DirectX9::GetShaderVertex(const E_SHADER_LIST eShader) const
		{
			DirectX9::VtxShader* pShader = NULL;
			switch(eShader)
			{
			case SL_PARALLEL_LIGHT:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(0));
				break;
			case SL_POINT_LIGHT:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(1));
				break;
			case SL_POINT_BUMP:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(2));
				break;
			case SL_POINT_DIFFUSE:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(3));
				break;
			case SL_POINT_PARALLAX:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(4));
				break;
			case SL_AMBIENT:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(5));
				break;
			case SL_DIFFUSE:
				pShader = const_cast<VtxShader*>(&m_VecVtxShader.at(6));
				break;
			}
			GAE_ASSERTM(pShader, L"Invalid shader!");
			return pShader;
		}

		void DirectX9::SelectShader(const E_SHADER_LIST eShader) const
		{
			switch(eShader)
			{
			case SL_NONE:
				m_pD3dDev->SetVertexShader(NULL);
				m_pD3dDev->SetPixelShader(NULL);
				break;
			case SL_PARALLEL_LIGHT:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(0).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(0).pShader);
				break;
			case SL_POINT_LIGHT:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(1).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(1).pShader);
				break;
			case SL_POINT_BUMP:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(2).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(2).pShader);
				break;
			case SL_POINT_DIFFUSE:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(3).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(3).pShader);
				break;
			case SL_POINT_PARALLAX:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(4).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(4).pShader);
				break;
			case SL_AMBIENT:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(5).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(5).pShader);
				break;
			case SL_DIFFUSE:
				m_pD3dDev->SetVertexShader(m_VecVtxShader.at(6).pShader);
				m_pD3dDev->SetPixelShader(m_VecPxlShader.at(6).pShader);
				break;
			}

			GAE_ASSERTM(SL_COUNT != m_eLighting, L"Incorrect lighting type!");
		}

		void DirectX9::KeyPressed(UINT uKeyCode)
		{
			if(DIK_3 == uKeyCode)
			{
				m_bWireframe = !m_bWireframe;
				m_pD3dDev->SetRenderState(D3DRS_FILLMODE, m_bWireframe ? D3DFILL_WIREFRAME : D3DFILL_SOLID);
			}
			// Temporarily disabled for Trinigy.
			/*
			else if(DIK_4 == uKeyCode)
			{
				m_eLighting = (E_SHADER_LIST) ((m_eLighting + 1) % SL_COUNT);

				SelectShader(m_eLighting);
			}
			else if(DIK_5 == uKeyCode)
			{
				m_bRenderWithShadows = !m_bRenderWithShadows;
				if(m_bRenderWithShadows)
				{
					m_eLighting = SL_NONE;
					m_pD3dDev->SetVertexShader(NULL);
					m_pD3dDev->SetPixelShader(NULL);
				}
				else
				{
					KeyPressed(DIK_4);
				}
			}
			else if(DIK_6 == uKeyCode)
			{
				m_bRenderWithLights = !m_bRenderWithLights;
			}
			*/
		}

		bool DirectX9::Uninit()
		{
			// Unregister the input callbacks.
			GAE_INPUT_UNREGISTER_EVENT(OnKeyPress, this);

			m_pD3dDev->Release();
			m_pD3d9->Release();

			return TRUE;
		}

		void DirectX9::SetCurrentLight(Gae::Entities::Light* pLight)
		{
			GAE_ASSERTM(NULL != pLight, L"Light is not valid!");
			m_pLight = pLight;
		}

		bool DirectX9::InitMesh(Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERT(NULL != pMesh);
			GAE_ASSERT(NULL != pMesh->m_pVertexBuffer);
			GAE_ASSERT(NULL == pMesh->m_pVertexBuffer->m_pVB);
			GAE_ASSERT(NULL != pMesh->m_pVertexBuffer->m_pData);
			GAE_ASSERT(NULL == pMesh->m_pVertexBuffer->m_pVertexDeclaration);
			GAE_ASSERT(NULL != pMesh->m_pIndexBuffer);			
			GAE_ASSERT(NULL == pMesh->m_pIndexBuffer->m_pIB);			
			GAE_ASSERT(NULL != pMesh->m_pIndexBuffer->m_pData);

			super::InitMesh(pMesh);

			{
				using namespace Gae::Graphics::Geometry;

				static D3DVERTEXELEMENT9 pVertexElementColNormalUVTB[] = 
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
					{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
					{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
					{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0},
					{ 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BINORMAL, 0},
					D3DDECL_END()
				};

				static D3DVERTEXELEMENT9 pVertexElementColNormalUV[] = 
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
					{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
					{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
					D3DDECL_END()
				};

				static D3DVERTEXELEMENT9 pVertexElementNormalUV[] = 
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},					
					{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
					D3DDECL_END()
				};

				static D3DVERTEXELEMENT9 pVertexElementColNormal[] = 
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
					{ 0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
					D3DDECL_END()
				};

				static D3DVERTEXELEMENT9 pVertexElementCol[] =
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					{ 0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
					D3DDECL_END()
				};

				static D3DVERTEXELEMENT9 pVertexElementNormal[] =
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
					D3DDECL_END()
				};

				static D3DVERTEXELEMENT9 pVertexElement[] =
				{	// stream, offset, type, method, usage, usage index
					{ 0, 0, D3DDECLTYPE_FLOAT3,	D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
					D3DDECL_END()
				};

				UINT32 uVertexFormat = pMesh->m_pVertexBuffer->m_uFormat;
				if(uVertexFormat == (VT_COLOR | VT_NORMAL | VT_UV_COORD | VT_TANGENT | VT_BINORMAL))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElementColNormalUVTB, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else if((uVertexFormat & VT_COLOR) && (uVertexFormat & VT_NORMAL) && (uVertexFormat & VT_UV_COORD))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElementColNormalUV, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else if((uVertexFormat & VT_COLOR) && (uVertexFormat & VT_NORMAL))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElementColNormal, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else if((uVertexFormat & VT_NORMAL) && (uVertexFormat & VT_UV_COORD))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElementNormalUV, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else if((uVertexFormat & VT_COLOR))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElementCol, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else if((uVertexFormat & VT_NORMAL))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElementNormal, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else if((!uVertexFormat))
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexDeclaration(pVertexElement, &pMesh->m_pVertexBuffer->m_pVertexDeclaration));
				}
				else
				{
					GAE_ASSERTM(FALSE, L"Invalid vertex format!");
				}
			}

			// Create the index and vertex buffer.
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexBuffer((UINT) pMesh->m_pVertexBuffer->m_sizeData, D3DUSAGE_WRITEONLY, 0 /* non FVF format */, D3DPOOL_DEFAULT, &pMesh->m_pVertexBuffer->m_pVB, NULL));
			//GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateVertexBuffer(pMesh->m_pVertexBuffer->m_sizeData, D3DUSAGE_WRITEONLY, D3DFVF_XYZ | D3DFVF_DIFFUSE, D3DPOOL_DEFAULT, &pMesh->m_pVertexBuffer->m_pVB, NULL));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateIndexBuffer((UINT) pMesh->m_pIndexBuffer->m_sizeData, D3DUSAGE_WRITEONLY, D3DFMT_INDEX32, D3DPOOL_DEFAULT, &pMesh->m_pIndexBuffer->m_pIB, NULL));

			// Copy the vertexes and indexes to the device.
			void** pFirstVertex = pMesh->m_pVertexBuffer->m_pData;
			void** pFirstIndex = pMesh->m_pIndexBuffer->m_pData;

			void* ppDeviceMemory;	// Memory to copy our vertex and index data.
			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pVertexBuffer->m_pVB->Lock(0, (UINT) pMesh->m_pVertexBuffer->m_sizeData, (void**)&ppDeviceMemory, 0));
			{
				GAE_TRACE(L"Size of vertex buffer: [%u]\n", pMesh->m_pVertexBuffer->m_sizeData);
				std::memcpy(ppDeviceMemory, pFirstVertex, pMesh->m_pVertexBuffer->m_sizeData);
			}
			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pVertexBuffer->m_pVB->Unlock());

			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pIndexBuffer->m_pIB->Lock(0, (UINT) pMesh->m_pIndexBuffer->m_sizeData, (void**)&ppDeviceMemory, 0));
			{
				GAE_TRACE(L"Size of index buffer: [%u]\n", pMesh->m_pIndexBuffer->m_sizeData);
				std::memcpy(ppDeviceMemory, pFirstIndex, pMesh->m_pIndexBuffer->m_sizeData);
			}
			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pIndexBuffer->m_pIB->Unlock());
     
			// Creates the material's texture(s).
			// Create the texture from file if it doesn't exist yet.
			Gae::Graphics::Material* pMat = pMesh->m_pMaterial;
			
			//GAE_ASSERTM(NULL != pMat, L"The mesh doesn't have a material assigned!\n");

			if(pMat)
			{
				InitTexture(pMat->m_pMapDiffuse);
				InitTexture(pMat->m_pMapNormal);
				InitTexture(pMat->m_pMapSpecular);
				InitTexture(pMat->m_pMapHeight);

				for each(Gae::Graphics::Material::BlendStage* pBlendStage in pMat->GetVecBlendStage())
				{
					if(!pBlendStage->m_bMakeAlpha)
					{
						InitTexture(pBlendStage->m_pTexture);
					}
					else
					{
						InitTexture(pBlendStage->m_pTexture, pBlendStage->m_bMakeAlpha);
					}
				}

				/*
				if(pMat->FlagEnabled(Material::M_SOLID))
				{
					*/
					GenerateAndEmbeedHeightmap(pMat);
					/*
				}
				*/

				// Process the mask color into the directX format.
				for each(Gae::Graphics::Material::BlendStage* pBlendStage in pMat->GetVecBlendStage())
				{
					DWORD dwMaskColor = pBlendStage->m_dwMaskComponent;
					pBlendStage->m_dwMaskComponent = 0L;
					if(dwMaskColor & (1 << 0)) pBlendStage->m_dwMaskComponent |= D3DCOLORWRITEENABLE_RED;
					if(dwMaskColor & (1 << 1)) pBlendStage->m_dwMaskComponent |= D3DCOLORWRITEENABLE_GREEN;
					if(dwMaskColor & (1 << 2)) pBlendStage->m_dwMaskComponent |= D3DCOLORWRITEENABLE_BLUE;
					if(dwMaskColor & (1 << 3)) pBlendStage->m_dwMaskComponent |= D3DCOLORWRITEENABLE_ALPHA;
				}
			}

#ifdef _DEBUG
			m_uMeshesInDevice++;
#endif // _DEBUG

			return TRUE;
		}

		bool DirectX9::InitTexture(Gae::Core::Resources::Texture* pTex, bool bMakeAlpha/* = false*/)
		{
			if(NULL == pTex) return FALSE;

			// \TODO: Create a map of filenames or write the fucking resource manager :D to check for duplicated textures. x'D

			if(NULL == pTex->m_pTexture)
			{	// Let's load the texture if it is not loaded

				GAE_TRACE(L"\tLoading texture: [%s]\n", pTex->m_wsFilename.c_str());
				LPDIRECT3DTEXTURE9 lpTexture = static_cast<LPDIRECT3DTEXTURE9>(pTex->m_pTexture);

				// If the texture should be unique or it has not been initialized yet create it.
				UINT32 uTextureID = pTex->GetTagCRC32();
				if(m_MapTextures.find(uTextureID) == m_MapTextures.end())
				{
					//GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateTextureFromFile(m_pD3dDev, pTex->m_wsFilename.c_str(), &lpTexture));
					if(bMakeAlpha)
					{
						// TODO: When FMT FROM FILE is used it doesn't work well. Thus, we are doing something wrong depending on the texture format loaded...
						//GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateTextureFromFileEx(m_pD3dDev, pTex->m_wsFilename.c_str(), 0, 0, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &lpTexture));
						GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateTextureFromFileEx(m_pD3dDev, pTex->m_wsFilename.c_str(), 0, 0, 0, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_FILTER_BOX, 0, NULL, NULL, &lpTexture));
					}
					else
					{
						HRESULT hResult = D3D_OK;
						// TODO: Fully review the texture formats used. Create a map for testing.
						GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateTextureFromFile(m_pD3dDev, pTex->m_wsFilename.c_str(), &lpTexture));
						//GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateTextureFromFileEx(m_pD3dDev, pTex->m_wsFilename.c_str(), 0, 0, D3DX_FROM_FILE, 0, D3DFMT_FROM_FILE, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &lpTexture));
					}
					
					m_MapTextures[uTextureID].push_back(pTex);

					// Calculate texel size.
					D3DSURFACE_DESC surfDesc;
					lpTexture->GetLevelDesc(0, &surfDesc);

					pTex->m_pTexture = (void*)lpTexture;
					pTex->m_v2fTexelSize = Vector2f(1.0f / surfDesc.Width, 1.0f / surfDesc.Height);
					pTex->m_fTexelsPerSide = sqrt(surfDesc.Width * surfDesc.Height * 2.0f);

					// Modify the texture with the correct alpha value.
					if(bMakeAlpha)
					{
						/*
						if(surfDesc.Format != D3DFMT_L8)
						{
							*/
							D3DLOCKED_RECT textureNormalRect;
							GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->LockRect(0, &textureNormalRect, NULL, 0));
							{
								for(UINT y = 0; y < surfDesc.Height; ++y)
								{
									for(UINT x = 0; x < surfDesc.Width; ++x)
									{
										unsigned char* pComponents = (unsigned char*) (((UINT32*)textureNormalRect.pBits) + (y * textureNormalRect.Pitch / sizeof(UINT32)) + x);
										pComponents[3] = (pComponents[0] + pComponents[1] + pComponents[2]) / 3;
									}
								}
							}
							lpTexture->UnlockRect(0);
							/*
						}
						*/
					}
				}
				else	// Otherwise assign it.
				{
					pTex->m_pTexture = m_MapTextures[pTex->GetTagCRC32()].at(0)->m_pTexture;
					pTex->m_v2fTexelSize = m_MapTextures[pTex->GetTagCRC32()].at(0)->GetTexelSize();
				}
			}
			else
			{
				//GAE_TRACE(L"\tTexture already loaded. Skipping: [%s]\n", pTex->m_wsFilename.c_str());
			}
			GAE_ASSERTM(NULL != pTex->m_pTexture, L"Texture was not assigned correctly!\n");	

			return TRUE;
		}

		/*
		bool DirectX9::InitTexture(Gae::Core::Resources::Texture* pTex)
		{
			if(NULL == pTex) return FALSE;

			// \TODO: Create a map of filenames or write the fucking resource manager :D to check for duplicated textures. x'D

			if(NULL == pTex->m_pTexture)
			{	// Let's load the texture if it is not loaded

				GAE_TRACE(L"\tLoading texture: [%s]\n", pTex->m_wsFilename.c_str());
				LPDIRECT3DTEXTURE9 lpTexture = static_cast<LPDIRECT3DTEXTURE9>(pTex->m_pTexture);

				// If the texture should be unique or it has not been initialized yet create it.
				UINT32 uTextureID = pTex->GetTagCRC32();
				if(m_MapTextures.find(uTextureID) == m_MapTextures.end())
				{
					GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateTextureFromFile(m_pD3dDev, pTex->m_wsFilename.c_str(), &lpTexture));
					m_MapTextures[uTextureID].push_back(pTex);

					// Calculate texel size.
					D3DSURFACE_DESC surfDesc;
					lpTexture->GetLevelDesc(0, &surfDesc);

					pTex->m_pTexture = (void*)lpTexture;
					//pTex->m_v2fTexelSize = Vector2f(1.0f / lpTexture->Width, 1.0f / lpTexture->Height);
					pTex->m_v2fTexelSize = Vector2f(1.0f / surfDesc.Width, 1.0f / surfDesc.Height);
				}
				else	// Otherwise assign it.
				{
					pTex->m_pTexture = m_MapTextures[pTex->GetTagCRC32()].at(0)->m_pTexture;
					pTex->m_v2fTexelSize = m_MapTextures[pTex->GetTagCRC32()].at(0)->GetTexelSize();
				}
			}
			else
			{
				GAE_TRACE(L"\tTexture already loaded. Skipping: [%s]\n", pTex->m_wsFilename.c_str());
			}
			GAE_ASSERTM(NULL != pTex->m_pTexture, L"Texture was not assigned correctly!\n");	

			return TRUE;
		}
		*/

		IDirect3DTexture9* DirectX9::GetDummyTexture(E_DUMMY_TEXT eTexture) const
		{
			// Create the texture only once...
			static IDirect3DTexture9* pNormalMap = NULL;
			if(NULL == pNormalMap)
			{
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateTexture(2, 2, 1, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pNormalMap, NULL));

				D3DLOCKED_RECT textureNormalRect;
				UINT32 uColor = D3DCOLOR_XRGB(128, 128, 255);
				GAE_ASSERT_D3D_FN(D3D_OK, pNormalMap->LockRect(0, &textureNormalRect, NULL, 0));
				{
					for(UINT y = 0; y < 2; ++y)
					{
						for(UINT x = 0; x < 2; ++x)
						{
							*(((UINT32*)textureNormalRect.pBits) + (y * textureNormalRect.Pitch / sizeof(UINT32)) + x) = uColor;
						}
					}
				}
				pNormalMap->UnlockRect(0);
			}

			// Create the texture only once...
			static IDirect3DTexture9* pSpecularMap = NULL;
			if(NULL == pSpecularMap)
			{
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateTexture(2, 2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pSpecularMap, NULL));

				D3DLOCKED_RECT textureNormalRect;
				UINT32 uColor = D3DCOLOR_XRGB(0, 0, 0);
				GAE_ASSERT_D3D_FN(D3D_OK, pSpecularMap->LockRect(0, &textureNormalRect, NULL, 0));
				{
					for(UINT y = 0; y < 2; ++y)
					{
						for(UINT x = 0; x < 2; ++x)
						{
							*(((UINT32*)textureNormalRect.pBits) + (y * textureNormalRect.Pitch / sizeof(UINT32)) + x) = uColor;
						}
					}
				}
				pSpecularMap->UnlockRect(0);
			}

			switch(eTexture)
			{
			case DT_NORMAL:
				return pNormalMap;
				break;
			case DT_SPECULAR:
				return pSpecularMap;
				break;
			default:
				return NULL;
			}
		}

		void DirectX9::GenerateAndEmbeedHeightmap(Gae::Graphics::Material* pMat)
		{
			if(NULL == pMat) return;
			if(NULL == pMat->m_pMapDiffuse) return;

			// Create a standard (0.0f, 0.0f, 1.0f) normal map if it doesn't exist.
			if(NULL == pMat->m_pMapNormal)
			{	// Create the texture. 
				pMat->m_pMapNormal = new Gae::Core::Resources::Texture;

				// ...and assign it multiple times.
				pMat->m_pMapNormal->m_pTexture = (void*)GetDummyTexture(DT_NORMAL);
				pMat->m_pMapNormal->m_v2fTexelSize << 0.5f, 0.5f;
			}

			// Create a standard (255.0f, 255.0f, 255.0f) specular map if it doesn't exist.
			if(NULL == pMat->m_pMapSpecular)
			{	// Create the texture.
				pMat->m_pMapSpecular = new Gae::Core::Resources::Texture;
				// ...and assign it multiple times.
				pMat->m_pMapSpecular->m_pTexture = (void*)GetDummyTexture(DT_SPECULAR);
				pMat->m_pMapSpecular->m_v2fTexelSize << 0.5f, 0.5f;
			}
			// Create a standard (255.0f, 255.0f, 255.0f) diffuse map if it doesn't exist.
			/*
			if(NULL == pMat->m_pMapDiffuse)
			{	// Create the texture.
				pMat->m_pMapDiffuse = new Gae::Core::Resources::Texture;

				// Create the texture only once...
				static IDirect3DTexture9* pDiffuseMap = NULL;
				if(NULL == pDiffuseMap)
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateTexture(2, 2, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pDiffuseMap, NULL));

					D3DLOCKED_RECT textureNormalRect;
					UINT32 uColor = D3DCOLOR_ARGB(0, 255, 255, 255);
					GAE_ASSERT_D3D_FN(D3D_OK, pDiffuseMap->LockRect(0, &textureNormalRect, NULL, 0));
					{
						for(UINT y = 0; y < 2; ++y)
						{
							for(UINT x = 0; x < 2; ++x)
							{
								*(((UINT32*)textureNormalRect.pBits) + (y * textureNormalRect.Pitch / sizeof(UINT32)) + x) = uColor;
							}
						}
					}
					pDiffuseMap->UnlockRect(0);
				}

				// ...and assign it multiple times.
				pMat->m_pMapDiffuse->m_pTexture = (void*)pDiffuseMap;
				pMat->m_pMapDiffuse->m_v2fTexelSize << 0.5f, 0.5f;
			}
			return;
			*/

			if(ContainsHeightmap(pMat))
			{	// NOTE - For Wulfen texture pack 1.0. 
				//	A heightmap is embedded in the alpha component of specular map but not all
				// of the specular maps contains a valid heightmap. Check if every field of the
				// heightmap (alpha component of the specular one) contains always the same
				// value and tag the material to be not rendered with Parallax Mapping.
				pMat->FlagSet(Gae::Graphics::Material::M_HEIGHTMAP);
			}
			else
			{
				return;
				if(NULL == pMat->m_pMapHeight)
				{	// Create the texture.
					pMat->m_pMapHeight = new Gae::Core::Resources::Texture;
				}

				Gae::Core::Resources::Texture* pTexNormal = pMat->m_pMapNormal;
				Gae::Core::Resources::Texture* pTexHeight = pMat->m_pMapHeight;
				Gae::Core::Resources::Texture* pTexSpecular = pMat->m_pMapSpecular;

				GAE_ASSERTM(NULL != pTexNormal->m_pTexture, L"We need the texture data for generating the heightmap!\n");
				GAE_TRACE(L"\tGenerating and embedding Heightmap in A channel of Specular Map for texture: [%s]\n", pTexNormal->m_wsFilename.c_str());
				LPDIRECT3DTEXTURE9 lpTexture = static_cast<LPDIRECT3DTEXTURE9>(pTexNormal->m_pTexture);
				//LPDIRECT3DTEXTURE9 lpTextureHeight = static_cast<LPDIRECT3DTEXTURE9>(pTexHeight->m_pTexture);
				LPDIRECT3DTEXTURE9 lpTextureSpecular = static_cast<LPDIRECT3DTEXTURE9>(pTexSpecular->m_pTexture);

				// Normal map
				LPDIRECT3DSURFACE9 lpSurface = NULL;
				GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->GetSurfaceLevel(0, &lpSurface));
				D3DSURFACE_DESC surfaceDesc;
				GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->GetLevelDesc(0, &surfaceDesc));

				// Specular map
				LPDIRECT3DSURFACE9 lpSurfaceSpecular = NULL;
				GAE_ASSERT_D3D_FN(D3D_OK, lpTextureSpecular->GetSurfaceLevel(0, &lpSurfaceSpecular));
				D3DSURFACE_DESC surfaceDescSpecular;
				GAE_ASSERT_D3D_FN(D3D_OK, lpTextureSpecular->GetLevelDesc(0, &surfaceDescSpecular));
				
				GAE_ASSERTM(D3DFMT_A8R8G8B8 == surfaceDescSpecular.Format || D3DFMT_X8R8G8B8 == surfaceDescSpecular.Format, L"Wrong texture format!");
				
				// Create the HeightTexture
				IDirect3DTexture9* pHeightMap = NULL;
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->CreateTexture(surfaceDesc.Width, surfaceDesc.Height, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pHeightMap, NULL));

				D3DLOCKED_RECT textureNormalRect;
				D3DLOCKED_RECT textureHeightRect;
				GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->LockRect(0, &textureNormalRect, NULL, D3DLOCK_READONLY));	// Retrieve the texture data
				GAE_ASSERT_D3D_FN(D3D_OK, pHeightMap->LockRect(0, &textureHeightRect, NULL, 0));	// Retrieve the texture data
				{
					float fMaxHeight = -32768.0f;
					float fMinHeight = -fMaxHeight;
					float fVariance = 0.0f;
					
					for(UINT x = 0; x < surfaceDesc.Width; ++x)
					{
						float iCurrentHeight = 0.0;
						
						for(UINT y = 0; y < surfaceDesc.Height; ++y)
						{
							UINT32 uNormal = *(((UINT32*)textureNormalRect.pBits) + (y * (textureNormalRect.Pitch / sizeof(UINT32))) + x);
							//float fRed = -((2.0f * ((1.0 * ((uNormal & 0x00FF0000) >> 16)) / 256.0f)) - 1.0f);
							float fGreen = ((2.0f * ((1.0 * ((uNormal & 0x0000FF00) >> 8)) / 256.0f)) - 1.0f);
							//float fBlue = ((2.0f * ((1.0 * ((uNormal & 0x000000FF))) / 256.0f)) - 1.0f);

							iCurrentHeight += fGreen > 0.0f ? 0.0078125f : (fGreen < 0.0f ? -0.0078125f : 0.0f);// > 0.0f ? fBlue : fRed < 0.0f ? -fBlue : 0.0f;

							if(iCurrentHeight > fMaxHeight)
							{
								fMaxHeight = iCurrentHeight;
							}

							if(iCurrentHeight < fMinHeight)
							{ 
								fMinHeight = iCurrentHeight;
							}
	  
							*(((float*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) += iCurrentHeight;
						}   
	  
						iCurrentHeight = 0;
						for(int y = surfaceDesc.Height - 1; y >= 0; --y)
						{
							UINT32 uNormal = *(((UINT32*)textureNormalRect.pBits) + (y * textureNormalRect.Pitch / sizeof(UINT32)) + x);
							//float fRed = -((2.0f * ((1.0 * ((uNormal & 0x00FF0000) >> 16)) / 256.0f)) - 1.0f);
							float fGreen = ((2.0f * ((1.0 * ((uNormal & 0x0000FF00) >> 8)) / 256.0f)) - 1.0f);
							//float fBlue = ((2.0f * ((1.0 * ((uNormal & 0x000000FF))) / 256.0f)) - 1.0f);
	 
							iCurrentHeight += fGreen;// > 0.0f ? fBlue : fRed < 0.0f ? -fBlue : 0.0f;

							if(iCurrentHeight > fMaxHeight)
							{
								fMaxHeight = iCurrentHeight;
							}

							if(iCurrentHeight < fMinHeight) 
							{
								fMinHeight = iCurrentHeight;
							}

							*(((float*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) += iCurrentHeight;	
						}
					}

					/*
					for(UINT x = 0; x < surfaceDesc.Width; ++x)
					{
						int iCurrentHeight = 0;
						for(UINT y = 0; y < surfaceDesc.Height; ++y)
						{
							UINT32 uNormal = *(((UINT32*)textureNormalRect.pBits) + (y * (textureNormalRect.Pitch / sizeof(UINT32))) + x);
							UINT32 fRed = (uNormal & 0x00FF0000) >> 16;
							UINT32 fGreen = (uNormal & 0x0000FF00) >> 8;
							UINT32 fBlue = uNormal & 0x000000FF;

							if(fGreen > 129 || fGreen < 126)
							{
								if(fGreen > 129)
								{ 
									iCurrentHeight += 1;
								}
								else
								{
									iCurrentHeight -= 1;
								}
							}
	   
							*(((int*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) = iCurrentHeight;
						}

						iCurrentHeight = 0;
						for(int y = surfaceDesc.Height - 1; y >= 0; --y)
						{
							UINT32 uNormal = *(((UINT32*)textureNormalRect.pBits) + (y * (textureNormalRect.Pitch / sizeof(UINT32))) + x);
							UINT32 fRed = (uNormal & 0x00FF0000) >> 16;
							UINT32 fGreen = (uNormal & 0x0000FF00) >> 8;
							UINT32 fBlue = uNormal & 0x000000FF;

							if(fGreen > 129 || fGreen < 126)
							{
								if(fGreen > 129)
								{ 
									iCurrentHeight += 1;
								}
								else
								{
									iCurrentHeight -= 1;
								}
							}

							*(((int*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) += iCurrentHeight;
						}
					}
					*/

					lpTexture->UnlockRect(0);	// We do not need the normal map anymore.
					fMaxHeight = -1000000.0f;
					fMinHeight = -fMaxHeight;
					for(UINT x = 0; x < surfaceDesc.Width; ++x)
					{
						float iCurrentHeight = 0;
						for(UINT y = 0; y < surfaceDesc.Height; ++y)
						{
							iCurrentHeight = *(((float*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x);

							if(iCurrentHeight > fMaxHeight)
							{
								fMaxHeight = iCurrentHeight;
							}

							if(iCurrentHeight < fMinHeight)
							{
								fMinHeight = iCurrentHeight;
							}
						}
					}

					D3DLOCKED_RECT textureSpecularRect;
					GAE_ASSERT_D3D_FN(D3D_OK, lpTextureSpecular->LockRect(0, &textureSpecularRect, NULL, D3DLOCK_READONLY));	// Retrieve the texture data
					// Normalize and store the values.
					for(UINT y = 0; y < surfaceDesc.Height; ++y)
					{
						for(UINT x = 0; x < surfaceDesc.Width; ++x)
						{
							//UINT32 uHeight = (UINT32)(((((*(((int*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x)) / (float)fMaxHeight)) * 128.0f) + 128.0f);
							float iHeight = *(((float*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x);
							iHeight = (iHeight-fMinHeight) / (fMaxHeight - fMinHeight); // Convert to 0.0f <-> 1.0f range.
							UINT32 uInRange0To256 = (UINT32)(iHeight * 255.0f) >= 255 ? 255 : (UINT32)(iHeight * 255.0f);
							GAE_ASSERTM(uInRange0To256 <= 255 && uInRange0To256 >= 0, L"Incorrect normalization!");

							unsigned char* pXRGBSpecular = ((unsigned char*)(((UINT32*)textureSpecularRect.pBits) + (y * textureSpecularRect.Pitch / sizeof(UINT32)) + x));
							*(((UINT32*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) = D3DCOLOR_ARGB((unsigned char)uInRange0To256, *(pXRGBSpecular+1), *(pXRGBSpecular+2), *(pXRGBSpecular+3));
							//unsigned char ucVal = (unsigned char)uInRange0To256;
							//*(((UINT32*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) = D3DCOLOR_ARGB(ucVal, ucVal, ucVal, ucVal);
							
						}
					}
					lpTextureSpecular->UnlockRect(0);
	/*
					for(UINT y = 0; y < surfaceDesc.Height; ++y)
					{
						int iCurrentHeight = 0;
						for(UINT x = 0; x < surfaceDesc.Width; ++x)
						{
							UINT32 uNormal = *(((UINT32*)textureNormalRect.pBits) + (y * (textureNormalRect.Pitch / sizeof(UINT32))) + x);
							UINT32 fRed = (uNormal & 0x00FF0000) >> 16;
							UINT32 fGreen = (uNormal & 0x0000FF00) >> 8;
							UINT32 fBlue = uNormal & 0x000000FF;

							if(fRed > 129 || fRed < 126)
							{
								if(fRed > 129)
								{ 
									iCurrentHeight += 1;
								}
								else
								{
									iCurrentHeight -= 1;
								}
							}

							*(((int*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) = iCurrentHeight;
						}
					}
					*/

					

	 /*
					int iMaxHeight = INT_MIN;
					int iMinHeight = INT_MAX;
					for(UINT x = 0; x < surfaceDesc.Width; ++x)
					{
						int iCurrentHeight = 0;
						for(UINT y = 0; y < surfaceDesc.Height; ++y)
						{
							iCurrentHeight = *(((int*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x);

							if(iCurrentHeight > iMaxHeight)
							{
								iMaxHeight = iCurrentHeight;
							}

							if(iCurrentHeight < iMinHeight)
							{
								iMinHeight = iCurrentHeight;
							}
						}
					}

					// Normalize the values.
					for(UINT y = 0; y < surfaceDesc.Height; ++y)
					{
						for(UINT x = 0; x < surfaceDesc.Width; ++x)
						{
							int iHeight = *(((int*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x);
							float fHeight = ((float)iHeight-(float)iMinHeight) / ((float)iMaxHeight - (float)iMinHeight); // Convert to 0.0f <-> 1.0f range.
							UINT32 uInRange0To255 = (UINT32)(fHeight * 255.0f) > 255 ? 255 : (UINT32)(fHeight * 255.0f);
							GAE_ASSERTM(uInRange0To255 < 256 && uInRange0To255 >= 0, L"Incorrect normalization!");

							*(((UINT32*)textureHeightRect.pBits) + (y * textureHeightRect.Pitch / sizeof(UINT32)) + x) = D3DCOLOR_XRGB((unsigned char)uInRange0To255, (unsigned char)uInRange0To255, (unsigned char)uInRange0To255);
						}
					}
					*/

				}
				pHeightMap->UnlockRect(0);

				pTexHeight->m_pTexture = (void*)pHeightMap;
				//pTexSpecular->m_pTexture = (void*)pHeightMap;	// Memory leak for sure because we loose the old pointer to the specular texture. TODO - FIX THIS SHIT
				pMat->FlagSet(Gae::Graphics::Material::M_HEIGHTMAP);
			}
		}

		bool DirectX9::ContainsHeightmap(Gae::Graphics::Material* pMat)
		{
			if (!pMat || !pMat->m_pMapSpecular) return false;

			Gae::Core::Resources::Texture* pTexNormal = pMat->m_pMapSpecular;

			GAE_TRACE(L"\tEvaluating Heightmap in A channel for texture: [%s]\n", pTexNormal->m_wsFilename.c_str());
			LPDIRECT3DTEXTURE9 lpTexture = static_cast<LPDIRECT3DTEXTURE9>(pTexNormal->m_pTexture);

			LPDIRECT3DSURFACE9 lpSurface = NULL;
			GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->GetSurfaceLevel(0, &lpSurface));
			D3DSURFACE_DESC surfaceDesc;
			GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->GetLevelDesc(0, &surfaceDesc));
			if(D3DFMT_X8R8G8B8 == surfaceDesc.Format) return false; // Not alpha channel? No heightmap.
			GAE_ASSERTM(D3DFMT_A8R8G8B8 == surfaceDesc.Format || D3DFMT_X8R8G8B8 == surfaceDesc.Format, L"Wrong texture format!");
			
			// Start evaluation of A component.
			D3DLOCKED_RECT textureNormalRect;
			bool bContainValidHeightmap = false;
			GAE_ASSERT_D3D_FN(D3D_OK, lpTexture->LockRect(0, &textureNormalRect, NULL, 0));	// Retrieve the texture data
			{	// Extract first value and start the comparison.
				UINT32 uFirstARGB = *(((UINT32*)textureNormalRect.pBits) + (/*y*/0 * (textureNormalRect.Pitch / sizeof(UINT32))) + /*x*/0);
				const BYTE c_u8Height = (BYTE)((uFirstARGB & 0xFF000000) >> 24);

				for(UINT x = 0; x < surfaceDesc.Width; ++x)
				{
					for(UINT y = 0; y < surfaceDesc.Height; ++y)
					{
						UINT32 uARGB = *(((UINT32*)textureNormalRect.pBits) + (y * (textureNormalRect.Pitch / sizeof(UINT32))) + x);
						if(c_u8Height != (BYTE)((uARGB & 0xFF000000) >> 24)) 
						{
							bContainValidHeightmap = true;
							break;
						}
					}
					if (bContainValidHeightmap) break;
				}
			}
			lpTexture->UnlockRect(0);
			if(!bContainValidHeightmap)
			{
				GAE_TRACE(L"FUNCIONA!!!");
			}
			return bContainValidHeightmap;
		}
		
		bool DirectX9::UninitMesh(Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERT(NULL != pMesh);
			GAE_ASSERT(NULL != pMesh->m_pVertexBuffer);
			GAE_ASSERT(NULL != pMesh->m_pVertexBuffer->m_pVB);
			GAE_ASSERT(NULL != pMesh->m_pVertexBuffer->m_pData);
			GAE_ASSERT(NULL != pMesh->m_pVertexBuffer->m_pVertexDeclaration);
			GAE_ASSERT(NULL != pMesh->m_pIndexBuffer);			
			GAE_ASSERT(NULL != pMesh->m_pIndexBuffer->m_pIB);			
			GAE_ASSERT(NULL != pMesh->m_pIndexBuffer->m_pData);

			super::UninitMesh(pMesh);

			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pVertexBuffer->m_pVertexDeclaration->Release());
			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pVertexBuffer->m_pVB->Release());
			GAE_ASSERT_D3D_FN(D3D_OK, pMesh->m_pIndexBuffer->m_pIB->Release());
			
			// Releases the texture.
			if(pMesh->m_pMaterial->m_pMapDiffuse)
			{
				if(pMesh->m_pMaterial->m_pMapDiffuse->m_pTexture)
				{
					GAE_ASSERT_D3D_FN(D3D_OK, ((LPDIRECT3DTEXTURE9)(pMesh->m_pMaterial->m_pMapDiffuse->m_pTexture))->Release());
				}
			} 
 
#ifdef _DEBUG
			m_uMeshesInDevice--;
#endif // _DEBUG

			return TRUE;
		}

		void DirectX9::ComputeViewport(const Vector4f* av4fVertexes, int iVtxCount, RECT& outViewport) const
		{
			super::ComputeViewport(av4fVertexes, iVtxCount, outViewport);

			DWORD dwScreenWidth = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth-1;
			DWORD dwScreenHeight = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight-1;

			// Invert the y-clip coordinate in order to match the directX screen coordinates system (y = 0 = top)
			outViewport.top = dwScreenHeight - outViewport.top;
			outViewport.bottom = dwScreenHeight - outViewport.bottom;
		}

		void DirectX9::Render(const Gae::Graphics::Mesh* pMesh, bool bFirstPass/* = true*/)
		{
			if(NULL == pMesh) return;
			const Gae::Graphics::Material* pMat = pMesh->m_pMaterial;
			if(NULL == pMat) return;
			//if(!pMat->FlagEnabled(Material::M_SOLID)) return;

			static bool bFirstPassPrev = false;
			if (bFirstPassPrev == false && bFirstPass == true)
			{
				m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
			bFirstPassPrev = bFirstPass;

			if(bFirstPass)// && !pMat->FlagEnabled(Material::M_BLEND))
			{
				RenderFirstPass(pMesh);
			}
			else	// Blending passes (for each light)
			{
				//if(pMat->FlagEnabled(Material::M_BLEND | Material::M_DECAL)) return;
				Gae::Core::Resources::Texture* pMapDiffuse = pMat->m_pMapDiffuse;
				Gae::Core::Resources::Texture* pMapSpecular = pMat->m_pMapSpecular;
				Gae::Core::Resources::Texture* pMapNormal = pMat->m_pMapNormal;
				Gae::Core::Resources::Texture* pMapHeight = pMat->m_pMapHeight;

				if(pMapDiffuse)
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(0, (LPDIRECT3DTEXTURE9)pMapDiffuse->m_pTexture));
					m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, m_TexAddressing);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, m_TexAddressing);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSW, m_TexAddressing);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, m_TexMagFilter);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, m_TexMinFilter);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_MIPFILTER, m_TexMipFilter);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
					m_pD3dDev->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0);					
				}
				else
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(0, NULL));
				}

				if(pMapSpecular)
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(1, (LPDIRECT3DTEXTURE9)pMapSpecular->m_pTexture));
					m_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSU, m_TexAddressing);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSV, m_TexAddressing);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_ADDRESSW, m_TexAddressing);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_MAGFILTER, m_TexMagFilter);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_MINFILTER, m_TexMinFilter);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_MIPFILTER, m_TexMipFilter);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_MAXANISOTROPY, 16);
					m_pD3dDev->SetSamplerState(1, D3DSAMP_MAXMIPLEVEL, 0);					
				}
				else
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(1, NULL));
				}

				if(pMapNormal)
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(2, (LPDIRECT3DTEXTURE9)pMapNormal->m_pTexture));
					m_pD3dDev->SetSamplerState(2, D3DSAMP_ADDRESSU, m_TexAddressing);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_ADDRESSV, m_TexAddressing);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_ADDRESSW, m_TexAddressing);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MAGFILTER, m_TexMagFilter);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MINFILTER, m_TexMinFilter);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MIPFILTER, m_TexMipFilter);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MAXANISOTROPY, 16);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MAXMIPLEVEL, 0);					
				}
				else
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(2, NULL));
				}

				if(pMapHeight)
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(3, (LPDIRECT3DTEXTURE9)pMapHeight->m_pTexture));
					m_pD3dDev->SetSamplerState(2, D3DSAMP_ADDRESSU, m_TexAddressing);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_ADDRESSV, m_TexAddressing);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_ADDRESSW, m_TexAddressing);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MAGFILTER, m_TexMagFilter);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MINFILTER, m_TexMinFilter);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MIPFILTER, m_TexMipFilter);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MAXANISOTROPY, 16);
					m_pD3dDev->SetSamplerState(2, D3DSAMP_MAXMIPLEVEL, 0);					
				}
				else
				{
					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(3, NULL));
				}

				{	// Set whether to use parallax or not for this material.
					PxlShader* pPxlShader = GetShaderPixel(SL_POINT_BUMP);
					D3DXHANDLE hUseParallax = pPxlShader->pConstants->GetConstantByName(NULL, "g_bUseParallax");

					static bool bUseParallax = true;
					if(::GetAsyncKeyState('V'))
					{
						bUseParallax = !bUseParallax;
						while(::GetAsyncKeyState('V'));
						if ( false == bUseParallax)
						{
							GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetBool(m_pD3dDev, hUseParallax, false));
						}
					}

					static bool bMatHasParallax = false;
					if (bMatHasParallax != pMat->FlagEnabled(Gae::Graphics::Material::M_HEIGHTMAP))
					{
						bMatHasParallax = !bMatHasParallax;
						if(bUseParallax)
						{
							GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetBool(m_pD3dDev, hUseParallax, bMatHasParallax));
						}
					}
				}


				Gae::Graphics::Geometry::VertexBuffer* pVB = pMesh->m_pVertexBuffer;
				Gae::Graphics::Geometry::IndexBuffer* pIB = pMesh->m_pIndexBuffer;
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexDeclaration(pVB->m_pVertexDeclaration));
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetStreamSource(0, pVB->m_pVB, 0, (UINT) pVB->m_sizeVertex));
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetIndices(pIB->m_pIB));

				if (pMat->FlagEnabled(Gae::Graphics::Material::M_TWOSIDED))
				{
					m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				}
				else
				{
					m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
				}

				if (pMat->FlagEnabled(Gae::Graphics::Material::M_ALPHATEST))
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

					m_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD) pMat->m_fAlphaTest);
					m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
				}
				else
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
				}

				m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				if(pMat->FlagEnabled(Gae::Graphics::Material::M_BLEND))
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					//m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

					m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pMat->GetBlendSrc());
					m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pMat->GetBlendDst());
				}
				else
				{				
					m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
					m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				}

				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));

			}

			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
		}

		void DirectX9::RenderOld(const Gae::Graphics::Mesh* pMesh, bool bFirstPass/* = true*/)
		{
			const Gae::Graphics::Material* pMat = pMesh->m_pMaterial;
			if(NULL == pMat) return;

			Gae::Core::Resources::Texture* pMapDiffuse = pMat->m_pMapDiffuse;
			Gae::Core::Resources::Texture* pMapSpecular = pMat->m_pMapSpecular;
			Gae::Core::Resources::Texture* pMapNormal = pMat->m_pMapNormal;
			Gae::Core::Resources::Texture* pMapHeight = pMat->m_pMapHeight;
			
			//if(bFirstPass && !pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID)) return;

			if(!pMapDiffuse || !pMapNormal || !pMapSpecular) return;

			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(0, (LPDIRECT3DTEXTURE9)pMapDiffuse->m_pTexture));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(1, (LPDIRECT3DTEXTURE9)pMapSpecular->m_pTexture));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(2, (LPDIRECT3DTEXTURE9)pMapNormal->m_pTexture));
			//GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(3, (LPDIRECT3DTEXTURE9)pMapHeight->m_pTexture));

			if(bFirstPass)
			{
				RenderFirstPass(pMesh);
				return;
			}

			Gae::Graphics::Geometry::VertexBuffer* pVB = pMesh->m_pVertexBuffer;
			Gae::Graphics::Geometry::IndexBuffer* pIB = pMesh->m_pIndexBuffer;
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexDeclaration(pVB->m_pVertexDeclaration));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetStreamSource(0, pVB->m_pVB, 0, (UINT) pVB->m_sizeVertex));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetIndices(pIB->m_pIB));


			if (pMat->FlagEnabled(Gae::Graphics::Material::M_ALPHATEST))
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

				m_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD) pMat->m_fAlphaTest);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}

			m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

			//if(pMat->FlagEnabled(Material::M_SOLID))
			if(pMat->FlagEnabled(Gae::Graphics::Material::M_SOLID))
			{
				if(!bFirstPass)
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

					m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pMat->GetBlendSrc());
					m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pMat->GetBlendDst());

					/*
					m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
					m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

					if(pMat->FlagEnabled(Material::M_DECAL))
					{
						m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
						m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
					}
					else if(pMat->FlagEnabled(Material::M_BLEND))
					{
						m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
						m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					}
					*/
				}
				else
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				}

				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));		

			}
			else
			{				
				m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

				float fDepthBias = -0.0000152587890625f;
				float fSlopeBias = pMat->GetPolygonOffset() * 1.0f;
				m_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fDepthBias);
				m_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&fSlopeBias);

				if(!bFirstPass)
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

					m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pMat->GetBlendSrc());
					m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pMat->GetBlendDst());

					/*
					if(pMat->FlagEnabled(Material::M_DECAL))
					{
						m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
						m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
					}
					else if(pMat->FlagEnabled(Material::M_BLEND))
					{
						m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
						m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
					}
					*/
				}
				else
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				}

				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));

				/*
				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				m_pD3dDev->SetPixelShader(NULL);
				for each(const Material::BlendStage* pBlendStage in pMat->GetVecBlendStage())
				{
					m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pBlendStage->m_dwBlendSrc);
					m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pBlendStage->m_dwBlendDst);
					m_pD3dDev->SetTexture(0,  (LPDIRECT3DTEXTURE9)pBlendStage->m_pTexture->m_pTexture);

					GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));
				}
				*/
			}

			m_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, 0);
			m_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
		}

		void DirectX9::SetTwoSided(const Gae::Graphics::Material* pMat)
		{
			// double sided or single sided? (cull nothing or back sides?)
			static _D3DCULL eOldCull = D3DCULL_CCW;
			bool bTwoSided = pMat->FlagEnabled(Gae::Graphics::Material::M_TWOSIDED);

			if (eOldCull == D3DCULL_CCW && bTwoSided)
			{
				eOldCull = D3DCULL_NONE;
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			else if (eOldCull == D3DCULL_NONE && !bTwoSided)
			{
				eOldCull = D3DCULL_CCW;
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
		}

		void DirectX9::SetAlphaTest(const Gae::Graphics::Material* pMat)
		{
			// alpha tested or not?
			static bool bOldAlphaTest = false;
			bool bAlphaTest = pMat->FlagEnabled(Gae::Graphics::Material::M_ALPHATEST);

			if (bAlphaTest)
			{
				bOldAlphaTest = true;
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD) pMat->m_fAlphaTest);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else if (bOldAlphaTest == true && !bAlphaTest)
			{
				bOldAlphaTest = false;
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
		}

		void DirectX9::RenderFirstPass(const Gae::Graphics::Mesh* pMesh)
		{
			if(NULL == pMesh) return;
			const Gae::Graphics::Material* pMat = pMesh->m_pMaterial;
			if(NULL == pMat) return;			

			Gae::Core::Resources::Texture* pMapDiffuse = pMat->m_pMapDiffuse;
			Gae::Core::Resources::Texture* pMapSpecular = pMat->m_pMapSpecular;
			Gae::Core::Resources::Texture* pMapNormal = pMat->m_pMapNormal;
			Gae::Core::Resources::Texture* pMapHeight = pMat->m_pMapHeight;
			
			if(pMapDiffuse)
			{
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(0, (LPDIRECT3DTEXTURE9)pMapDiffuse->m_pTexture)); // We only want simple black for everything in the first pass. Fill Z-Buffer.
				m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSU, m_TexAddressing);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, m_TexAddressing);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSW, m_TexAddressing);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, m_TexMagFilter);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, m_TexMinFilter);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MIPFILTER, m_TexMipFilter);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0);					
			}
			else
			{
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetTexture(0, NULL)); // We only want simple black for everything in the first pass. Fill Z-Buffer.
			}

			// double sided or single sided? (cull nothing or back sides?)
			SetTwoSided(pMat);
			/*
			static _D3DCULL eOldCull = D3DCULL_CCW;
			if (eOldCull == D3DCULL_CCW && pMat->FlagEnabled(Gae::Graphics::Material::M_TWOSIDED))
			{
				eOldCull = D3DCULL_NONE;
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			else if (eOldCull == D3DCULL_NONE && !pMat->FlagEnabled(Gae::Graphics::Material::M_TWOSIDED))
			{
				eOldCull = D3DCULL_CCW;
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			//m_pD3dDev->SetRenderState(D3DRS_CULLMODE, pMat->FlagEnabled(Gae::Graphics::Material::M_TWOSIDED) ? D3DCULL_NONE : D3DCULL_CCW);
			*/

			SetAlphaTest(pMat);
			/*
			if (pMat->FlagEnabled(Gae::Graphics::Material::M_ALPHATEST))
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD) pMat->m_fAlphaTest);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
			*/

			// Configure stream
			Gae::Graphics::Geometry::VertexBuffer* pVB = pMesh->m_pVertexBuffer;
			Gae::Graphics::Geometry::IndexBuffer* pIB = pMesh->m_pIndexBuffer;
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexDeclaration(pVB->m_pVertexDeclaration));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetStreamSource(0, pVB->m_pVB, 0, (UINT) pVB->m_sizeVertex));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetIndices(pIB->m_pIB));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));

			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		
		}

		void DirectX9::RenderBlendStages(const Gae::Graphics::Mesh* pMesh)
		{
			const Gae::Graphics::Material* pMat = pMesh->m_pMaterial;

			Gae::Graphics::Geometry::VertexBuffer* pVB = pMesh->m_pVertexBuffer;
			Gae::Graphics::Geometry::IndexBuffer* pIB = pMesh->m_pIndexBuffer;
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexDeclaration(pVB->m_pVertexDeclaration));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetStreamSource(0, pVB->m_pVB, 0, (UINT) pVB->m_sizeVertex));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetIndices(pIB->m_pIB));

			SetTwoSided(pMat);
			/*
			if (pMat->FlagEnabled(Gae::Graphics::Material::M_TWOSIDED))
			{
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			else
			{
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			}
			*/

			SetAlphaTest(pMat);
			/*
			if (pMat->FlagEnabled(Gae::Graphics::Material::M_ALPHATEST))
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				//m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

				m_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD) pMat->m_fAlphaTest);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
				//m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_EQUAL);
				//m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_LESSEQUAL);
			}
			else
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}
			*/

			//m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			/*
			if(pMat->GetMapDiffuse() && (!pMat->GetMapSpecular() && !pMat->GetMapNormal()))
			//if(pMat->GetMapDiffuse())
			{
				m_pD3dDev->SetTexture(0, (LPDIRECT3DTEXTURE9)pMat->GetMapDiffuse()->m_pTexture);
				m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pMat->GetBlendSrc());
				m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pMat->GetBlendDst());
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));
			}
			*/

			for each(const Gae::Graphics::Material::BlendStage* pBlendStage in pMat->GetVecBlendStage())
			{
				m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, pBlendStage->m_dwMaskComponent);

				m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pBlendStage->m_dwBlendSrc);
				m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pBlendStage->m_dwBlendDst);
				m_pD3dDev->SetTexture(0,  (LPDIRECT3DTEXTURE9)pBlendStage->m_pTexture->m_pTexture);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSV, m_TexAddressing);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_ADDRESSW, m_TexAddressing);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MAGFILTER, m_TexMagFilter);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MINFILTER, m_TexMinFilter);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MIPFILTER, m_TexMipFilter);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);
				m_pD3dDev->SetSamplerState(0, D3DSAMP_MAXMIPLEVEL, 0);					

				//m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));
			}
 
			//m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			//m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			//m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		}

		void DirectX9::RenderBlendStagesOld(const Gae::Graphics::Mesh* pMesh)
		{
			const Gae::Graphics::Material* pMat = pMesh->m_pMaterial;

			Gae::Graphics::Geometry::VertexBuffer* pVB = pMesh->m_pVertexBuffer;
			Gae::Graphics::Geometry::IndexBuffer* pIB = pMesh->m_pIndexBuffer;
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexDeclaration(pVB->m_pVertexDeclaration));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetStreamSource(0, pVB->m_pVB, 0, (UINT) pVB->m_sizeVertex));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetIndices(pIB->m_pIB));

			if (pMat->FlagEnabled(Gae::Graphics::Material::M_ALPHATEST))
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

				m_pD3dDev->SetRenderState(D3DRS_ALPHAREF, (DWORD) pMat->m_fAlphaTest);
				m_pD3dDev->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			}
			else
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			}

			float fDepthBias = -0.0000152587890625f;
			float fSlopeBias = pMat->GetPolygonOffset() * 1.0f;
			m_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, *(DWORD*)&fDepthBias);
			m_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD*)&fSlopeBias);
			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

			/*
			if(pMat->GetMapDiffuse() && (!pMat->GetMapSpecular() && !pMat->GetMapNormal()))
			//if(pMat->GetMapDiffuse())
			{
				m_pD3dDev->SetTexture(0, (LPDIRECT3DTEXTURE9)pMat->GetMapDiffuse()->m_pTexture);
				m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pMat->GetBlendSrc());
				m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pMat->GetBlendDst());
				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));
			}
			*/

			for each(const Gae::Graphics::Material::BlendStage* pBlendStage in pMat->GetVecBlendStage())
			{
				m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, pBlendStage->m_dwMaskComponent);

				m_pD3dDev->SetRenderState(D3DRS_SRCBLEND, pBlendStage->m_dwBlendSrc);
				m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, pBlendStage->m_dwBlendDst);
				m_pD3dDev->SetTexture(0,  (LPDIRECT3DTEXTURE9)pBlendStage->m_pTexture->m_pTexture);

				GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, pVB->m_uVertexCount, 0, pIB->m_uIndexCount / 3));
			}
 
			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, 0);
			m_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
			m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
		}

		void DirectX9::Render(const Driver::VecRenderList& deqRenderList)
		{
			// Lets render!
			for(VecRenderList::const_iterator it = deqRenderList.begin(), itEnd = deqRenderList.end(); it != itEnd; ++it)
			{
				const Gae::Graphics::Mesh* pMesh = *it;
				Render(pMesh);
			}
		}

		void DirectX9::RenderLight(const Gae::Entities::Light* pLight)
		{
			VecRenderList vecNonSolid;
			// TODO - OJO! Las luces también tienen stages. Modificar el loop de acuerdo al devsite de id.
			{	// Create a mask with the affected pixels by the light volume in the stencil buffer.
				// Set the scissor rect to the projected light's AABB
				m_pD3dDev->SetScissorRect(pLight->GetAABBRect());

				// Clear only the affected screen rect of the light volume from the stencil buffer.
				m_pD3dDev->Clear(1, (D3DRECT*)pLight->GetAABBRect(), D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0,128), 1.0f, 0L );

				m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);	// Deactivate writing of the four colour channels to the color buffer.

				// After rendering a mesh the ALPHABLEND and ALPHATEST is left in an undetermined
				// state (see Render(Mesh*, bool)). So for minimizing RenderState changes we explicitly
				// set the correct values here instead each time after rendering the mesh.
				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

				// Configure the stencil operations for marking the areas affected by the light volume.
				// \note The improvement is nearly unnoticeable. The light volume needs to have A LOT of
				// polygons in order to see the performance slightly increased. With the current one is
				// nearly the same using two-sided stencil and double pass.
				m_pD3dDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
				m_pD3dDev->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
				m_pD3dDev->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_INCR);	// Back faces increment stencil value.
				m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);	// Front faces decrement stencil value.
				pLight->DrawVolume(this);

				// Leave the render states ready for the next step.
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// Cull Back-faces again.
				m_pD3dDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);	// Only draw the pixel where the light volume has influence.
				m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
				m_pD3dDev->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
				m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);	// Deactivate the writting of the four colour channels to the color buffer.
			}

			// Now with the light volume mask in the stencil draw the surfaces that are affected by the light.
			{
				// Configure the vertex shader view projection common param and get the handlers to the other ones.
				SelectShader(SL_POINT_BUMP);
				VtxShader* pVtxShader = GetShaderVertex(SL_POINT_BUMP);
				PxlShader* pPxlShader = GetShaderPixel(SL_POINT_BUMP);

				D3DXHANDLE hViewProj = pVtxShader->pConstants->GetConstantByName(NULL, "m44fViewProj");
				D3DXHANDLE hLight = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Light");
				D3DXHANDLE hLightRadius = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Radius");
				D3DXHANDLE hLightColor = pPxlShader->pConstants->GetConstantByName(NULL, "g_f4LightColor");

				Matrix4f equalViewProj = m_mtx44fProj * m_mtx44fView;
				// TODO - Investigate why the next line doesn't work.
				//GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&m_mtx44fViewProj));
				GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&equalViewProj));

				{	// Shaders configuration.
					{	// Vertex shader
						// TODO - This should be correct as light source but it doesn't work. Investigate why the getworldposition direct works fine...
						//Vector3f fLight(pLight->GetOrigin() + pLight->GetCenter());
						Vector3f fLight(pLight->GetWorldPositionDirect());
						Vector3f f3LightingRadius(pLight->GetRadius());

						GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLight, reinterpret_cast<const float*>(fLight.data()), 3));
						GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLightRadius, reinterpret_cast<const float*>(f3LightingRadius.data()), 3));
					}

					{	// Pixel shader
						Vector3f v3fColor(pLight->GetColor());
						Vector4f f4LightColor(v3fColor.x(), v3fColor.y(), v3fColor.z(), 1.0f); 

						GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetFloatArray(m_pD3dDev, hLightColor, reinterpret_cast<const float*>(f4LightColor.data()), 4));
					}
				}

				// Render now the affected surfaces.
				RECT rectLight = *pLight->GetAABBRect();
#ifdef _DEBUG_DRAW
				if(Gae::Core::Universe::Manager::Singleton().VisualDebugEnabled(VD_LIGHTVOL))
				{
					(const_cast<Entities::Light*>(pLight))->SetDrawDebug(false);
				}
#endif					
				for each(const Gae::Graphics::Mesh* pMesh in pLight->GetMeshes())
				{
					RECT& rectToMod = (const_cast<Entities::Light*>(pLight))->GetAABBRectForModification();

					for each(const RECT& rect in pMesh->GetVecRect())
					{
						rectToMod = rectLight;
						if(!Gae::Graphics::Geometry::ClipRectAgainstRect(rect, rectToMod)) continue;

						m_pD3dDev->SetScissorRect(&rectToMod);
#ifdef _DEBUG_DRAW
						if(Gae::Core::Universe::Manager::Singleton().VisualDebugEnabled(VD_LIGHTVOL))
						{
							if(!pLight->IsDrawDebug())
							{
								(const_cast<Entities::Light*>(pLight))->SetDrawDebug(true);
							}
						}
#endif
						Render(pMesh, false);
						
						if(std::find(vecNonSolid.begin(), vecNonSolid.end(), pMesh) == vecNonSolid.end())
						{
							vecNonSolid.push_back(pMesh);
						}
						/*
						if(pMesh->GetMaterial() && pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
						{
							//Render(pMesh, false);
						}
						else
						{
							if(std::find(vecNonSolid.begin(), vecNonSolid.end(), pMesh) == vecNonSolid.end())
							{
								vecNonSolid.push_back(pMesh);
							}
						}
						*/

						/*
						if(pMesh->GetMaterial() && !pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
						{
							if(std::find(vecNonSolid.begin(), vecNonSolid.end(), pMesh) == vecNonSolid.end())
							{
								vecNonSolid.push_back(pMesh);
							}
						}
						*/
					}
					rectToMod = rectLight;	// Restore the original light volume's screen rect.
				}
			}
		}

		void DirectX9::RenderWithLights()
		{
			bool bFlashLight = false;
			{	// 1st. Render geometry in black and updates Depth buffer.
				if(!::GetAsyncKeyState('F'))
				{
					SelectShader(SL_AMBIENT); // este shader es realmente un diffuse shader para que al pintar los alphatest texture
					//en el primer paso éstos hagan la transparencia correctamente.
					//	Cambiar este shader por un Ambient auténtico para dibujar la base más negra que el sobaco de un grillo.
					//	A la hora de dibujar un alphatested mesh activar el zwrites a partir del 2º paso.

					VtxShader* pVtxShader = GetShaderVertex(SL_AMBIENT);
					PxlShader* pPxlShader = GetShaderPixel(SL_AMBIENT);

					D3DXHANDLE hViewProj = pVtxShader->pConstants->GetConstantByName(NULL, "m44fViewProj");
					D3DXHANDLE hColor = pPxlShader->pConstants->GetConstantByName(NULL, "g_f4Color");
					// TODO: Color can be really only a 8bits value? mmmmmmh investigate this
					Vector4f f4Color(Vector4f::Zero());
					//f4Color(3) = 1.0f;

					GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&m_mtx44fViewProj));
					GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetFloatArray(m_pD3dDev, hColor,reinterpret_cast<const float*>(f4Color.data()), 4));
				}
				else
				{
					// Configure the vertex shader common view projection param and get the handlers to the other ones.
					bFlashLight = true;
					SelectShader(SL_POINT_BUMP);
					VtxShader* pVtxShader = GetShaderVertex(SL_POINT_BUMP);
					PxlShader* pPxlShader = GetShaderPixel(SL_POINT_BUMP);

					D3DXHANDLE hViewProj = pVtxShader->pConstants->GetConstantByName(NULL, "m44fViewProj");
					D3DXHANDLE hLight = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Light");
					D3DXHANDLE hLightRadius = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Radius");
					D3DXHANDLE hLightColor = pPxlShader->pConstants->GetConstantByName(NULL, "g_f4LightColor");

					Matrix4f equalViewProj = m_mtx44fProj * m_mtx44fView;
					//GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&m_mtx44fViewProj));
					GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&equalViewProj));

					{	// Shaders configuration.
						{	// Vertex shader
							// TODO Maybe it is better to invert it in the sahder?
							const Matrix4f& m44fCam = m_mtx44fView.inverse();
							Vector3f fLight(m44fCam.block(0,3,3,1));
							static const float fLightRadius = 200.0f;
							Vector3f f3LightingRadius(fLightRadius, fLightRadius, fLightRadius);
// TODO mira a ver el model que da error
							GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLight, reinterpret_cast<const float*>(fLight.data()), 3));
							GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLightRadius, reinterpret_cast<const float*>(f3LightingRadius.data()), 3));
						}

						{	// Pixel shader
							Vector3f v3fColor(1.0f, 1.0f, 1.0f);
							Vector4f f4LightColor(v3fColor.x(), v3fColor.y(), v3fColor.z(), 0.0f);

							GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetFloatArray(m_pD3dDev, hLightColor, reinterpret_cast<const float*>(f4LightColor.data()), 4));
						}
					}
				}

				{
					// Updates Z-Buffer only and set a black base in the frame buffer.
					m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

					//m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
					Render(m_DeqRenderStatic);
					//m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					//m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
					//Render(m_DeqRenderStaticTranslucent);
					//m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
					//m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

					// We do not need to write anymore to the depth buffer.
					// Starting from this point the whole render functions will blend the fragments.
					m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
				}
			}

			// 2nd - For each light:
			//		Clear stencil buffer.
			//		Draw light volume into the stencil buffer with the stencil shadow technique. (Front faces, cull faces using z-fail test)
			//		Use the mask to draw pixels only in the area affected by the stencil mask
			
			// Configure stencil buffer's render states.
			m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);		// Activate Stencil Buffer.
			m_pD3dDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			m_pD3dDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);
			m_pD3dDev->SetRenderState(D3DRS_STENCILREF, 0x0);

			m_pD3dDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
			m_pD3dDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

			m_pD3dDev->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);

			VecRenderList vecNonSolid;
			// TODO - OJO! Las luces también tienen stages. Modificar el loop de acuerdo al devsite de id.
			for each(const Gae::Entities::Light* pLight in m_DeqRenderLight)
			{
				{	// Create a mask with the affected pixels by the light volume in the stencil buffer.
					// Set the scissor rect to the projected light's AABB
					m_pD3dDev->SetScissorRect(pLight->GetAABBRect());

					// Clear only the affected screen rect of the light volume from the stencil buffer.
					m_pD3dDev->Clear(1, (D3DRECT*)pLight->GetAABBRect(), D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0,128), 1.0f, 0L );

					m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);	// Deactivate writing of the four colour channels to the color buffer.

					// After rendering a mesh the ALPHABLEND and ALPHATEST is left in an undetermined
					// state (see Render(Mesh*, bool)). So for minimizing RenderState changes we explicitly
					// set the correct values here instead each time after rendering the mesh.
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
					m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

					// Configure the stencil operations for marking the areas affected by the light volume.
					// \note The improvement is nearly unnoticeable. The light volume needs to have A LOT of
					// polygons in order to see the performance slightly increased. With the current one is
					// nearly the same using two-sided stencil and double pass.
					m_pD3dDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
					m_pD3dDev->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, TRUE);
					m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
					m_pD3dDev->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_INCR);	// Back faces increment stencil value.
					m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);	// Front faces decrement stencil value.
					pLight->DrawVolume(this);

					// Leave the render states ready for the next step.
					m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// Cull Back-faces again.
					m_pD3dDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);	// Only draw the pixel where the light volume has influence.
					m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
					m_pD3dDev->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
					m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);	// Deactivate the writting of the four colour channels to the color buffer.
				}

				// Now with the light volume mask in the stencil draw the surfaces that are affected by the light.
				{
					// Configure the vertex shader view projection common param and get the handlers to the other ones.
					SelectShader(SL_POINT_BUMP);
					VtxShader* pVtxShader = GetShaderVertex(SL_POINT_BUMP);
					PxlShader* pPxlShader = GetShaderPixel(SL_POINT_BUMP);

					D3DXHANDLE hViewProj = pVtxShader->pConstants->GetConstantByName(NULL, "m44fViewProj");
					D3DXHANDLE hLight = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3LightPos");
					D3DXHANDLE hLightRadius = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Radius");
					D3DXHANDLE hViewPos = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3ViewPos");
					D3DXHANDLE hLightColor = pPxlShader->pConstants->GetConstantByName(NULL, "g_f4LightColor");
					D3DXHANDLE hUseParallax = pPxlShader->pConstants->GetConstantByName(NULL, "g_bUseParallax");
					D3DXHANDLE hShowHeight = pPxlShader->pConstants->GetConstantByName(NULL, "g_bShowHeight");
					D3DXHANDLE hShowSpecular = pPxlShader->pConstants->GetConstantByName(NULL, "g_bShowSpecular");

					Matrix4f equalViewProj = m_mtx44fProj * m_mtx44fView;
					// TODO - Investigate why the next line doesn't work.
					//GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&m_mtx44fViewProj));
					GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&equalViewProj));

					{	// Shaders configuration.
						{	// Vertex shader
							// TODO - This should be correct as light source but it doesn't work. Investigate why the getworldposition direct works fine...
							//Vector3f fLight(pLight->GetOrigin() + pLight->GetCenter());
							Vector3f fLight(pLight->GetWorldPositionDirect());
							Vector3f f3LightingRadius(pLight->GetRadius());
							Vector3f f3ViewPos(m_mtx44fView.inverse().block(0,3,3,1));

							GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLight, reinterpret_cast<const float*>(fLight.data()), 3));
							GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLightRadius, reinterpret_cast<const float*>(f3LightingRadius.data()), 3));
							GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hViewPos, reinterpret_cast<const float*>(f3ViewPos.data()), 3));
						}

						{	// Pixel shader
							Vector3f v3fColor(pLight->GetColor());
							Vector4f f4LightColor(v3fColor.x(), v3fColor.y(), v3fColor.z(), 1.0f); 

							GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetFloatArray(m_pD3dDev, hLightColor, reinterpret_cast<const float*>(f4LightColor.data()), 4));

							/*
							static bool bUseParallax = true;
							if(::GetAsyncKeyState('V'))
							{
								bUseParallax = !bUseParallax;
								while(::GetAsyncKeyState('V'));
							}
							GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetBool(m_pD3dDev, hUseParallax, bUseParallax));
							*/

							{
								static bool bShowHeight = false;
								if(::GetAsyncKeyState('C'))
								{
									bShowHeight = !bShowHeight;
									while(::GetAsyncKeyState('C'));
								}
								GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetBool(m_pD3dDev, hShowHeight, bShowHeight));
							}
							{
								static bool bShowSpec = false;
								if(::GetAsyncKeyState('X'))
								{
									bShowSpec = !bShowSpec;
									while(::GetAsyncKeyState('X'));
								}
								GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetBool(m_pD3dDev, hShowSpecular, bShowSpec));
							}
						}
					}

					// Render now the affected surfaces.
					RECT rectLight = *pLight->GetAABBRect();
#ifdef _DEBUG_DRAW
					if(Gae::Core::Universe::Manager::Singleton().VisualDebugEnabled(VD_LIGHTVOL))
					{
						(const_cast<Entities::Light*>(pLight))->SetDrawDebug(false);
					}
#endif					
					for each(const Gae::Graphics::Mesh* pMesh in pLight->GetMeshes())
					{
						RECT& rectToMod = (const_cast<Entities::Light*>(pLight))->GetAABBRectForModification();

						for each(const RECT& rect in pMesh->GetVecRect())
						{
							rectToMod = rectLight;
							if(!Gae::Graphics::Geometry::ClipRectAgainstRect(rect, rectToMod)) continue;
	
							m_pD3dDev->SetScissorRect(&rectToMod);
#ifdef _DEBUG_DRAW
							if(Gae::Core::Universe::Manager::Singleton().VisualDebugEnabled(VD_LIGHTVOL))
							{
								if(!pLight->IsDrawDebug())
								{
									(const_cast<Entities::Light*>(pLight))->SetDrawDebug(true);
								}
							}
#endif
							Render(pMesh, false);	// Draw the mesh with normal mapping.
							
							if(std::find(vecNonSolid.begin(), vecNonSolid.end(), pMesh) == vecNonSolid.end())
							{
								vecNonSolid.push_back(pMesh);
							}
							/*
							if(pMesh->GetMaterial() && pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
							{
								//Render(pMesh, false);
							}
							else
							{
								if(std::find(vecNonSolid.begin(), vecNonSolid.end(), pMesh) == vecNonSolid.end())
								{
									vecNonSolid.push_back(pMesh);
								}
							}
							*/

							/*
							if(pMesh->GetMaterial() && !pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
							{
								if(std::find(vecNonSolid.begin(), vecNonSolid.end(), pMesh) == vecNonSolid.end())
								{
									vecNonSolid.push_back(pMesh);
								}
							}
							*/
						}
						rectToMod = rectLight;	// Restore the original light volume's screen rect.
					}
				}
			}

			
			//m_pD3dDev->SetPixelShader(SL_DIFFUSE);
			SelectShader(SL_DIFFUSE);
			m_pD3dDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

			/*
			if(!::GetAsyncKeyState('B'))
			{
				for each(const Gae::Entities::Light* pLight in m_DeqRenderLight)
				{
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
					for each(const Gae::Graphics::Mesh* pMesh in pLight->GetMeshes())
					{
						RenderBlendStages(pMesh);
					}
					m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
					m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
				}
			}
			*/

			if(!::GetAsyncKeyState('B'))
			{
				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				for each(const Gae::Graphics::Mesh* pMesh in vecNonSolid)
				{
					RenderBlendStages(pMesh);
				}
				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
			}

			/*
			if(!::GetAsyncKeyState('N'))
			{
				for each(const Gae::Graphics::Mesh* pMesh in m_DeqRenderStaticDecal)
				{
					RenderBlendStages(pMesh);
				}
			}

			if(!::GetAsyncKeyState('M'))
			{
				for each(const Gae::Graphics::Mesh* pMesh in m_DeqRenderStaticTranslucent)
				{
					RenderBlendStages(pMesh);
				}
			}
			*/

			// 3rd. Render the rest of material's blend. Decals and blend stages. 
			//m_pD3dDev->SetVertexShader(NULL);
			/*
			if(!::GetAsyncKeyState('O'))
			{
				if(::GetAsyncKeyState('P'))
				{
					m_pD3dDev->SetPixelShader(NULL);
					m_pD3dDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
					m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

					m_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
					m_pD3dDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);

					m_pD3dDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
					m_pD3dDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

					m_pD3dDev->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
					m_pD3dDev->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);

					for each(const Gae::Graphics::Mesh* pMesh in vecNonSolid)
					{
						Render(pMesh, false);
					}
				}
				else
				{
					m_pD3dDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
					m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);

					m_pD3dDev->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_SELECTARG1);
					m_pD3dDev->SetTextureStageState(0,D3DTSS_COLORARG1,D3DTA_TEXTURE);

					m_pD3dDev->SetTextureStageState(0,D3DTSS_ALPHAOP,D3DTOP_SELECTARG1);
					m_pD3dDev->SetTextureStageState(0,D3DTSS_ALPHAARG1,D3DTA_TEXTURE);

					m_pD3dDev->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_DISABLE);
					m_pD3dDev->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);

					for each(const Gae::Entities::Light* pLight in m_DeqRenderLight)
					{
						// Configure the vertex shader view projection common param and get the handlers to the other ones.
						SelectShader(SL_POINT_BUMP);
						VtxShader* pVtxShader = GetShaderVertex(SL_POINT_BUMP);
						PxlShader* pPxlShader = GetShaderPixel(SL_POINT_BUMP);

						D3DXHANDLE hViewProj = pVtxShader->pConstants->GetConstantByName(NULL, "m44fViewProj");
						D3DXHANDLE hLight = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Light");
						D3DXHANDLE hLightRadius = pVtxShader->pConstants->GetConstantByName(NULL, "g_f3Radius");
						D3DXHANDLE hLightColor = pPxlShader->pConstants->GetConstantByName(NULL, "g_f4LightColor");

						GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&m_mtx44fViewProj));

						{	// Shaders configuration.
							{	// Vertex shader
								Vector3f fLight(pLight->GetOrigin() + pLight->GetCenter());
								Vector3f f3LightingRadius(pLight->GetRadius());

								GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLight, reinterpret_cast<const float*>(fLight.data()), 3));
								GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetFloatArray(m_pD3dDev, hLightRadius, reinterpret_cast<const float*>(f3LightingRadius.data()), 3));
							}

							{	// Pixel shader
								Vector3f v3fColor(pLight->GetColor());
								Vector4f f4LightColor(v3fColor.x(), v3fColor.y(), v3fColor.z(), 0.0f);

								GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetFloatArray(m_pD3dDev, hLightColor, reinterpret_cast<const float*>(f4LightColor.data()), 4));
							}
						}

						for each(const Gae::Graphics::Mesh* pMesh in pLight->GetMeshes())
						{
							//if(pMesh->GetMaterial() && !pMesh->GetMaterial()->FlagEnabled(Material::M_SOLID))
							if(pMesh->GetMaterial() && pMesh->GetMaterial()->FlagEnabled(Material::M_DECAL | Material::M_BLEND))
							{
								Render(pMesh, false);
							}
						}
					}
				}
			}
			*/

			m_pD3dDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);		// Deactivate Stencil Buffer.
		}


		void DirectX9::RenderGeometry()
		{
			RenderWithLights();
			//RenderOnlyDiffuse();
		}

		void DirectX9::Render()
		{
			if(m_bRenderWithShadows)
			{
				RenderWithShadows();
				return;
			}

			// Clear Z and stencil (for the shadow volumes) buffer
			m_pD3dDev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0,0,128), 1.0f, 0L );

			D3DXMATRIX MatrixWorld;
			D3DXMatrixIdentity(&MatrixWorld);
			m_pD3dDev->BeginScene();
			{
				m_pD3dDev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)(&m_mtx44fView));
				m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)(&m_mtx44fProj));
				m_pD3dDev->SetTransform(D3DTS_WORLD, &MatrixWorld);

				RenderGeometry();
			}
			m_pD3dDev->EndScene();

#ifdef _DEBUG_DRAW
			m_pD3dDev->SetTexture(0, NULL);
			m_pD3dDev->SetTexture(1, NULL);
			m_pD3dDev->SetTexture(2, NULL);
			m_pD3dDev->SetTexture(3, NULL);

			m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			// This pointers are used to backup the currently set pixel and vertex shader.
			IDirect3DVertexShader9* pVtxShader = NULL;
			IDirect3DPixelShader9* pPxlShader = NULL;

			m_pD3dDev->GetPixelShader(&pPxlShader);
			m_pD3dDev->GetVertexShader(&pVtxShader);

			super::DrawDebug();

			if(!m_bWireframe)
			{
				m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			}
			else
			{
				m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			//m_pD3dDev->SetPixelShader(pPxlShader);
			//m_pD3dDev->SetVertexShader(pVtxShader);
#endif

			m_pD3dDev->Present( NULL, NULL, NULL, NULL );

#ifdef _DEBUG_DRAW
			m_VecDrawDebug.clear();
#endif
		}

		void DirectX9::RenderWithShadowsFillZBuffer()
		{	// Render to Z-Buffer and colour buffer.
			//m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);	// Deactivate writing of the four colour channels to the color buffer.

			m_pD3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		// Activate Z-Buffer test.
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE );	// Active writing to the Z-Buffer if Z-Buffer test is success.
			m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE );// Deactivate writing to the Stencil Buffer.

			// Render the scene with the bump mapping shader. This fills the buffer color and the Z-Buffer.
			m_eLighting = SL_POINT_LIGHT;
			KeyPressed(DIK_4);

			RenderGeometry();

			m_eLighting = SL_NONE;
			m_pD3dDev->SetVertexShader(NULL);
			m_pD3dDev->SetPixelShader(NULL);
		}

		void DirectX9::RenderWithShadowsCreateStencilBuffer()
		{	// Lets create the shadow mask in the Stencil Buffer.
			m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, 0);	// Deactivate writing of the four colour channels to the color buffer.

			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Deactivate Z-Buffer writing.
			m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);		// Activate Stencil Buffer.

			m_pD3dDev->SetRenderState(D3DRS_STENCILREF, 0x0);
			m_pD3dDev->SetRenderState(D3DRS_STENCILMASK, 0xffffffff);
			m_pD3dDev->SetRenderState(D3DRS_STENCILWRITEMASK, 0xffffffff);

			m_pD3dDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
			m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);	// If Z-Test fails keep the value. Do nothing.
			m_pD3dDev->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);	// If Stencil's test fails keep the value. Do nothing.
			m_pD3dDev->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);	// Increment Stencil Buffer value if Z-Test passes.

			/*
			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_DISABLE);
			m_pD3dDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			m_pD3dDev->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
			*/


			/*
			if(GetAsyncKeyState('R') & 0x8000)
			{
				*/
				float fZero = 0.0f;
				static float fDepthBias = 0.0f;
				static const float fStep = 0.000000059604644775390625f;

				if(GetAsyncKeyState('T') & 0x8000)
				{
					fDepthBias += fStep;
				}

				if(GetAsyncKeyState('Y') & 0x8000)
				{
					fDepthBias -= fStep;
				}

				DWORD dwSlopeBias = *((DWORD*)&fZero);
				DWORD dwDepthBias = *((DWORD*)&fDepthBias);
				m_pD3dDev->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, dwSlopeBias);
				m_pD3dDev->SetRenderState(D3DRS_DEPTHBIAS, dwDepthBias);

				GAE_TRACE(L"DepthBias: [%f]\tSlope: [%f]\n", fDepthBias, fZero);

				/*
			}
			*/

			// Configures the infinite projection matrix.
			Matrix4f m44fInfiniteProj = m_mtx44fProj;
			// Infinite perspective matrix
			m_mtx44fProj(2,2) = 1.0f - 0.000001f;//fFar / (fNear - fFar);
			//m_mtx44fProj(2,2) *= -1.f - (4.f / powf(2.f,24));
			/*
			m_mtx44fProj(2,2) = 1.0f;
			m_mtx44fProj(2,3) = -1.0f;
			m_mtx44fProj(3,2) = 1.0f;
			*/

			//m_mtx44fProj(1,1) = 1.0f / fTan;	// Cotan(fFOV / 2.0f)
			//m_mtx44fProj(0,0) = m_mtx44fProj(1,1) / fAspectRatio;			
			//m_mtx44fProj(2,2) = fFar / (fFar - fNear);
			//m_mtx44fProj(2,3) = -((fFar * fNear) / (fFar - fNear));
			//m_mtx44fProj(3,2) = 1.0f;
			
			
			m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&m44fInfiniteProj);

			// Process front faces of shadow volumes.
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);				// Cull back-faces.
			m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR);	// If Z-Test fails keep the value. Do nothing.
			Render(m_DeqRenderShadowStatic);	// Render the front faces of the shadow volumes

			// Process back faces of shadow volumes.
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);				// Cull front-faces.
			m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);	// If Z-Test fails keep the value. Do nothing.
			Render(m_DeqRenderShadowStatic);	// Render the front faces of the shadow volumes

			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Activate Z-Buffer writing.
			m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);		// Deactivate writing to the Stencil Buffer.
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// Cull Back-faces.
			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

			{
				float fZero = 0.0f;
				DWORD dwSlopeBias = *((DWORD*)&fZero);
				DWORD dwDepthBias = *((DWORD*)&fZero);
				m_pD3dDev->SetRenderState (D3DRS_SLOPESCALEDEPTHBIAS, dwSlopeBias);
				m_pD3dDev->SetRenderState (D3DRS_DEPTHBIAS, dwDepthBias);

				m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)&m_mtx44fProj);
			}
		}

		void DirectX9::RenderWithShadows()
		{
			D3DXMATRIX MatrixWorld;
			D3DXMatrixIdentity(&MatrixWorld);

			// Clear Z and stencil (for the shadow volumes) buffer
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL, D3DCOLOR_XRGB(255,255,255), 1.0f, 0L));

			m_pD3dDev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)(&m_mtx44fView));
			m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)(&m_mtx44fProj));
			m_pD3dDev->SetTransform(D3DTS_WORLD, &MatrixWorld);

			m_pD3dDev->BeginScene();
			{
				RenderWithShadowsFillZBuffer();
				RenderWithShadowsCreateStencilBuffer();
				//Render(m_DeqRenderShadowStatic);
			}

			{	// Render the scene now with the Stencil test enabled.
				//m_pD3dDev->Clear( 0, NULL, D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0L );	// Clear Z-buffer to avoid Z-Fighting
				m_pD3dDev->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);	// Deactivate the writting of the four colour channels to the color buffer.
				/*
				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE );
				m_pD3dDev->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
				m_pD3dDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
				*/

				m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);// Activate Z-Buffer writing.
				m_pD3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);		// Activate Z-Buffer test.

				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);	// Do not overwrite the buffer colour, just add the value.
				m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, TRUE);		// Activate Stencil Buffer test.

				m_pD3dDev->SetRenderState(D3DRS_STENCILREF,  0x0);
				m_pD3dDev->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NOTEQUAL);

				// Draw a big, gray square
				m_pD3dDev->SetFVF( ShadowVertex::FVF_Flags );
				m_pD3dDev->SetStreamSource( 0, m_pBigSquareVB, 0, sizeof(ShadowVertex) );
				m_pD3dDev->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

				m_pD3dDev->SetRenderState(D3DRS_STENCILENABLE, FALSE);		// Deactivate Stencil Buffer.
				m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);		// Cull back-faces.
				m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);	// Do not overwrite the buffer colour, just add the value.
				m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			}
			m_pD3dDev->EndScene();

#ifdef _DEBUG_DRAW
			m_pD3dDev->SetTexture(0, NULL);
			m_pD3dDev->SetTexture(1, NULL);
			m_pD3dDev->SetTexture(2, NULL);
			m_pD3dDev->SetTexture(3, NULL);
			super::DrawDebug();
#endif

			m_pD3dDev->Present( NULL, NULL, NULL, NULL );

#ifdef _DEBUG_DRAW
			m_VecDrawDebug.clear();
#endif
		}

		// START - Draw Debug Methods
		//GAE_DEBUG_FN_IMPL(
#ifdef _DEBUG_DRAW
		void DirectX9::DrawDebug(const E_DEBUG_SHAPE eShape, const Gae::Graphics::Node* pNode, const DWORD dwColor) const
		{
			static ID3DXMesh* pMeshSphere = NULL;
			static ID3DXMesh* pMeshBox = NULL;
			/*
			static VtxShader ShaderVertex;
			static PxlShader ShaderPixel;
			*/

			if(pMeshSphere == NULL)
			{
				GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateSphere(m_pD3dDev, 1.0f, 32, 32, &pMeshSphere, NULL));
				GAE_ASSERT_D3D_FN(D3D_OK, D3DXCreateBox(m_pD3dDev, 1.0f, 1.0f, 1.0f, &pMeshBox, NULL));
			}

			D3DXMATRIX matWorld;
			D3DXMATRIX matRot;
			D3DXMatrixIdentity(&matWorld);
			D3DXMatrixIdentity(&matRot);
			{	// Entity World matrix calculation.
				// \TODO Add the scaling inside the node object. Currently is not included in the world matrix.
				const Vector3f& v3fScale = pNode->GetScale();
				//const Vector3f& v3fPos = const_cast<Gae::Graphics::Node*>(pNode)->GetWorldPosition();	// GetWorldPosition can update the node contents if it is dirty. We need a const_cast here.

				D3DXMatrixScaling(&matWorld, v3fScale.x(), v3fScale.y(), v3fScale.z());

				Matrix4f m3fRot = pNode->GetWorldTransformDirect();
				matRot = *(D3DXMATRIX*)(&m3fRot);
				D3DXMatrixMultiply(&matWorld, &matWorld, &matRot);

				/*
				matWorld._41 = v3fPos.x();
				matWorld._42 = v3fPos.y();
				matWorld._43 = v3fPos.z();
				*/
			}
			m_pD3dDev->SetTransform(D3DTS_WORLD, &matWorld);
			m_pD3dDev->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_mtx44fView);
			m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_mtx44fProj);

			m_pD3dDev->SetFVF(pMeshSphere->GetFVF());

			
			{	// Shader's configuration.
				Matrix4f m44fWorldViewProj = m_mtx44fViewProj * (*((Matrix4f*)(&matWorld)));

				SelectShader(SL_AMBIENT);
				{
					VtxShader* pVtxShader = GetShaderVertex(SL_AMBIENT);
					PxlShader* pPxlShader = GetShaderPixel(SL_AMBIENT);
					D3DXHANDLE hViewProj = pVtxShader->pConstants->GetConstantByName(NULL, "m44fViewProj");
					D3DXHANDLE hColor = pPxlShader->pConstants->GetConstantByName(NULL, "g_f4Color");
					Vector4f f4Color(((float)((dwColor & 0x00ff0000) >> 16)) / 256.0f, ((float)((dwColor & 0x0000ff00) >> 8)) / 256.0f, ((float)(dwColor & 0x000000ff)) / 256.0f, (((dwColor & 0xff000000) >> 24)) / 256.0f);

					GAE_ASSERT_D3D_FN(D3D_OK, pVtxShader->pConstants->SetMatrix(m_pD3dDev, hViewProj, (D3DXMATRIX*)&m44fWorldViewProj));
					GAE_ASSERT_D3D_FN(D3D_OK, pPxlShader->pConstants->SetFloatArray(m_pD3dDev, hColor,reinterpret_cast<const float*>(f4Color.data()), 4));
				}
			}

			switch(eShape)
			{
			case DS_SPHERE:
				pMeshSphere->DrawSubset(0);
				break;
			case DS_BOX:
				pMeshBox->DrawSubset(0);
				break;
			default:
				GAE_ASSERTM(false, L"Unsupported debug shape!");
				break;
			}
		}
#endif
		//)

#ifdef _DEBUG_DRAW
		struct TLVERTEX
		{
			float x, y, z, w;
			DWORD uiRGBA;
		};
#endif
		GAE_DEBUG_FN_IMPL(
		void DirectX9::DrawDebugRect(const RECT* pRect, const DWORD dwColor) const
		{
			DWORD dwWidth = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth;
			DWORD dwHeight = Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight;
			GAE_ASSERTM(pRect, L"Invalid RECT!");
			GAE_ASSERTM(pRect->top >= 0 && pRect->left >= 0 && pRect->right <= dwWidth && pRect->bottom <= dwHeight, L"Invalid RECT!!");

			TLVERTEX aVerts[4];

			aVerts[0].x = float(pRect->left);
			aVerts[0].y = float(pRect->top);

			aVerts[1].x = float(pRect->left);
			aVerts[1].y = float(pRect->bottom);

			aVerts[2].x = float(pRect->right);
			aVerts[2].y = float(pRect->bottom);

			aVerts[3].x = float(pRect->right);
			aVerts[3].y = float(pRect->top);

			int i;

			for (i=0;i<4;i++)
			{
				aVerts[i].uiRGBA = dwColor;
				aVerts[i].z = 0.0f;
				aVerts[i].w = 1.0f;
			}

			/*
			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_LIGHTING, FALSE);

			m_pD3dDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			m_pD3dDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

			m_pD3dDev->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

			m_pD3dDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
			*/
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

			m_pD3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
			m_pD3dDev->SetVertexShader(NULL);  
			m_pD3dDev->SetTexture(0, NULL);
			m_pD3dDev->SetPixelShader(NULL);
			m_pD3dDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);

			m_pD3dDev->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2, aVerts, sizeof(TLVERTEX));

			m_pD3dDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		}
		)
		GAE_DEBUG_FN_IMPL(
		void DirectX9::DrawDebugTriangles(const Vector4f* pv4fVertexes, UINT32 uCount) const
		{
			D3DXMATRIX MatrixWorld;
			D3DXMatrixIdentity(&MatrixWorld);

			IDirect3DVertexShader9* pVtxShader = NULL;
			IDirect3DPixelShader9* pPxlShader = NULL;

			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->GetVertexShader(&pVtxShader));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->GetPixelShader(&pPxlShader));
			m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			m_pD3dDev->SetVertexShader(NULL);
			m_pD3dDev->SetPixelShader(NULL);

			m_pD3dDev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)(&m_mtx44fView));
			m_pD3dDev->SetTransform(D3DTS_WORLD, &MatrixWorld);
			m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)(&m_mtx44fProj));

			m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			m_pD3dDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

			m_pD3dDev->BeginScene();
			{	
				m_pD3dDev->DrawPrimitiveUP(D3DPT_TRIANGLELIST, uCount, (const void*)pv4fVertexes->data(), sizeof(float) * 4);
			}
			m_pD3dDev->EndScene();

			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

			m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexShader(pVtxShader));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetPixelShader(pPxlShader));
		}
		)

		GAE_DEBUG_FN_IMPL(
		void DirectX9::DrawDebugTriangle(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2, const Vector4f* pv4fVertex3) const
		{
			Vector4f v4fVertexes[3];

			v4fVertexes[0] << pv4fVertex1->x(), pv4fVertex1->y(), pv4fVertex1->z(), pv4fVertex1->w();
			v4fVertexes[1] << pv4fVertex2->x(), pv4fVertex2->y(), pv4fVertex2->z(), pv4fVertex2->w();
			v4fVertexes[2] << pv4fVertex3->x(), pv4fVertex3->y(), pv4fVertex3->z(), pv4fVertex3->w();

			DrawDebugTriangles((const Vector4f*)&v4fVertexes, 1);
		}
		)

		GAE_DEBUG_FN_IMPL(
		void DirectX9::DrawDebugLines(const Vector4f* pv4fVertexes, UINT32 uCount) const
		{
			D3DXMATRIX MatrixWorld;
			D3DXMatrixIdentity(&MatrixWorld);

			IDirect3DVertexShader9* pVtxShader = NULL;
			IDirect3DPixelShader9* pPxlShader = NULL;

			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->GetVertexShader(&pVtxShader));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->GetPixelShader(&pPxlShader));

			//m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

			m_pD3dDev->SetVertexShader(NULL);
			m_pD3dDev->SetPixelShader(NULL);

			m_pD3dDev->SetTransform(D3DTS_WORLD, &MatrixWorld);
			m_pD3dDev->SetTransform(D3DTS_VIEW, (D3DMATRIX*)(&m_mtx44fView));
			m_pD3dDev->SetTransform(D3DTS_PROJECTION, (D3DMATRIX*)(&m_mtx44fProj));

			//m_pD3dDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

			m_pD3dDev->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE);

			m_pD3dDev->BeginScene();
			{	
				m_pD3dDev->DrawPrimitiveUP(D3DPT_LINESTRIP, uCount, (const void*)pv4fVertexes->data(), sizeof(float) * 4);
			}
			m_pD3dDev->EndScene();

			m_pD3dDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
			m_pD3dDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			m_pD3dDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			//m_pD3dDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetVertexShader(pVtxShader));
			GAE_ASSERT_D3D_FN(D3D_OK, m_pD3dDev->SetPixelShader(pPxlShader));
		}
		)
		

		GAE_DEBUG_FN_IMPL(
		void DirectX9::DrawDebugLine(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2) const
		{
			Vector4f v4fVertexes[2];

			v4fVertexes[0] << pv4fVertex1->x(), pv4fVertex1->y(), pv4fVertex1->z(), pv4fVertex1->w();
			v4fVertexes[1] << pv4fVertex2->x(), pv4fVertex2->y(), pv4fVertex2->z(), pv4fVertex2->w();

			DrawDebugLines((const Vector4f*)&v4fVertexes, 1);
		}
		)

	}
}