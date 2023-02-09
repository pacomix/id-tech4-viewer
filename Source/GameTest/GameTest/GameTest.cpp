// GameTest.cpp : Definiert den Einstiegspunkt für die Anwendung.
//
#include "stdafx.h"

#include "Gae.h"

#include "GameTest.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Create a config file for configuring the engine instead accessing this structure.
	{
		using namespace Gae::Core;
		ApplicationConfig::m_hAppInstance = hInstance;
		ApplicationConfig::m_RendererConfig.m_eRenderer = Gae::Renderer::GR_DIRECTX9;
		ApplicationConfig::m_RendererConfig.m_uFlags = Gae::Renderer::RendererConfig::RCF_WINDOWED;
		ApplicationConfig::m_RendererConfig.m_dwWidth = 1366;
		ApplicationConfig::m_RendererConfig.m_dwHeight = 768;
	}

	Gae::Core::Application::CreateSingleton(); 

	{	// Add the required models to the application's universe.
		Gae::Core::Universe::Manager& universeManager = Gae::Core::Universe::Manager::Singleton();

		using namespace Gae::Gifts::Quake4;
		MapLoader::CreateSingleton();
		MapLoader& mapLoader = MapLoader::Singleton();
		GAE_TRACE(L"command line: [%s]", lpCmdLine);
		// QUAKE4
		//mapLoader.LoadLevel(L"maps/game/airdefense2");
		//mapLoader.LoadLevel(L"maps/mp/q4dm7");
		//mapLoader.LoadLevel(L"maps/game/dispersal");
		//mapLoader.LoadLevel(L"maps/mp/q4dm6");
		//mapLoader.LoadLevel(L"maps/game/network1");
		//mapLoader.LoadLevel(L"maps/game/core1"); 
		//mapLoader.LoadLevel(L"maps/game/process1"); 
		//mapLoader.LoadLevel(L"maps/mp/q4ctf4");
		//mapLoader.LoadLevel(L"maps/mp/q4ctf5");
		//mapLoader.LoadLevel(L"maps/mp/q4dm3");

		std::wstring wsLevelName(lpCmdLine);
		if(wsLevelName.empty())
		{
			//mapLoader.LoadLevel(L"maps/game/building_b");
			//mapLoader.LoadLevel(L"maps/mp/q4ctf1");
			mapLoader.LoadLevel(L"maps/game/alphalabs1");
		}
		else
		{
			mapLoader.LoadLevel(lpCmdLine);
		}

		// DOOM3
		//mapLoader.LoadLevel(L"maps/game/admin");
		//mapLoader.LoadLevel(L"maps/game/hell1");	// <--- It doesn't work
		//mapLoader.LoadLevel(L"maps/game/mc_underground");
		//mapLoader.LoadLevel(L"maps/game/mp/d3dm1");
		
	}

	// Lets the party start
	Gae::Core::Application::Singleton().Run();

	Gae::Gifts::Quake4::MapLoader::DestroySingleton();

	Gae::Core::Application::DestroySingleton();

	return 0;
}
