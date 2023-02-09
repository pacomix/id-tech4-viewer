#include "stdafx.h"
#include "ApplicationWindow.h"



namespace Gae
{
	namespace Core
	{
		SINGLETON_IMPL(ApplicationWindow);

		bool ApplicationWindow::Init()
		{
			static bool bFirstInit = TRUE;
			m_bIsFinished = false;
			const ApplicationConfig* pAppCfg = Application::Singleton().GetApplicationConfig();

			ConfigureFullScreenMode(pAppCfg->m_RendererConfig.m_uFlags & Gae::Renderer::RendererConfig::RCF_FULLSCREEN);

			if(m_hAppInstance && !bFirstInit)	return FALSE;
			m_hAppInstance = pAppCfg->m_hAppInstance;

			m_lpszClassName = L"Genium Application Engine";

			// Initialize the message
			ZeroMemory(&m_msg,sizeof(m_msg));

			// Register the window
			WNDCLASSEX wcex;

			wcex.cbSize = sizeof(WNDCLASSEX);

			wcex.style			= CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc	= &ApplicationWindow::WndProc;
			wcex.cbClsExtra		= 0;
			wcex.cbWndExtra		= 0;
			wcex.hInstance		= this->m_hAppInstance;
			wcex.hIcon			= NULL;//LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESTWIN32));
			wcex.hCursor		= NULL;LoadCursor(NULL, IDC_ARROW);
			wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
			/*wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TESTWIN32);*/
			wcex.lpszMenuName	= 0;
			wcex.lpszClassName	= m_lpszClassName;
			wcex.hIconSm		= NULL;//LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

			if(!RegisterClassEx(&wcex)) return FALSE;

			// Creates the window
			DWORD dwWndStyle;
			DWORD dwWndExStyle;
			DWORD dwPosX;
			DWORD dwPosY;
			if(pAppCfg->m_RendererConfig.m_uFlags & Gae::Renderer::RendererConfig::RCF_WINDOWED)
			{
				dwWndExStyle = WS_EX_APPWINDOW | WS_EX_CLIENTEDGE;
				dwWndStyle = WS_OVERLAPPEDWINDOW;
				dwPosX = 0;
				dwPosY = 0;
			}
			else
			{
				dwWndExStyle = WS_EX_APPWINDOW;
				dwWndStyle = WS_POPUP;
				dwPosX = 0;
				dwPosY = 0;
			}

			m_hWnd = CreateWindowEx(dwWndExStyle, m_lpszClassName, L"Application", dwWndStyle, dwPosX, dwPosY, pAppCfg->m_RendererConfig.m_dwWidth, pAppCfg->m_RendererConfig.m_dwHeight, NULL, NULL, m_hAppInstance, NULL);

			const_cast<ApplicationConfig*>(pAppCfg)->m_RendererConfig.m_hWnd = m_hWnd;

			ShowWindow(m_hWnd, SW_SHOWNORMAL);
			UpdateWindow(m_hWnd);

			return TRUE;

		}

		bool ApplicationWindow::ConfigureFullScreenMode(bool bFullScreen)
		{
			ApplicationConfig* pAppCfg = const_cast<ApplicationConfig*>(Application::Singleton().GetApplicationConfig());

			DEVMODE dmScreenSettings;					// Device Mode
			memset(&dmScreenSettings,0,sizeof(dmScreenSettings));		// Makes Sure Memory's Cleared
			dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Size Of The Devmode Structure
			dmScreenSettings.dmPelsWidth	= pAppCfg->m_RendererConfig.m_dwWidth;			// Selected Screen Width
			dmScreenSettings.dmPelsHeight	= pAppCfg->m_RendererConfig.m_dwHeight;			// Selected Screen Height
			dmScreenSettings.dmBitsPerPel	= 32;				// Selected Bits Per Pixel
			dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

			// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
			if(bFullScreen)
			{
				if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
				{
					// If The Mode Fails, Offer Two Options.  Quit Or Run In A Window.
					if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Windowed mode will be use instead?", L"Genium Application Engine", MB_OK|MB_ICONEXCLAMATION) == IDOK)
					{
						pAppCfg->m_RendererConfig.m_uFlags &= ~Gae::Renderer::RendererConfig::RCF_FULLSCREEN;
						pAppCfg->m_RendererConfig.m_uFlags |= Gae::Renderer::RendererConfig::RCF_WINDOWED;
					}
				}
			}

			return TRUE;

		}

		bool ApplicationWindow::Update()
		{
			if(GetAsyncKeyState(VK_ESCAPE))
			{
				m_bIsFinished = true;
			}

			if(PeekMessage(&m_msg, m_hWnd, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
				return TRUE;
			}

			return FALSE;
		}

		LRESULT CALLBACK ApplicationWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			switch (message)
			{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			case WM_SYSCOMMAND:						// Intercept System Commands
				switch (wParam)						// Check System Calls
				{
				case SC_SCREENSAVE:				// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
					return 0;					// Prevent From Happening
				}
				break;							// Exit
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
			}
			return 0;
		}

		bool ApplicationWindow::Uninit()
		{
			return TRUE;
		}
	}

}
