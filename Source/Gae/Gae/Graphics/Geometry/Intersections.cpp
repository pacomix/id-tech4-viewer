#include "stdafx.h"
#include "Intersections.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			// \brief	Returns the intersection point of a segment against a plane.
			// \note	Passed segment must have a valid intersection point. This is
			//		each point must lie on either side of the plane.
			static Vector3f IntersectionSegmentPlane(const Vector3f& v3fVtx1, const Vector3f& v3fVtx2, const Vector4f& v4fPlane)
			{
				Vector3f v3fPlane = v4fPlane.block(0,0,3,1);

				// Early checks.
				if(v3fVtx1.dot(v3fPlane) == 0.0f ) return v3fVtx1;
				if(v3fVtx2.dot(v3fPlane) == 0.0f ) return v3fVtx2;

#ifdef _DEBUG
				{
					float fDot1 = v3fVtx1.dot(v3fPlane);
					float fDot2 = v3fVtx2.dot(v3fPlane);

					GAE_ASSERTM((fDot1 < 0 && fDot2 >= 0) || (fDot2 < 0 && fDot1 >= 0), L"Points doesn't intersect the planes. This method requires the points lies on either sides of the plane.");
				}
#endif
				float fDotSegmentPlane = (v3fVtx2 - v3fVtx1).dot(v3fPlane);
				float fDotSegment2Plane = -(v3fVtx1 - v3fPlane).dot(v3fPlane);
				Vector3f v3fIntersectionPoint = v3fVtx1 + ( (fDotSegment2Plane / fDotSegmentPlane) * (v3fVtx2 - v3fVtx1));
				return v3fIntersectionPoint;
			}

			bool IntersectionAABBEllipsoid(const AABB& aabb, const Vector3f& v3fEllipsoidCenter, const Vector3f& v3fRadii)
			{
				// TODO: Review. It seems that some meshes are not affected by any light.
				Vector3f v3fScale;
				Vector3f v3fTranslation;
				Vector3f v3fAABBMin = aabb.GetMin();
				Vector3f v3fAABBMax = aabb.GetMax();

				v3fScale.x() = 1.f / v3fRadii.x();
				v3fScale.y() = 1.f / v3fRadii.y();
				v3fScale.z() = 1.f / v3fRadii.z();

				v3fTranslation.x() = -v3fEllipsoidCenter.x() * v3fScale.x();
				v3fTranslation.y() = -v3fEllipsoidCenter.y() * v3fScale.y();
				v3fTranslation.z() = -v3fEllipsoidCenter.z() * v3fScale.z();
				
				float dmin = 0.f;
				float fMin, fMax;

				fMin = v3fAABBMin.x() * v3fScale.x() + v3fTranslation.x();
				fMax = v3fAABBMax.x() * v3fScale.x() + v3fTranslation.x();

				if (0.f < fMin) dmin+=fMin*fMin;
				else if (0.f > fMax) dmin+=fMax*fMax;

				fMin = v3fAABBMin.y() * v3fScale.y() + v3fTranslation.y();
				fMax = v3fAABBMax.y() * v3fScale.y() + v3fTranslation.y();

				if (0.f < fMin) dmin+=fMin*fMin;
				else if (0.f > fMax) dmin+=fMax*fMax;

				fMin = v3fAABBMin.z() * v3fScale.z() + v3fTranslation.z();
				fMax = v3fAABBMax.z() * v3fScale.z() + v3fTranslation.z();

				if (0.f < fMin) dmin+=fMin*fMin;
				else if (0.f > fMax) dmin+=fMax*fMax;

				return (dmin <= 1.0f);
			}

			bool ClipRectAgainstRect(const RECT& rectSource, RECT& rectToModify)
			{
				// Early detection.
				if(rectToModify.right < rectSource.left) return false;
				if(rectToModify.left > rectSource.right) return false;
				if(rectToModify.top > rectSource.bottom) return false;
				if(rectToModify.bottom < rectSource.top) return false;

				// Clip left-right
				if(rectToModify.left < rectSource.left)	rectToModify.left = rectSource.left;
				if(rectToModify.right > rectSource.right)	rectToModify.right = rectSource.right;

				// Clip top-bottom
				if(rectToModify.top < rectSource.top)	rectToModify.top = rectSource.top;
				if(rectToModify.bottom > rectSource.bottom)	rectToModify.bottom = rectSource.bottom;

				return true;
			}
		}
	}
}