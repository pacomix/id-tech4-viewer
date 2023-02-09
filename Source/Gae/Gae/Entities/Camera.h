#ifndef Camera_h__
#define Camera_h__

namespace Gae
{
	namespace Graphics
	{
		class Mesh;
	}

	namespace Entities
	{
		class Camera : public IEntity
		{
		public:
			Camera();
			~Camera();

			// IEntity Interface
			virtual bool Init();
			virtual bool Update();
			virtual bool Uninit();

			void SetProjectionMatrix(float fFOV, float fAspectRatio, float fNear, float fFar);

			inline const Matrix4f& GetProjectionMatrix() const;
			inline const Matrix4f& GetViewMatrix() const;
			inline const Matrix4f& GetViewProjectionMatrix() const;

			inline const Gae::Graphics::Geometry::Frustum& GetFrustum() const;
			inline const Gae::Graphics::Geometry::Frustum* GetFrustumForClipping() const;

			inline float GetFov() const;
			inline float GetAspectRatio() const;
			inline float GetNearDistance() const;
			inline float GetFarDistance() const;

		protected:
			void UpdateMatrices();

			Gae::Graphics::Geometry::Frustum m_Frustum;
			Gae::Graphics::Geometry::Frustum* m_pFrustumForPortalClipping;
			Modifier* m_pFreeMovement;

			float m_fFOV;
			float m_fAspectRatio;
			float m_fNear;
			float m_fFar;

			// Projection matrix.
			Matrix4f m_mtx44fProj;
			Matrix4f m_mtx44fView;
			Matrix4f m_mtx44fViewProj;

			// Untransformed Frustum's vertexes.
			//[0,1,2,3] FarPlane -> TopLeft,DownLeft,DownRight,TopRight
			//[4,5,6,7] NearPlane -> TopLeft,DownLeft,DownRight,TopRight
			Vector3f m_v3fVertexes[8];

		private:
			Vector3f m_v3fFrustumCenter;	// It is here 'cos if we have to calculate the frustum's center in the Frustum class
											// it would be expensive. It is used to calculate the bounding sphere of the camera's frustum.

		};
	}
}

#include "Entities/Camera.inl"

#endif // Camera_h__