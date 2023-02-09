#include "stdafx.h"
#include "OpenGL.h"

namespace Gae
{
	namespace Renderer
	{
		SINGLETON_IMPL(OpenGL);

		bool OpenGL::Init()
		{
			static bool bFirstInit = TRUE;
			if(!bFirstInit) return FALSE;
			super::Init();

			m_hDevContext = GetDC(m_pRendererCfg->m_hWnd);

			PIXELFORMATDESCRIPTOR pfd;
			ZeroMemory(&pfd, sizeof(pfd));

			pfd.nSize = sizeof(pfd);
			pfd.nVersion = 1;
			pfd.dwFlags = 
				PFD_SUPPORT_OPENGL		| 
				PFD_DRAW_TO_WINDOW		|
				PFD_DOUBLEBUFFER		|
				PFD_GENERIC_ACCELERATED;
				
			pfd.iPixelType = PFD_TYPE_RGBA;
			pfd.cColorBits = 24;
			pfd.cDepthBits = 16;
			pfd.iLayerType = PFD_MAIN_PLANE;
			int iFormat = ChoosePixelFormat(m_hDevContext, &pfd);
			SetPixelFormat(m_hDevContext, iFormat, &pfd);

			m_hRenderContext = wglCreateContext(m_hDevContext);
			wglMakeCurrent(m_hDevContext, m_hRenderContext);

			bFirstInit = FALSE;
			return TRUE;
		}

		GAE_DEBUG_FN_IMPL(
		void OpenGL::DrawDebug(const E_DEBUG_SHAPE eShape, const Gae::Graphics::Node* pNode, const DWORD dwColor) const
		{
			GAE_ASSERTM(false, L"Not implemented yet!");
		}
		)

		bool OpenGL::InitMesh(Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERTM(FALSE, L"Not implemented yet!");
			return FALSE;
		}

		bool OpenGL::UninitMesh(Gae::Graphics::Mesh* pMesh)
		{
			GAE_ASSERTM(FALSE, L"Not implemented yet!");
			return FALSE;
		}

		void OpenGL::SetViewMatrix(const Matrix4f& mtx4fView)
		{
			GAE_ASSERTM(FALSE, L"Not implemented yet!");
		}

		void OpenGL::SetProjectionMatrix(const Matrix4f& mtx4fView)
		{
			GAE_ASSERTM(FALSE, L"Not implemented yet!");
		}

		void OpenGL::Render()
		{
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear( GL_COLOR_BUFFER_BIT );
			SwapBuffers(m_hDevContext);
		}

		bool OpenGL::Uninit()
		{
			wglMakeCurrent(NULL, NULL);
			wglDeleteContext(m_hRenderContext);
			ReleaseDC(m_pRendererCfg->m_hWnd, m_hDevContext);

			return TRUE;
		}
	}
}