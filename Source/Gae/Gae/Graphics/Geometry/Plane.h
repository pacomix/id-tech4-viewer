#ifndef Plane_h__
#define Plane_h__

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			class Plane
			{
			public:
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW

				Plane(){};
				~Plane(){};

				void SetFromPoints(const Vector3f& v3fPoint1, const Vector3f& v3fPoint2, const Vector3f& v3fPoint3);

				inline float GetDistanceToPlane(const Vector3f& v3fPoint) const
				{					
					return m_v3fNormal.dot(v3fPoint) + m_fDistance;
				}

				void GetIntersectionSegment(const Vector3f& v3fSegmentStart, const Vector3f& v3fSegmentEnd, Vector3f& v3fOut) const;
				bool ClipLineSegment2(Vector3f& v3fSegmentStart, Vector3f& v3fSegmentEnd, Vector3f* v3fClipped, int* iClippedCount) const;

				/* 
					\brief Sutherland-Hodgman clipping.
				*/
				void ClipPolygon(/*const */Vector3f* v3fTriStart, int* iVtxCount, Vector3f* v3fClipped = NULL, int* iClippedCount = NULL) const;

				inline bool IsLineSegmentInside(const Vector3f& v3fSegmentStart, const Vector3f& v3fSegmentEnd) const
				{
					const float fDot1 = GetDistanceToPlane(v3fSegmentStart);
					const float fDot2 = GetDistanceToPlane(v3fSegmentEnd);
					return fDot1 >= 0.0f || fDot2 >= 0.0f;
				}

				union
				{
					struct
					{
						Vector3f m_v3fNormal;
						float m_fDistance;
					};

					struct
					{
						Vector4f m_v4fNormalDistance;
					};
				};
			};
		}
	}
}
#endif // Plane_h__