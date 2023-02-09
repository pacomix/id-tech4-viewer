namespace Gae
{
	namespace Graphics
	{
		void Node::SetWorldTransform(const Matrix4f& m44fWorldTransform)
		{
			m_mtx44fWorldTransform = m44fWorldTransform;
		}

		void Node::SetRotation(const Quaternionf& qRot)
		{
			m_qRotation = qRot;
			DirtyFlagSet(DF_ROTATION);
		}

		void Node::SetTranslation(const Vector3f& v3fTrans)
		{
			m_v3fTranslation = v3fTrans;
			DirtyFlagSet(DF_TRANSLATION);
		}

		void Node::SetPosition(const Vector3f& v3fTrans)
		{
			m_mtx44fWorldTransform.block(0,3,3,1) = v3fTrans;
			//SetDirty();
		}

		void Node::SetScale(const Vector3f& v3fScale)
		{
			m_v3fScale = v3fScale;
			DirtyFlagSet(DF_SCALE);
		}

		void Node::SetFromAxis(const Vector3f& v3fAxisX, const Vector3f& v3fAxisY, const Vector3f& v3fAxisZ)
		{
			m_mtx44fWorldTransform[0] = v3fAxisX.x();
			m_mtx44fWorldTransform[1] = v3fAxisX.y();
			m_mtx44fWorldTransform[2] = v3fAxisX.z();

			m_mtx44fWorldTransform[4] = v3fAxisY.x();
			m_mtx44fWorldTransform[5] = v3fAxisY.y();
			m_mtx44fWorldTransform[6] = v3fAxisY.z();

			m_mtx44fWorldTransform[8] = v3fAxisZ.x();
			m_mtx44fWorldTransform[9] = v3fAxisZ.y();
			m_mtx44fWorldTransform[10] = v3fAxisZ.z();
		}

		void Node::AddRotation(const Quaternionf& qRot)
		{
			m_qRotation *= qRot;
			DirtyFlagSet(DF_ROTATION);
		}

		void Node::AddRotationX(const Quaternionf& qRot)
		{
			m_qRotationX *= qRot;
			DirtyFlagSet(DF_ROTATION);
		}

		void Node::AddRotationY(const Quaternionf& qRot)
		{
			m_qRotationY *= qRot;
			DirtyFlagSet(DF_ROTATION);
		}

		void Node::AddRotationZ(const Quaternionf& qRot)
		{
			m_qRotationZ *= qRot;
			DirtyFlagSet(DF_ROTATION);
		}

		void Node::AddTranslation(const Vector3f& v3fTrans)
		{
			m_v3fTranslation += v3fTrans;
			DirtyFlagSet(DF_TRANSLATION);
		}

		void Node::AddWorldTranslation(const Vector3f& v3fTrans)
		{
			m_v3fWorldTranslation += v3fTrans;
			DirtyFlagSet(DF_TRANSLATION);
		}

		void Node::AddScale(const Vector3f& v3fScale)
		{
			m_v3fScale.cwise() *= v3fScale;
			DirtyFlagSet(DF_SCALE);
		}

		const Vector3f& Node::GetLocalPosition() const
		{
			// TODO: parent's relative position
			return m_v3fTranslation;
		}

		const Quaternionf& Node::GetLocalRotation() const
		{
			return m_qRotation;
		}

		const Vector3f& Node::GetScale() const
		{
			return m_v3fScale;
		}

		const Vector3f& Node::GetWorldPosition() /*const*/
		{
			if(DirtyFlagEnabled(DF_TRANSLATION))
			{
				UpdateWorldTransform();
			}

			// Only valid on column-major matrix types.
			return *reinterpret_cast<Vector3f*>(m_mtx44fWorldTransform.data()+12);
		}

		const Vector3f& Node::GetWorldPositionDirect() const
		{
			GAE_WARNINGM(FALSE == DirtyFlagEnabled(DF_TRANSLATION), L"Be carefull! The node is dirty! Probably the given position is not the most recent one!");
			// Only valid on column-major matrix types.
			return *reinterpret_cast<const Vector3f*>(m_mtx44fWorldTransform.data()+12);
		}

		const Vector3f& Node::GetWorldAxisX() const
		{
			GAE_WARNINGM(FALSE == DirtyFlagEnabled(DF_ROTATION), L"Be carefull! The node is dirty! Probably the given position is not the most recent one!");
			return *reinterpret_cast<const Vector3f*>(m_mtx44fWorldTransform.data());
		}

		const Vector3f& Node::GetWorldAxisY() const
		{
			GAE_WARNINGM(FALSE == DirtyFlagEnabled(DF_ROTATION), L"Be carefull! The node is dirty! Probably the given position is not the most recent one!");
			return *reinterpret_cast<const Vector3f*>(m_mtx44fWorldTransform.data()+4);
		}

		const Vector3f& Node::GetWorldAxisZ() const
		{
			GAE_WARNINGM(FALSE == DirtyFlagEnabled(DF_ROTATION), L"Be carefull! The node is dirty! Probably the given position is not the most recent one!");
			return *reinterpret_cast<const Vector3f*>(m_mtx44fWorldTransform.data()+8);
		}

		const Quaternionf& Node::GetRotationX() const
		{
			return m_qRotationX;
		}

		const Quaternionf& Node::GetRotationY() const
		{
			return m_qRotationY;
		}

		const Quaternionf& Node::GetRotationZ() const
		{
			return m_qRotationZ;
		}

		const Matrix4f& Node::GetWorldTransform()
		{
			if(0 != m_uDirtyFlags)
			{
				UpdateWorldTransform();
			}

			return m_mtx44fWorldTransform;
		}

		const Matrix4f& Node::GetWorldTransformDirect() const
		{
			GAE_WARNINGM(0 == m_uDirtyFlags, L"Be carefull! The node is dirty! Probably the given world transform is not the most recent one!");
			return m_mtx44fWorldTransform;
		}

		void Node::AttachNode(Node* pNode)
		{
			// Early detection: Not valid or Already attached.
			if(!(pNode && NULL == pNode->GetRootNode()))
			{	// TODO: Add a warning to the output console.
				return;
			}

			// It is a valid node. Add it to the deque.
			m_DeqChilds.push_back(pNode);
			pNode->SetRootNode(this);

			// TODO: Make the attached node keeps the current position by setting its local transforms.
		}

		void Node::DettachNode(const Node* pNode)
		{
			// Early detection: Not valid or Is not attached.
			if(!(pNode && this == pNode->GetRootNode()))
			{	// TODO: Warning -> 
				return;
			}

			DeqChilds::iterator itFound = std::find(m_DeqChilds.begin(), m_DeqChilds.end(), pNode);
			if(itFound != m_DeqChilds.end())
			{
				m_DeqChilds.erase(itFound);
				(*itFound)->SetRootNode(NULL);
				// TODO: Make the detached node keeps the current transformation by setting its local transforms.
			}
		}

		void Node::SetRootNode(Node* pNode)
		{
			m_pNodeRoot = pNode;
		}

		Node* Node::GetRootNode() const
		{
			return m_pNodeRoot;
		}

		bool Node::IsDirty() const
		{
			return 0 != m_uDirtyFlags;
		}

		// functor for SetDirty
		void Node::ftorSetDirty::operator() (Node* pNode)
		{
			pNode->SetDirty();
		}
	}
}