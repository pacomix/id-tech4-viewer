#ifndef BSPTree_h__
#define BSPTree_h__
namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			class Plane;
		}
	}
	namespace Gifts
	{
		namespace Quake4
		{
			class BSPTree : public Gae::Core::Universe::IPortalAreaDetermination
			{
				friend class MapLoader;

			public:
				class Node
				{
					friend class BSPTree;
					friend class MapLoader;

				public:
					EIGEN_MAKE_ALIGNED_OPERATOR_NEW
					Node(){};
					~Node(){};

					inline int GetChild(const Vector3f& v3fPos)
					{
						//float fRes = v3fPos[0] * m_v4fHiperplane[0] + v3fPos[1] * m_v4fHiperplane[1] + v3fPos[2] * m_v4fHiperplane[2] + m_v4fHiperplane[3];
						//float fDot = m_v4fHiperplane.block(0,0,3,1).dot(v3fPos) + m_v4fHiperplane[3];

						//return fRes < 0 ? m_iChildNeg : m_iChildPos;
						//return fDot < 0 ? m_iChildNeg : m_iChildPos;
						return m_Plane.GetDistanceToPlane(v3fPos) < 0.0f ? m_iChildNeg : m_iChildPos;
					}

				private:
					//Vector4f m_v4fHiperplane;
					Graphics::Geometry::Plane m_Plane;

					int m_iChildPos;
					int m_iChildNeg;
				};

				BSPTree(UINT32 uNodeCount)
				{
					m_pRoot =  new Node[uNodeCount];
				}

				~BSPTree()
				{
					GAE_DELETESAFEARRAY(m_pRoot);
				}

				inline int GetAreaPortal(const Vector3f& v3fPos) const
				{
					int iNode = 0;

					do 
					{
						iNode = m_pRoot[iNode].GetChild(v3fPos);
					} while (iNode > 0);

					return -1-iNode;	// See .proc file. When a negative child is found it means we found a correct area for the position. Returning -1-area will give us the area number.
				}

			private:
				Node* m_pRoot;

			};
		}
	}
}

#endif // BSPTree_h__