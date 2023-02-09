#include "stdafx.h"
#include "Core/ApplicationConfig.h"

namespace Gae
{
	namespace Core
	{
		HINSTANCE ApplicationConfig::m_hAppInstance = NULL;
		Gae::Renderer::RendererConfig ApplicationConfig::m_RendererConfig;
	}
}