#ifndef Renderer_h__
#define Renderer_h__

namespace Gae
{
	namespace Renderer
	{
		enum E_GAE_RENDERER
		{
			GR_UNDEFINED = 0,
			GR_OPENGL,
			GR_DIRECTX9,
			GR_SOFTWARE
		};

		struct RendererConfig
		{
			enum E_FLAGS
			{
				RCF_FULLSCREEN	= 0x00000001,
				RCF_WINDOWED	= 0x00000002
			};

			E_GAE_RENDERER m_eRenderer;
			HWND m_hWnd;
			DWORD m_uFlags;
			DWORD m_dwWidth;
			DWORD m_dwHeight;
		};
	}
}

#ifdef _DEBUG_DRAW
	#include "Renderer/IDrawDebug.h"
#endif
#include "Renderer/Driver.h"
#include "Renderer/OpenGL.h"
#include "Renderer/DirectX9.h"

#endif // Renderer_h__