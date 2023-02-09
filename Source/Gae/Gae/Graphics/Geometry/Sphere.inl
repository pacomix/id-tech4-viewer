#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			void Sphere::SetValues(const Vector3f& v3fCenter, const float& fRadius)
			{
				SetCenter(v3fCenter);
				SetRadius(fRadius);
			}

			void Sphere::SetCenter(const Vector3f& v3fCenter)
			{
				m_v3fCenter = v3fCenter;
			}

			void Sphere::SetRadius(const float& fRadius)
			{
				GAE_WARNINGM(fRadius > 0.0f, L"Incorrect radius value. 0 < fRadius: 0 < %f", fRadius); 
				m_fRadius = fRadius;
				m_fRadiusSquared = fRadius * fRadius;
			}

			const Vector3f& Sphere::GetCenter() const
			{
				return m_v3fCenter;
			} 

			const float& Sphere::GetRadius() const
			{
				return m_fRadius;
			}

			const float& Sphere::GetRadiusSquared() const
			{
				return m_fRadiusSquared;
			}

			float Sphere::GetDistance(const Vector3f& v3fPoint) const
			{
				return (v3fPoint - m_v3fCenter).norm() - m_fRadius;
			}

			float Sphere::GetDistanceSquared(const Vector3f& v3fPoint) const
			{
				return (v3fPoint - m_v3fCenter).squaredNorm() - m_fRadiusSquared;
			}

			bool Sphere::IsInside(const Vector3f& v3fPoint) const
			{
				return GetDistanceSquared(v3fPoint) < 0.0f;
			}

			bool Sphere::Intersect(const Sphere& sphere) const
			{
				return sphere.GetDistanceSquared(m_v3fCenter) < m_fRadiusSquared;
			}
		}
	}
}