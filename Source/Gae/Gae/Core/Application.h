#ifndef Application_h__
#define Application_h__

#include "Renderer/Renderer.h"

namespace Gae
{
	namespace Core
	{
		struct ApplicationConfig;

		class Application
		{
			SINGLETON_DECL(Application);

		public:
			void SetConfiguration(ApplicationConfig& appCfg);
			void Run();

			inline const ApplicationConfig* GetApplicationConfig() const { return m_pAppCfg;}

			Gae::Renderer::Driver* GetRenderer();

		private:
			void DestroyRenderer();

			Gae::Renderer::E_GAE_RENDERER m_eRenderer;

			ApplicationConfig* m_pAppCfg;
		};
	}
}

#include "Application.inl"

#endif // Application_h__