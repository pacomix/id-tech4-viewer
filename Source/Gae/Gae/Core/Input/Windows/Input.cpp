#include "stdafx.h"

namespace Gae
{
	namespace Core
	{
		namespace Input
		{
			SINGLETON_IMPL(Manager);

			bool Manager::Init()
			{
				HINSTANCE hAppInst = Gae::Core::Application::Singleton().GetApplicationConfig()->m_hAppInstance;
				HWND hWnd = Gae::Core::Application::Singleton().GetApplicationConfig()->m_RendererConfig.m_hWnd;
				m_DIKArrayCurrent = (char*)operator new(KEYS_SIZE);
				m_DIKArrayOld = (char*)operator new(KEYS_SIZE);
				// m_pDIMouseStateOld = new DIMOUSESTATE;
				// m_pDIMouseStateCurrent = new DIMOUSESTATE;
				m_pDIMouseStateOld = new DIMOUSESTATE;
				m_pDIMouseStateCurrent = new DIMOUSESTATE;
				ClearKeyboardData();
				ClearMouseData();
				GAE_ASSERT_FN(DI_OK, DirectInput8Create(hAppInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pDIObject, NULL));

				// Create and acquire the keyboard device.
				GAE_ASSERT_FN(DI_OK, m_pDIObject->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboardDevice, NULL));
				GAE_ASSERT_FN(DI_OK, m_pDIKeyboardDevice->SetDataFormat(&c_dfDIKeyboard));
#ifdef _DEBUG
				GAE_ASSERT_FN(DI_OK, m_pDIKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND));
#else
				GAE_ASSERT_FN(DI_OK, m_pDIKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY));
#endif
				GAE_ASSERT_FN(DI_OK, m_pDIKeyboardDevice->Acquire());

				// Create and acquire the mouse device.
				GAE_ASSERT_FN(DI_OK, m_pDIObject->CreateDevice(GUID_SysMouse, &m_pDIMouseDevice, NULL));
				GAE_ASSERT_FN(DI_OK, m_pDIMouseDevice->SetDataFormat(&c_dfDIMouse));
				GAE_ASSERT_FN(DI_OK, m_pDIMouseDevice->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND));
				GAE_ASSERT_FN(DI_OK, m_pDIMouseDevice->Acquire());
				m_fMouseSensitiveness = 1.0f;

				return TRUE;
			}

			void Manager::ClearKeyboardData()
			{
				memset(m_DIKArrayCurrent, 0, KEYS_SIZE);
				memset(m_DIKArrayOld, 0, KEYS_SIZE);
			}

			void Manager::ClearMouseData()
			{
				memset(m_pDIMouseStateOld, 0, sizeof(DIMOUSESTATE));
				memset(m_pDIMouseStateCurrent, 0, sizeof(DIMOUSESTATE));
			}

			void Manager::Update()
			{
				// Keyboard
				if(FAILED(m_pDIKeyboardDevice->GetDeviceState(KEYS_SIZE, (LPVOID)m_DIKArrayCurrent)))
				{
					if(FAILED(m_pDIKeyboardDevice->Acquire()))
					{	// Clear the keyboard to simulate we released
						// the whole keys in the case the last time
						// we checked the keyboard state one of the keys
						// were being pressed.
						memset(m_DIKArrayCurrent, 0, KEYS_SIZE);
					}

					if(FAILED(m_pDIKeyboardDevice->GetDeviceState(KEYS_SIZE, (LPVOID)m_DIKArrayCurrent)))
					{
						memset(m_DIKArrayCurrent, 0, KEYS_SIZE);
					}
				}

				{	// Now we can check if we pressed/released any key.
					for(UINT i = 0; i < 256; ++i)
					{
						if(IsKeyPressed(i))
						{
							OnKeyPress.emit(i);
						}
						else if(IsKeyReleased(i))
						{	//Test if the key was released.
							OnKeyRelease.emit(i);
						}
					}

					// "Copy" the new key array into the new one.
					char* tempArrayOld = m_DIKArrayOld;
					m_DIKArrayOld = m_DIKArrayCurrent;
					m_DIKArrayCurrent = tempArrayOld;
				}

				// Mouse
				if(FAILED(m_pDIMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)m_pDIMouseStateCurrent)))
				{
					if(FAILED(m_pDIMouseDevice->Acquire()))
					{	// Clear the mouse data to simulate we released
						// the whole buttons in the case the last time
						// we checked the mouse state one of the buttons
						// were being pressed.
						memset(m_pDIMouseStateCurrent, 0, sizeof(DIMOUSESTATE));
					}
					if(FAILED(m_pDIMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)m_pDIMouseStateCurrent)))
					{
						memset(m_pDIMouseStateCurrent, 0, sizeof(DIMOUSESTATE));
					}
				}

				{	// Check for presses.
					for(UINT i = 0; i < 4; ++i)
					{
						if(IsMousePressed(i))
						{
							OnMousePress.emit(i);
						}
						else if(IsMouseReleased(i))
						{
							OnMouseRelease.emit(i);
						}
					}

					// Check for axis
					if(m_pDIMouseStateCurrent->lX != 0.0f)
					{
						LONG iValue = (LONG)((FLOAT)m_pDIMouseStateCurrent->lX * m_fMouseSensitiveness);
						OnMouseAxisX.emit(iValue);
					}
					
					if(m_pDIMouseStateCurrent->lY != 0.0f)
					{
						LONG iValue = (LONG)((FLOAT)m_pDIMouseStateCurrent->lY * m_fMouseSensitiveness);
						OnMouseAxisY.emit(iValue);
					}
					
					if(m_pDIMouseStateCurrent->lZ != 0.0f)
					{
						LONG iValue = (LONG)((FLOAT)m_pDIMouseStateCurrent->lZ * m_fMouseSensitiveness);
						OnMouseAxisZ.emit(iValue);
					}					

					// Exchange the state structure
					LPDIMOUSESTATE temp = m_pDIMouseStateOld;
					m_pDIMouseStateOld = m_pDIMouseStateCurrent;
					m_pDIMouseStateCurrent = temp;
				}
			}

			bool Manager::Uninit()
			{
				m_pDIKeyboardDevice->Release();
				m_pDIObject->Release();

				GAE_DELETESAFE(m_DIKArrayOld);
				GAE_DELETESAFE(m_DIKArrayCurrent);
				GAE_DELETESAFE(m_pDIMouseStateOld);
				GAE_DELETESAFE(m_pDIMouseStateCurrent);

				return TRUE;
			}
		}
	}
}