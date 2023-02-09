#ifndef Windows_Input_h__
#define Windows_Input_h__

namespace Gae
{
	namespace Core
	{
		class Application;

		namespace Input
		{
			class Manager
			{
				friend class Gae::Core::Application;

				SINGLETON_DECL(Manager);

			public:
				bool IsKeyPressed(UINT keyCode);
				bool IsKeyReleased(UINT keyCode);
				bool IsMousePressed(UINT keyCode);
				bool IsMouseReleased(UINT keyCode);
				
				// Signal. Used by macros.
				// TODO:	Extend the system to assign keys directly to a 
				//		class method instead of using this signal system.
				//			Read DirectInput doc. I think I read something
				//		somewhere.
				ksignal1::Signal1<UINT> OnKeyPress;
				ksignal1::Signal1<UINT> OnKeyRelease;

				ksignal1::Signal1<UINT> OnMousePress;
				ksignal1::Signal1<UINT> OnMouseRelease;

				ksignal1::Signal1<LONG> OnMouseAxisX;
				ksignal1::Signal1<LONG> OnMouseAxisY;
				ksignal1::Signal1<LONG> OnMouseAxisZ;

				void SetMouseSensitiveness(FLOAT fSens);

			private:

				void Update();

				void ClearKeyboardData();
				void ClearMouseData();

				LPDIRECTINPUT8 m_pDIObject;
				LPDIRECTINPUTDEVICE8 m_pDIKeyboardDevice;
				LPDIRECTINPUTDEVICE8 m_pDIMouseDevice;

				// Keyboard
				static const int KEYS_SIZE = 256;
				char* m_DIKArrayOld;	//!< array with key press state for immediate mode
				char* m_DIKArrayCurrent;	//!< array with key press state for immediate mode
				// TODO: Implement buffered keyboard data

				// Mouse
				static const int MOUSE_BUTTONS_SIZE = 256;
				LPDIMOUSESTATE m_pDIMouseStateOld;
				LPDIMOUSESTATE m_pDIMouseStateCurrent;
				FLOAT m_fMouseSensitiveness;



			};
		}
	}
}

#include "Core/Input/Windows/Manager.inl"

#endif // Input_h__