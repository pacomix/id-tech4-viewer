#ifndef OpenGL_h__
#define OpenGL_h__

namespace Gae
{
	namespace Renderer
	{
		class OpenGL : public Driver
		{
			typedef Driver super;

			SINGLETON_DECL(OpenGL);

		protected:
			void SetViewMatrix(const Matrix4f& mtx4fView);
			void SetProjectionMatrix(const Matrix4f& mtx4fView);
			void Render();
			bool InitMesh(Gae::Graphics::Mesh* pMesh);
			bool UninitMesh(Gae::Graphics::Mesh* pMesh);

			GAE_DEBUG_FN_DECL(virtual void DrawDebug(const E_DEBUG_SHAPE eShape, const Gae::Graphics::Node* pNode, const DWORD dwColor) const);

		private:
			HDC m_hDevContext;
			HGLRC m_hRenderContext;
		};
	}
}

#endif // OpenGL_h__