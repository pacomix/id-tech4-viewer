#include "stdafx.h"

namespace Gae
{
	namespace Graphics
	{
		namespace Geometry
		{
			void Plane::SetFromPoints(const Vector3f& v3fPoint1, const Vector3f& v3fPoint2, const Vector3f& v3fPoint3)
			{
				m_v3fNormal = (v3fPoint1 - v3fPoint3).cross(v3fPoint2 - v3fPoint3).normalized();
				m_fDistance = -m_v3fNormal.dot(v3fPoint3);
			}

			void Plane::GetIntersectionSegment(const Vector3f& v3fSegmentStart, const Vector3f& v3fSegmentEnd, Vector3f& v3fOut) const
			{
#ifdef _DEBUG	
				{
					float fDot1 = GetDistanceToPlane(v3fSegmentStart);
					float fDot2 = GetDistanceToPlane(v3fSegmentEnd);

					GAE_WARNINGM((fDot1 < 0 && fDot2 >= 0) || (fDot2 < 0 && fDot1 >= 0), L"Points doesn't intersect the planes. This method requires the points lies on either sides of the plane.");
				}
#endif
				/*
				float fDotStart = GetDistanceToPlane(v3fSegmentStart);
				float fDotEnd = GetDistanceToPlane(v3fSegmentEnd);

				float fRatio = fDotStart / ((v3fSegmentStart - v3fSegmentEnd).norm());

				v3fOut = v3fSegmentStart + fRatio * (v3fSegmentEnd - v3fSegmentStart);
				*/

				float da = GetDistanceToPlane(v3fSegmentStart);   // distance plane -> point a
				float db = GetDistanceToPlane(v3fSegmentEnd);   // distance plane -> point b

				float s = da/(da-db);   // intersection factor (between 0 and 1)

				v3fOut.x() = v3fSegmentStart.x() + s*(v3fSegmentEnd.x()-v3fSegmentStart.x());
				v3fOut.y() = v3fSegmentStart.y() + s*(v3fSegmentEnd.y()-v3fSegmentStart.y());
				v3fOut.z() = v3fSegmentStart.z() + s*(v3fSegmentEnd.z()-v3fSegmentStart.z());
			}

			bool Plane::ClipLineSegment2(Vector3f& v3fSegmentStart, Vector3f& v3fSegmentEnd, Vector3f* v3fClipped, int* iClippedCount) const
			{
				float fDotStart = GetDistanceToPlane(v3fSegmentStart);
				float fDotEnd = GetDistanceToPlane(v3fSegmentEnd);

				if(fDotStart < 0.0f && fDotEnd < 0.0f) return false;

				if(fDotStart >= 0.0f && fDotEnd >= 0.0f)
				{
					*v3fClipped = v3fSegmentStart;
					(*iClippedCount)++;
				}
				else if(fDotStart >= 0.0f)
				{
					*v3fClipped = v3fSegmentStart;

					Vector3f v3fTemp;
					GetIntersectionSegment(v3fSegmentEnd, v3fSegmentStart, v3fTemp);

					*(v3fClipped + 1) = v3fTemp;
					*iClippedCount += 2;
				}
				else
				{
					Vector3f v3fTemp;
					
					GetIntersectionSegment(v3fSegmentEnd, v3fSegmentStart, v3fTemp);
					

					*v3fClipped = v3fTemp;
					(*iClippedCount)++;
				}

				return true;

			}

			void Plane::ClipPolygon(/*const*/ Vector3f* v3fTriStart, int* iVtxCount, Vector3f* v3fClipped/* = NULL*/, int* iClippedCount) const
			{
				//Vector3f* vecClippedVtx = new Vector3f[iVtxCount * 2];	// Security value. A segment line can not produce 
																		// more than 2 segment lines when it is clipped 
																		// against a plane.
				static std::vector< Vector3f > vecClippedVtx(4096);

				int iClippedIdx = 0;
				for(int i = 0; i < *iVtxCount; ++i)
				{
					ClipLineSegment2(*(v3fTriStart + i), *(v3fTriStart + ((i + 1) % (*iVtxCount))), &vecClippedVtx[iClippedIdx], &iClippedIdx);
				}

				if(NULL != v3fClipped)
				{
					memcpy(v3fClipped, &vecClippedVtx[0], iClippedIdx * 12);	// sizeof Vector3f = 3 floats = 12 bytes
					*iClippedCount = iClippedIdx;
				}
				else
				{
					memcpy(v3fTriStart, &vecClippedVtx[0], iClippedIdx * 12);	// sizeof Vector3f = 3 floats = 12 bytes
					*iVtxCount = iClippedIdx;
				}

				//GAE_DELETESAFEARRAY(vecClippedVtx);
			}
		}
	}
}