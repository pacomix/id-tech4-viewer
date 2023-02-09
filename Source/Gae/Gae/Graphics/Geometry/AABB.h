#ifndef AABB_h__
#define AABB_h__

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			class AABB
#ifdef _DEBUG_DRAW
				: public Gae::Renderer::IDrawDebug
#endif
			{
				EIGEN_MAKE_ALIGNED_OPERATOR_NEW

			public:
				AABB();
				~AABB();

				inline void SetValues(const Vector3f& v3fMin, const Vector3f& v3fMax);

				inline const Vector3f& GetMin() const;
				inline const Vector3f& GetMax() const;
				inline const Vector3f& GetCenter() const;
				inline const Sphere& GetBoundingSphere() const;

#ifdef _DEBUG_DRAW
				void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif
			private:
				Vector3f m_v3fMin;
				Vector3f m_v3fMax;
				Vector3f m_v3fCenter;

				Sphere m_BoundingSphere;

			};
		}
	}
}

#include "AABB.inl"
#endif // AABB_h__