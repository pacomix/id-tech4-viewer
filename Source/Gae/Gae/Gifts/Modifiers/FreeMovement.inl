namespace Gae
{
	namespace Gifts
	{
		namespace Modifiers
		{
			inline void FreeMovement::ResetMovementFlag()
			{
				m_uMovementFlag = MD_NONE;
			}

			inline void FreeMovement::SetMovementFlag(E_MovementDirections eMD)
			{
				m_uMovementFlag |= eMD;
			}

			inline void FreeMovement::UnsetMovementFlag(E_MovementDirections eMD)
			{
				m_uMovementFlag &= ~eMD;
			}
		}
	}
}