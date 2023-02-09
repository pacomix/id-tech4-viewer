#ifndef _APPLICATIONWINDOW_H__
#define _APPLICATIONWINDOW_H__



namespace Gae
{
	namespace Core
	{
		class Application;

		namespace Renderer
		{
			class Driver;
		}

		class ApplicationWindow
		{
			SINGLETON_DECL(ApplicationWindow);

			friend class Application;	//Only the engine can initialize the window.
			friend class Gae::Renderer::Driver;

		private:
			inline bool IsFinished() const { return m_bIsFinished;}
			bool Update();

			bool ConfigureFullScreenMode(bool bFullScreen);

			HINSTANCE m_hAppInstance;

			static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			HWND m_hWnd;
			MSG m_msg;

			LPCWSTR m_lpszClassName;
			bool m_bIsFinished;

		};
	}
}

#endif