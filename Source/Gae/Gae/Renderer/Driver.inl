namespace Gae
{
	namespace Renderer
	{
		void Driver::SetViewMatrix(const Matrix4f& mtx4fView)
		{
			m_mtx44fView = mtx4fView;
		}

		void Driver::SetProjectionMatrix(const Matrix4f& mtx4fProj)
		{
			m_mtx44fProj = mtx4fProj;
		}

		void Driver::SetViewProjectionMatrix(const Matrix4f& mtx4fViewProj)
		{
			m_mtx44fViewProj = mtx4fViewProj;
		}

		const Matrix4f& Driver::GetViewMatrix() const
		{
			return m_mtx44fView;
		}

		const Matrix4f& Driver::GetProjectionMatrix() const
		{
			return m_mtx44fProj;
		}

		const Matrix4f& Driver::GetViewProjectionMatrix() const
		{
			return m_mtx44fViewProj;
		}

		const Driver::VecRenderList& Driver::GetDeqRenderStatic() const
		{
			return m_DeqRenderStatic;
		}

		const Driver::VecRenderList& Driver::GetDeqRenderStaticTranslucent() const
		{
			return m_DeqRenderStaticTranslucent;
		}

		const Driver::VecRenderList& Driver::GetDeqRenderShadowStatic() const
		{
			return m_DeqRenderShadowStatic;
		}

		const Driver::VecRenderList& Driver::GetDeqRender() const
		{
			return m_DeqRender;
		}

		const Driver::VecRenderLight& Driver::GetDeqRenderLight() const
		{
			return m_DeqRenderLight;
		}
	}
}