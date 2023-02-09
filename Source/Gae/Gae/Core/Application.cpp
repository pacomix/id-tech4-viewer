#include "stdafx.h"
#include "Core/Application.h"

namespace Gae
{
	namespace Core
	{
		
		SINGLETON_IMPL(Application);

		bool Application::Init()
		{
			static bool bFirstInit = TRUE;

			if(!bFirstInit) return FALSE;


			// Change the directory.
			wchar_t pFullPathExecutable[1024];
			GetModuleFileName(NULL, pFullPathExecutable, 1024);
			std::wstring wsExecutablePath = pFullPathExecutable;
			_wchdir(wsExecutablePath.substr(0, wsExecutablePath.find_last_of(L'\\')).c_str());

			m_pAppCfg = new ApplicationConfig;

			Clock::CreateSingleton();
			Clock::Singleton().Init();

			ApplicationWindow::CreateSingleton();
			ApplicationWindow& AppWindow = ApplicationWindow::Singleton();
			//m_pAppCfg->m_RendererConfig.m_hWnd = AppWindow.m_hWnd;

			Input::Manager::CreateSingleton();
			Input::Manager::Singleton().SetMouseSensitiveness(1.0f);

			// Initializes the correct renderer.
			Gae::Renderer::Driver& AppRenderer = *(GetRenderer());

			Gae::Graphics::Geometry::Procedural::CreateSingleton();
			Resources::ResManager::CreateSingleton();

			Universe::Manager::CreateSingleton();

			return TRUE;
		}

		void Application::Run()
		{
			ApplicationWindow& AppWindow = ApplicationWindow::Singleton();
			Gae::Renderer::Driver& AppRenderer = *(GetRenderer());
			Universe::Manager& UniverseManager = Universe::Manager::Singleton();
			Input::Manager& InputManager = Input::Manager::Singleton();
			Clock& MainClock = Clock::Singleton();

			while(!AppWindow.IsFinished())
			{
				if(!AppWindow.Update())	// Manages windows event
				{	// Implement the application loop here
					MainClock.Update();
					InputManager.Update();
					UniverseManager.Update();
					AppRenderer.Render();	// Start the rendering process.
				}
			}
		}

		Gae::Renderer::Driver* Application::GetRenderer()
		{
			static Gae::Renderer::Driver* pDriver = NULL;
			if(pDriver)	return pDriver;

			switch(m_pAppCfg->m_RendererConfig.m_eRenderer)
			{
			case Gae::Renderer::GR_DIRECTX9:
				Gae::Renderer::DirectX9::CreateSingleton();
				return (pDriver = static_cast<Gae::Renderer::Driver*>(&Gae::Renderer::DirectX9::Singleton()));
				break;
			case Gae::Renderer::GR_OPENGL:
				Gae::Renderer::OpenGL::CreateSingleton();
				return (pDriver = static_cast<Gae::Renderer::Driver*>(&Gae::Renderer::OpenGL::Singleton()));
				break;
			default:	// Renderer not implemented.
				return NULL;
				break;
			}
		}

		void Application::DestroyRenderer()
		{
			switch(m_pAppCfg->m_RendererConfig.m_eRenderer)
			{
			case Gae::Renderer::GR_DIRECTX9:
				Gae::Renderer::DirectX9::DestroySingleton();
				break;
			case Gae::Renderer::GR_OPENGL:
				Gae::Renderer::OpenGL::DestroySingleton();
				break;
			default:	// Renderer not implemented.
				break;
			}
		}

		bool Application::Uninit()
		{
			Gae::Core::Universe::Manager::DestroySingleton();
			Gae::Core::Resources::ResManager::DestroySingleton();
			Gae::Graphics::Geometry::Procedural::DestroySingleton();
			DestroyRenderer();
			ApplicationWindow::DestroySingleton();
			GAE_DELETESAFE(m_pAppCfg);

			return TRUE;
		}
	}
}