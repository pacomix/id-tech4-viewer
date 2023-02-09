#include "stdafx.h"

namespace Gae
{
	namespace Entities
	{
		Camera::Camera()
		{
			m_pFrustumForPortalClipping = new Gae::Graphics::Geometry::Frustum(4);
			m_fFOV = 0.0f;
			m_fAspectRatio = 0.0f;
			m_fNear = 0.0f;
			m_fFar = 0.0f;
		}

		bool Camera::Init()
		{
			m_pFreeMovement = new Gae::Gifts::Modifiers::FreeMovement;
			m_pFreeMovement->Init(this);

			m_mtx44fProj = Matrix4f::Zero();
			m_mtx44fView = Matrix4f::Identity();
			m_mtx44fViewProj = Matrix4f::Zero();

#ifdef _DEBUG
			static bool bFirst = true;
			SetProjectionMatrix(D3DXToRadian(45), 16.0f/9.0f, 1.0f, bFirst ? 2000.0f : 8000.0f);

			if(bFirst) bFirst = false;

#else
			SetProjectionMatrix(D3DXToRadian(75), 16.0f/9.0f, 1.0f, 40000.0f);
#endif

			return TRUE;
		}

		bool Camera::Update()
		{
			m_pFreeMovement->Run();

			if (!IsDirty()) return false;

			{
				UpdateMatrices();
				m_Frustum.SetFromMatrixViewProjection(m_mtx44fViewProj);
				Vector4f v4fFrustumCenter(m_v3fFrustumCenter.x(), m_v3fFrustumCenter.y(), m_v3fFrustumCenter.z(), 1.0f);
				m_Frustum.GetBoundingSphere().SetCenter((GetWorldTransform() * v4fFrustumCenter).block(0,0,3,1));

				m_pFrustumForPortalClipping->SetPlane(0, m_Frustum.GetPlane(0).m_v4fNormalDistance);
				m_pFrustumForPortalClipping->SetPlane(1, m_Frustum.GetPlane(1).m_v4fNormalDistance);
				m_pFrustumForPortalClipping->SetPlane(2, m_Frustum.GetPlane(2).m_v4fNormalDistance);
				m_pFrustumForPortalClipping->SetPlane(3, m_Frustum.GetPlane(3).m_v4fNormalDistance);
				
#ifdef _DEBUG
				{	// Transform the frustum's vertexes to camera's world space
					Vector4f v4fVertexes[8];
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_FAR, Gae::Graphics::Geometry::Frustum::PL_TOP, Gae::Graphics::Geometry::Frustum::PL_LEFT, reinterpret_cast<Vector3f*>(&v4fVertexes[0]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_FAR, Gae::Graphics::Geometry::Frustum::PL_LEFT, Gae::Graphics::Geometry::Frustum::PL_BOTTOM, reinterpret_cast<Vector3f*>(&v4fVertexes[1]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_FAR, Gae::Graphics::Geometry::Frustum::PL_BOTTOM, Gae::Graphics::Geometry::Frustum::PL_RIGHT, reinterpret_cast<Vector3f*>(&v4fVertexes[2]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_FAR, Gae::Graphics::Geometry::Frustum::PL_RIGHT, Gae::Graphics::Geometry::Frustum::PL_TOP, reinterpret_cast<Vector3f*>(&v4fVertexes[3]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_NEAR, Gae::Graphics::Geometry::Frustum::PL_TOP, Gae::Graphics::Geometry::Frustum::PL_LEFT, reinterpret_cast<Vector3f*>(&v4fVertexes[4]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_NEAR, Gae::Graphics::Geometry::Frustum::PL_LEFT, Gae::Graphics::Geometry::Frustum::PL_BOTTOM, reinterpret_cast<Vector3f*>(&v4fVertexes[5]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_NEAR, Gae::Graphics::Geometry::Frustum::PL_BOTTOM, Gae::Graphics::Geometry::Frustum::PL_RIGHT, reinterpret_cast<Vector3f*>(&v4fVertexes[6]));
					m_Frustum.GetIntersection(Gae::Graphics::Geometry::Frustum::PL_NEAR, Gae::Graphics::Geometry::Frustum::PL_RIGHT, Gae::Graphics::Geometry::Frustum::PL_TOP, reinterpret_cast<Vector3f*>(&v4fVertexes[7]));

					m_Frustum.SetVertexes(v4fVertexes);
				}
#endif
			}
			return true;
		}

		bool Camera::Uninit()
		{
			m_pFreeMovement->Uninit();
			return TRUE;
		}

		void Camera::UpdateMatrices()
		{
			m_mtx44fView = GetWorldTransform().inverse();

			m_mtx44fViewProj = m_mtx44fProj * m_mtx44fView;
		}

		void Camera::SetProjectionMatrix(float fFOV, float fAspectRatio, float fNear, float fFar)
		{
			/* Column-major. Left-handed perspective projection matrix				|	h = cot(fFOV/2);
				w			0			0					0						|	w = h / fAspectRatio.
				0			h			0					0						|	fFOV is in radians
				0			0	fFar/(fFar-fNear)	-((fFar*fNear)/(fFar-fNear))	|	fFar is far view-plane
				0			0			1					0						|	fNear is near view-plane
			*/

			m_fFOV = fFOV;
			m_fAspectRatio = fAspectRatio;
			m_fNear = fNear;
			m_fFar = fFar;

			float fTan = tan(fFOV * 0.5f);

			m_mtx44fProj(1,1) = 1.0f / fTan;	// Cotan(fFOV / 2.0f)
			m_mtx44fProj(0,0) = m_mtx44fProj(1,1) / fAspectRatio;			
			m_mtx44fProj(2,2) = fFar / (fFar - fNear);
			m_mtx44fProj(3,2) = 1.0f;
			m_mtx44fProj(2,3) = -((fFar * fNear) / (fFar - fNear));

			/*
			// Infinite perspective matrix
			m_mtx44fProj(1,1) = 1.0f / fTan;	// Cotan(fFOV / 2.0f)
			m_mtx44fProj(0,0) = m_mtx44fProj(1,1) / fAspectRatio;			
			m_mtx44fProj(2,2) = 1.0f - 0.001f;//fFar / (fNear - fFar);
			//m_mtx44fProj(2,2) = 1.0f;//fFar / (fNear - fFar);
			m_mtx44fProj(2,3) = -1.0f;
			m_mtx44fProj(3,2) = fNear;
			*/
 
			/*
			m_mtx44fProj(1,1) = 1.f / fAspectRatio * ( cosf(fFOV/2.f)/sinf(fFOV/2.f) );
			m_mtx44fProj(0,0) = cosf(fFOV/2.0f) / sinf(fFOV/2.0f);
			m_mtx44fProj(2,2) = fFar / (fFar - fNear);
			m_mtx44fProj(2,3) = 1.0f;
			m_mtx44fProj(3,2) = -(fFar / (fFar - fNear)) * fNear;
			*/

			// Calculate the center of the frustum and the radius for the bounding sphere.
			Vector3f v3fZAxis(GetWorldTransform()[8], GetWorldTransform()[9], GetWorldTransform()[10]);	// Cam's Z-Axis
			Vector3f v3fCenter(v3fZAxis * (fNear + (((v3fZAxis * fFar) - (v3fZAxis * fNear)).norm() * 0.5f)));
			Vector3f v3fTopRightFarPlaneVtx((fTan * fFar) / fAspectRatio, fTan * fFar, fFar);
			float fRadius = (v3fTopRightFarPlaneVtx - v3fCenter).norm();
			m_Frustum.GetBoundingSphere().SetValues(v3fCenter, fRadius);
			m_v3fFrustumCenter = v3fCenter;

			Update();
			/*
#ifdef _DEBUG
			{	// Compute the 8 vertexes of the frustum.
				// Lets start with the 4 four points of the FarPlane.
				const Vector3f& v3fCamXAxis = GetWorldTransform().col(0).block(0,0,3,1);
				const Vector3f& v3fCamYAxis = GetWorldTransform().col(1).block(0,0,3,1);
				const Vector3f& v3fCamZAxis = GetWorldTransform().col(2).block(0,0,3,1);
				const Vector3f& v3fCamPos = GetWorldTransform().col(3).block(0,0,3,1);

				// Calculate some useful values.
				Matrix3f mtx33fAxis = Matrix3f::Identity();

				Vector3f v3fFarPlaneCenter = mtx33fAxis.col(2) * fFar;
				Vector3f v3fFarPlaneHeightDiv2 = mtx33fAxis.col(1) * (tan(fFOV * 0.5f) * fFar);
				Vector3f v3fFarPlaneWidthDiv2 = mtx33fAxis.col(0) * ((tan(fFOV * 0.5f) * fFar) * fAspectRatio);

				Vector3f v3fNearPlaneCenter = mtx33fAxis.col(2) * fNear;
				Vector3f v3fNearPlaneHeightDiv2 = mtx33fAxis.col(1) * (tan(fFOV * 0.5f) * fNear);
				Vector3f v3fNearPlaneWidthDiv2 = mtx33fAxis.col(0) * ((tan(fFOV * 0.5f) * fNear) * fAspectRatio);

				// Far Plane vertexes.
				m_v3fVertexes[0] = v3fFarPlaneCenter + v3fFarPlaneHeightDiv2 - v3fFarPlaneWidthDiv2;
				m_v3fVertexes[1] = m_v3fVertexes[0] - (v3fFarPlaneHeightDiv2 * 2.0f);
				m_v3fVertexes[2] = m_v3fVertexes[1] + (v3fFarPlaneWidthDiv2 * 2.0f);
				m_v3fVertexes[3] = m_v3fVertexes[2] + (v3fFarPlaneHeightDiv2 * 2.0f);

				// Near Plane vertexes
				m_v3fVertexes[4] = v3fNearPlaneCenter + v3fNearPlaneHeightDiv2 - v3fNearPlaneWidthDiv2;
				m_v3fVertexes[5] = m_v3fVertexes[4] - (v3fNearPlaneHeightDiv2 * 2.0f);
				m_v3fVertexes[6] = m_v3fVertexes[5] + (v3fNearPlaneWidthDiv2 * 2.0f);
				m_v3fVertexes[7] = m_v3fVertexes[6] + (v3fNearPlaneHeightDiv2 * 2.0f);

			}
#endif
			*/
		}

		Camera::~Camera()
		{
			GAE_DELETESAFE(m_pFrustumForPortalClipping);
			GAE_DELETESAFE(m_pFreeMovement);
		}
	}
}