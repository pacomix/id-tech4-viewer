#ifndef Node_h__
#define Node_h__

namespace Gae
{
	namespace Graphics
	{
		// TODO: Move this class into Gae::Entities
		class Node
		{
		public:
			EIGEN_MAKE_ALIGNED_OPERATOR_NEW

			Node();
			~Node();

			// --------------- Transforms --------------- 
			void ResetLocalTransformations();

			inline void SetWorldTransform(const Matrix4f& m44fWorldTransform);
			inline void SetRotation(const Quaternionf& qRot);
			inline void SetTranslation(const Vector3f& v3fTrans);
			inline void SetScale(const Vector3f& v3fScale);
			inline void SetFromAxis(const Vector3f& v3fAxisX, const Vector3f& v3fAxisY, const Vector3f& v3fAxisZ);

			inline void SetPosition(const Vector3f& v3fPosition);

			inline void AddRotation(const Quaternionf& qRot);
			inline void AddRotationX(const Quaternionf& qRot);
			inline void AddRotationY(const Quaternionf& qRot);
			inline void AddRotationZ(const Quaternionf& qRot);
			inline void AddTranslation(const Vector3f& v3fTrans);
			inline void AddWorldTranslation(const Vector3f& v3fTrans);
			inline void AddScale(const Vector3f& v3fScale);
			// ------------------------------------------

			inline const Vector3f& GetLocalPosition() const;
			inline const Quaternionf& GetLocalRotation() const;
			inline const Vector3f& GetScale() const;
			//const Matrix34f& GetLocalTransform() const;

			inline const Vector3f& GetWorldPosition() /*const*/;
			inline const Matrix4f& GetWorldTransform();
			inline const Vector3f& GetWorldPositionDirect() const;
			inline const Matrix4f& GetWorldTransformDirect() const;

			inline const Vector3f& GetWorldAxisX() const;
			inline const Vector3f& GetWorldAxisY() const;
			inline const Vector3f& GetWorldAxisZ() const;

			inline const Quaternionf& GetRotationX() const;
			inline const Quaternionf& GetRotationY() const;
			inline const Quaternionf& GetRotationZ() const;

			// --------------- Hierarchy ---------------
			inline void AttachNode(Node* pNode);
			inline void DettachNode(const Node* pNode);
			// -----------------------------------------

			inline bool IsDirty() const;
			void UpdateWorldTransform();

		private:
		#define DF_ROTATION		0x00000001
		#define DF_TRANSLATION	0x00000002
		#define DF_SCALE		0x00000004

			UINT32 m_uDirtyFlags;	//!< Flags for Visual Debug output.
			inline void DirtyFlagSet(UINT32 uFlag)
			{
				m_uDirtyFlags |= uFlag;
			}

			inline void DirtyFlagUnset(UINT32 uFlag)
			{
				m_uDirtyFlags &= ~uFlag;
			}

			inline void DirtyFlagToggle(UINT32 uFlag)
			{
				m_uDirtyFlags ^= uFlag;
			}

			inline const bool DirtyFlagEnabled(UINT32 uFlag) const
			{
				return (m_uDirtyFlags & uFlag) > 0;
			}


			// --------------- Transform --------------- 
			Matrix4f	m_mtx44fWorldTransform;	//!< World transform matrix
			

			Vector3f	m_v3fScale;			//!< Local scale
			Vector3f	m_v3fTranslation;	//!< Local translation
			Vector3f	m_v3fWorldTranslation;	//!< World translation

			Quaternionf m_qRotation;		//!< Local rotation
			Quaternionf m_qRotationX;		//!< Local rotation
			Quaternionf m_qRotationY;		//!< Local rotation
			Quaternionf m_qRotationZ;		//!< Local rotation

			bool	m_bDirty;

			void SetDirty();

			// functor for SetDirty
			struct ftorSetDirty
			{
				inline void operator() (Node* pNode);
			};
			// -----------------------------------------

			// --------------- Hierarchy --------------- 
			inline void SetRootNode(Node* pNode);
			inline Node* GetRootNode() const;

			typedef std::deque< Node* > DeqChilds;
			Node* m_pNodeRoot;
			DeqChilds m_DeqChilds;
			// -----------------------------------------
		};
	}
}

#include "Node.inl"

#endif // Node_h__
