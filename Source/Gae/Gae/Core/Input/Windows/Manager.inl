namespace Gae
{
	namespace Core
	{
		namespace Input
		{
			inline bool Manager::IsKeyPressed(UINT keyCode)
			{
				return (m_DIKArrayCurrent[keyCode] & 0x80) && !(m_DIKArrayOld[keyCode] & 0x80);
			}

			inline bool Manager::IsKeyReleased(UINT keyCode)
			{
				return (!(m_DIKArrayCurrent[keyCode] & 0x80)) && (m_DIKArrayOld[keyCode] & 0x80);
			}

			inline bool Manager::IsMousePressed(UINT buttonCode)
			{
				GAE_ASSERTM(4 > buttonCode, L"Mouse button index incorrect. Should be 0 <= buttonCode < 4\n");
				return (m_pDIMouseStateCurrent->rgbButtons[buttonCode] & 0x80) && !(m_pDIMouseStateOld->rgbButtons[buttonCode] & 0x80);
			}

			inline bool Manager::IsMouseReleased(UINT buttonCode)
			{
				GAE_ASSERTM(4 > buttonCode, L"Mouse button index incorrect. Should be 0 <= buttonCode < 4\n");
				return (!(m_pDIMouseStateCurrent->rgbButtons[buttonCode] & 0x80)) && (m_pDIMouseStateOld->rgbButtons[buttonCode] & 0x80);
			}

			inline void Manager::SetMouseSensitiveness(FLOAT fSens)
			{
				GAE_ASSERTM(fSens > 0.0f, L"Mouse sensitiveness must be greater than 0!");
				m_fMouseSensitiveness = fSens;
			}

		}
	}
}