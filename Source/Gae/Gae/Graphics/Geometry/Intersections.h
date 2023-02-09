#ifndef Intersections_h__
#define Intersections_h__

// TODO: Delete this methods as they exist in the plane class.
namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			// \brief	Returns the intersection point of a segment against a plane.
			// \note	Passed segment must have a valid intersection point. This is
			//		each point must lie on either side of the plane.
			bool IntersectionAABBEllipsoid(const AABB& aabb, const Vector3f& v3fEllipsoidCenter, const Vector3f& v3fRadii);
			bool ClipRectAgainstRect(const RECT& rectSource, RECT& rectToModify);
		}
	}
}

#endif // Intersections_h__