#ifndef FreeMovement_h__
#define FreeMovement_h__

namespace Gae
{
	namespace Entities
	{
		class IEntity;
	}
	namespace Gifts
	{
		namespace Modifiers
		{
			class FreeMovement : public Gae::Entities::Modifier
			{
			public:
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW

				typedef Gae::Entities::Modifier super;
				FreeMovement();
				virtual ~FreeMovement();

				// Modifier 
				bool Init(Gae::Entities::IEntity* pEntity);
				void Run();
				void Uninit();

			protected:
				// Movement flags.
				enum E_MovementDirections
				{
					MD_NONE 	=	0x00000000,
					MD_UP		=	0x00000001,
					MD_DOWN		=	0x00000002,
					MD_FW		=	0x00000004,
					MD_BW		=	0x00000008,
					MD_LEFT 	=	0x00000010,
					MD_RIGHT	=	0x00000020
				};

				void SetMovementFlag(E_MovementDirections eMD);
				void UnsetMovementFlag(E_MovementDirections eMD);
				void ResetMovementFlag();

				UINT32 m_uMovementFlag;

				Vector3f m_v3fDirection;
				AngleAxisf m_rotX;
				AngleAxisf m_rotY;
				Quaternionf m_qStepRotX;
				Quaternionf m_qStepRotY;

			private:
				// Speed
				static const float m_fSpeed;
				float m_fSpeedMultiplier;
				// Input callbacks
				void KeyPressed(UINT uKeyCode);		//!< DIK_xxxxx
				void KeyReleased(UINT uKeyCode);	//!< DIK_xxxxx
				void MousePressed(UINT uBtnCode);	//!< DIMOFS_BUTTONxxxxx
				void MouseReleased(UINT uBtnCode);	//!< DIMOFS_BUTTONxxxxx
				void MouseAxisX(LONG iOffset);		//!< Offset from the last position.
				void MouseAxisY(LONG iOffset);		//!< Offset from the last position.
				void MouseAxisZ(LONG iOffset);		//!< Offset from the last position. The wheel mouse.

			};
		}
	}
}

#include "FreeMovement.inl"

#endif // FreeMovement_h__