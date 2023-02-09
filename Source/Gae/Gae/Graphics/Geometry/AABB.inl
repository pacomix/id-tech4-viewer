namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			/*inline */void AABB::SetValues(const Vector3f &v3fMin, const Vector3f &v3fMax)
			{
				GAE_ASSERTM(
					v3fMin.x() <= v3fMax.x() &&
					v3fMin.y() <= v3fMax.y() &&
					v3fMin.z() <= v3fMax.z(),
					L"Incorrect values. v3fMin has values that are greater than some values of v3fMax...");


				m_v3fMin = v3fMin;
				m_v3fMax = v3fMax;

				// Implicit bounding sphere.
				float fRadius = (v3fMax - v3fMin).norm() * 0.5f;
				m_BoundingSphere.SetValues(m_v3fMin + ((v3fMax - v3fMin).normalized() * fRadius), fRadius);

				// Center.
				m_v3fCenter = v3fMin + ((v3fMax - v3fMin) * 0.5f);
			}

			/*inline */const Vector3f& AABB::GetMin() const
			{
				return m_v3fMin;
			}

			/*inline */const Vector3f& AABB::GetMax() const
			{
				return m_v3fMax;
			}

			const Vector3f& AABB::GetCenter() const
			{
				return m_v3fCenter;
			}

			/*inline */const Sphere& AABB::GetBoundingSphere() const
			{
				return m_BoundingSphere;
			}
		}
	}
}