namespace Gae
{

	namespace Entities
	{

		const Matrix4f& Camera::GetProjectionMatrix() const
		{
			return m_mtx44fProj;
		}

		const Matrix4f& Camera::GetViewMatrix() const
		{
			return m_mtx44fView;
		}

		const Matrix4f& Camera::GetViewProjectionMatrix() const
		{
			return m_mtx44fViewProj;
		}

		const Gae::Graphics::Geometry::Frustum& Camera::GetFrustum() const
		{
			return m_Frustum;
		}

		const Gae::Graphics::Geometry::Frustum* Camera::GetFrustumForClipping() const
		{
			return m_pFrustumForPortalClipping;
		}

		float Camera::GetFov() const
		{
			return m_fFOV;
		}

		float Camera::GetAspectRatio() const
		{
			return m_fAspectRatio;
		}

		float Camera::GetNearDistance() const
		{
			return m_fNear;
		}

		float Camera::GetFarDistance() const
		{
			return m_fFar;
		}
	}
}