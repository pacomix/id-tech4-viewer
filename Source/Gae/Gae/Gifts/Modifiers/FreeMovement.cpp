#include "stdafx.h"

namespace Gae
{
	namespace Gifts
	{
		namespace Modifiers
		{
			const float FreeMovement::m_fSpeed = 250.0f;	//5000 cm per second (5m per second) - Common running man.

			FreeMovement::FreeMovement()
			{
			}

			FreeMovement::~FreeMovement()
			{
			}

			bool FreeMovement::Init(Gae::Entities::IEntity* pEntity)
			{
				super::Init(pEntity);

				// Register the input callbacks
				GAE_INPUT_REGISTER_EVENT(OnKeyPress, this, &FreeMovement::KeyPressed);
				GAE_INPUT_REGISTER_EVENT(OnKeyRelease, this, &FreeMovement::KeyReleased);
				GAE_INPUT_REGISTER_EVENT(OnMousePress, this, &FreeMovement::MousePressed);
				GAE_INPUT_REGISTER_EVENT(OnMouseRelease, this, &FreeMovement::MouseReleased);
				GAE_INPUT_REGISTER_EVENT(OnMouseAxisX, this, &FreeMovement::MouseAxisX);
				GAE_INPUT_REGISTER_EVENT(OnMouseAxisY, this, &FreeMovement::MouseAxisY);
				GAE_INPUT_REGISTER_EVENT(OnMouseAxisZ, this, &FreeMovement::MouseAxisZ);

				// Init the step rotation quaternions
				m_rotX = AngleAxisf(GAE_RAD_DEGREE, Vector3f::UnitX());
				m_rotY = AngleAxisf(GAE_RAD_DEGREE, Vector3f::UnitY());
				m_qStepRotX = Quaternionf(m_rotX);
				m_qStepRotY = Quaternionf(m_rotY);

				m_fSpeedMultiplier = 1.0f;

				ResetMovementFlag();
				return TRUE;
			}

			void FreeMovement::Run()
			{
				float fOffset = m_fSpeed * m_fSpeedMultiplier * (float)Gae::Core::Clock::GetEllapsedSeconds();

				// Take the decisions for moving the parent.
				if(m_uMovementFlag & MD_FW) m_pParent->AddTranslation(Vector3f::UnitZ() * fOffset);
				if(m_uMovementFlag & MD_BW) m_pParent->AddTranslation(-Vector3f::UnitZ() * fOffset);
				if(m_uMovementFlag & MD_LEFT) m_pParent->AddTranslation(-Vector3f::UnitX() * fOffset);
				if(m_uMovementFlag & MD_RIGHT) m_pParent->AddTranslation(Vector3f::UnitX() * fOffset);
				if(m_uMovementFlag & MD_UP) m_pParent->AddWorldTranslation(Vector3f::UnitY() * fOffset);
				if(m_uMovementFlag & MD_DOWN) m_pParent->AddWorldTranslation(-Vector3f::UnitY() * fOffset);
			}

			void FreeMovement::KeyPressed(UINT uKeyCode)
			{
//				GAE_TRACE(L"Key Pressed: [%u]\n", uKeyCode);
				switch(uKeyCode)
				{
				case DIK_W:
					SetMovementFlag(MD_FW);
					break;
				case DIK_S:
					SetMovementFlag(MD_BW);
					break;
				case DIK_A:
					SetMovementFlag(MD_LEFT);
					break;
				case DIK_D:
					SetMovementFlag(MD_RIGHT);
					break;
				case DIK_Q:
					SetMovementFlag(MD_UP);
					break;
				case DIK_E:
					SetMovementFlag(MD_DOWN);
					break;
				default:
					break;
				}
			}

			void FreeMovement::KeyReleased(UINT uKeyCode)
			{
				//GAE_TRACE(L"Key Released: [%u]\n", uKeyCode);
				switch(uKeyCode)
				{
				case DIK_W:
					UnsetMovementFlag(MD_FW);
					break;
				case DIK_S:
					UnsetMovementFlag(MD_BW);
					break;
				case DIK_A:
					UnsetMovementFlag(MD_LEFT);
					break;
				case DIK_D:
					UnsetMovementFlag(MD_RIGHT);
					break;
				case DIK_Q:
					UnsetMovementFlag(MD_UP);
					break;
				case DIK_E:
					UnsetMovementFlag(MD_DOWN);
					break;
				default:
					break;
				}
			}

			void FreeMovement::MousePressed(UINT uBtnCode)
			{
				GAE_TRACE(L"Mouse Pressed: [%u]\n", uBtnCode);

				switch(uBtnCode)
				{
				case 0:	// left button pressed
					m_fSpeedMultiplier = 0.2f;
					break;
				case 1:	// right button pressed
					//SetMovementFlag(MD_FW);
					m_fSpeedMultiplier = 3.0f;
					break;
				default:
					break;
				}
			}

			void FreeMovement::MouseReleased(UINT uBtnCode)
			{
				//GAE_TRACE(L"Mouse Released: [%u]\n", uBtnCode);
				switch(uBtnCode)
				{
				case 0:	// left or
				case 1:	// right button pressed
					//UnsetMovementFlag(MD_FW);
					m_fSpeedMultiplier = 1.0f;
					break;
				default:
					break;
				}
			}

			void FreeMovement::MouseAxisX(LONG iOffset)
			{
				//GAE_TRACE(L"Mouse Axis X: [%i]\n", iOffset);
				m_rotY.angle() = GAE_RAD_DEGREE * 0.25f * (float)iOffset;
				m_pParent->AddRotationX(Quaternionf(m_rotY));
			}

			void FreeMovement::MouseAxisY(LONG iOffset)
			{
				//GAE_TRACE(L"Mouse Axis Y: [%i]\n", iOffset);
				m_rotX.angle() = GAE_RAD_DEGREE * 0.25f * (float)iOffset;
				m_pParent->AddRotationY(Quaternionf(m_rotX));
			}

			void FreeMovement::MouseAxisZ(LONG iOffset)
			{
				//GAE_TRACE(L"Mouse Axis Z: [%i]\n", iOffset);
			}

			void FreeMovement::Uninit()
			{
				// Unregister the input callbacks.
				GAE_INPUT_UNREGISTER_EVENT(OnKeyPress, this);
				GAE_INPUT_UNREGISTER_EVENT(OnKeyRelease, this);
				GAE_INPUT_UNREGISTER_EVENT(OnMousePress, this);
				GAE_INPUT_UNREGISTER_EVENT(OnMouseRelease, this);
				GAE_INPUT_UNREGISTER_EVENT(OnMouseAxisX, this);
				GAE_INPUT_UNREGISTER_EVENT(OnMouseAxisY, this);
				GAE_INPUT_UNREGISTER_EVENT(OnMouseAxisZ, this);
			}
		}
	}
}