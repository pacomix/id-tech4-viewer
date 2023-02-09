#include "stdafx.h"
#include "Node.h"

namespace Gae
{
	namespace Graphics
	{
		Node::Node()
		{
			m_qRotation.setIdentity();
			m_qRotationX = Quaternionf(AngleAxisf(0.0f, Vector3f::UnitX()));
			m_qRotationY = Quaternionf(AngleAxisf(0.0f, Vector3f::UnitY()));
			m_qRotationZ = Quaternionf(AngleAxisf(0.0f, Vector3f::UnitZ()));
			m_v3fScale = Vector3f(1.0f, 1.0f, 1.0f);
			m_v3fTranslation.setZero();
			m_v3fWorldTranslation.setZero();
			m_mtx44fWorldTransform.setIdentity();

			m_pNodeRoot = NULL;
			m_uDirtyFlags = 0;
			UpdateWorldTransform();
		}

		void Node::UpdateWorldTransform()
		{
			if(m_pNodeRoot)
			{
				// TODO: Add the scaling
				// Rotation
				if(DirtyFlagEnabled(DF_ROTATION))
				{
					m_mtx44fWorldTransform.block(0,0,3,3) = (m_qRotationX * (m_qRotationZ * m_qRotationY)).normalized().toRotationMatrix() * m_pNodeRoot->GetWorldTransform().block(0,0,3,3);
				}
				// Translation
				if(DirtyFlagEnabled(DF_TRANSLATION))
				{
					m_mtx44fWorldTransform.block(0,3,3,1) += m_pNodeRoot->GetWorldTransform().col(3);	
				}
			}
			else
			{
				// TODO: Add the scaling
				// Rotation
				if(DirtyFlagEnabled(DF_ROTATION))
				{
					//m_mtx44fWorldRotation.block(0,0,3,3) = (m_qRotationX * (m_qRotationZ * m_qRotationY)).normalized().toRotationMatrix();
					m_mtx44fWorldTransform.block(0,0,3,3) = (m_qRotationX * (m_qRotationZ * m_qRotationY)).normalized().toRotationMatrix();
				}

				// Translation
				if(DirtyFlagEnabled(DF_TRANSLATION))
				{
					m_mtx44fWorldTransform.block(0,3,3,1) += (m_mtx44fWorldTransform.block(0,0,3,3) * m_v3fTranslation) + m_v3fWorldTranslation;
					//m_mtx44fWorldTranslation.block(0,3,3,1) += m_v3fTranslation + m_v3fWorldTranslation;
				}

				if(DirtyFlagEnabled(DF_SCALE))
				{
					//m_mtx44fWorldTransform = m_mtx44fWorldTransform * m_v3fTranslation) + m_v3fWorldTranslation;
					//m_mtx44fWorldTranslation.block(0,3,3,1) += m_v3fTranslation + m_v3fWorldTranslation;
				}

			}

			m_v3fTranslation.setZero();	// After updating the world transform
										// reset the local translation.
			m_v3fWorldTranslation.setZero();

			m_bDirty = false;
			m_uDirtyFlags = 0;
		}

		void Node::SetDirty()
		{
			// Shit flows from up to down...
			for_each(m_DeqChilds.begin(), m_DeqChilds.end(), Node::ftorSetDirty());
			m_bDirty = TRUE;
		}

		Node::~Node()
		{
		}
	}
}

