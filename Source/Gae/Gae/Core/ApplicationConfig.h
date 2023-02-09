#ifndef ApplicationConfig_h__
#define ApplicationConfig_h__

#include "Renderer/Renderer.h"

namespace Gae
{
	namespace Core
	{
		struct ApplicationConfig
		{
			static HINSTANCE m_hAppInstance;
			static Gae::Renderer::RendererConfig m_RendererConfig;
		};
	}
}

#endif // ApplicationConfig_h__