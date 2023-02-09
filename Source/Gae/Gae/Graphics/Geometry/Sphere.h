#ifndef Sphere_h__
#define Sphere_h__

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			class Sphere
			{
			public:
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW

					Sphere() : m_v3fCenter(Vector3f::Zero()), m_fRadius(0.0f), m_fRadiusSquared(0.0f){};
				~Sphere(){};

				inline void SetValues(const Vector3f& v3fCenter, const float& fRadius);
				inline void SetCenter(const Vector3f& v3fCenter);
				inline void SetRadius(const float& fRadius);
				inline const Vector3f& GetCenter() const;
				inline const float& GetRadius() const;
				inline const float& GetRadiusSquared() const;
				inline float GetDistance(const Vector3f& v3fPoint) const;
				inline float GetDistanceSquared(const Vector3f& v3fPoint) const;
				inline bool IsInside(const Vector3f& v3fPoint) const;
				inline bool Intersect(const Sphere& sphere) const;

				//Vector3f GetIntersectionSegment(const Vector3f& v3fSegmentStart, const Vector3f& v3fSegmentEnd) const;

			private:
				union
				{
					struct
					{
						Vector3f m_v3fCenter;
						float m_fRadius;
					};

					struct
					{
						Vector4f m_v4fCenterRadius;
					};
				};

				float m_fRadiusSquared;
			};
		}
	}
}
#include "Sphere.inl"
#endif // Sphere_h__