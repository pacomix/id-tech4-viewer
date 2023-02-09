#ifndef Light_h__
#define Light_h__

namespace Gae
{
	namespace Graphics
	{
		class Mesh;

		namespace Geometry
		{
			class AABB;
		}
	}

	namespace Entities
	{
		using namespace Gae::Graphics;

		class Light : public Entity3D
#ifdef _DEBUG_DRAW
			, public Gae::Renderer::IDrawDebug
#endif
		{
		public:
			typedef std::vector< const Mesh* > VecMesh;

			enum E_TYPE
			{
				LT_PARALLEL = 0,
				LT_POINT,
				LT_OMNI
			};
			Light();
			~Light();

			// IEntity Interface
			virtual bool Init();
			virtual bool Update();
			virtual bool Uninit();

			inline bool IsOn() const;
			inline void Set(bool bOn);

			inline void SetShadowVolume(const Geometry::ShadowVolume* pShadowVolume);
			inline const Geometry::ShadowVolume* GetShadowVolume() const;

			bool IsMeshAffected(const Mesh* pMesh);
			inline void AddMesh(const Mesh* pMesh);
			inline const VecMesh& GetMeshes() const;
			inline void ClearMeshes()
			{
				m_VecMesh.clear();
			}
			inline void ComputeMeshOrder();

			inline const Vector3f& GetOrigin() const;
			inline const Vector3f& GetCenter() const;
			inline const Vector3f& GetColor() const;
			inline const Vector3f& GetRadius() const;

			inline void SetOrigin(const Vector3f& v3fOrigin);
			inline void SetCenter(const Vector3f& v3fCenter);
			inline void SetColor(const Vector3f& v3fColor);
			inline void SetRadius(const Vector3f& v3fRadius);

			inline void ComputeAABB();
			inline const Gae::Graphics::Geometry::AABB& GetAABB() const;

			/*
			inline void SetDrawn(bool bDrawn = true)
			{
				m_bDrawn = bDrawn;
			}

			inline bool IsDrawn() const
			{
				return m_bDrawn;
			}
			*/

			void ComputeClippedAABB(const Entities::Camera* pCam);
			void ResetClippedAABB();
			inline const RECT* GetAABBRect() const;
			inline RECT& GetAABBRectForModification();

			void DrawVolume(const Gae::Renderer::Driver* pRenderer) const;

		protected:
			//Modifier* m_pFreeMovement;
			//bool m_bDrawn;
			E_TYPE m_eType;

			Vector3f m_v3fOrigin;
			Vector3f m_v3fCenter;
			Vector3f m_v3fColor;
			Vector3f m_v3fRadius;

		private:
			bool m_bOn;

			const Geometry::ShadowVolume* m_pShadowVolume;	//!< Shadow volume that the light creates.
			VecMesh m_VecMesh;	//!< Meshes the light affects to.
			VecMesh m_VecMeshOpaque;
			VecMesh m_VecMeshDecal;
			VecMesh m_VecMeshTranslucent;

			Gae::Graphics::Geometry::AABB m_AABB;
			RECT m_ScreenRect;	// Light's bounding box in clip coordinates. Used for scissor test.

#ifdef _DEBUG_DRAW
			void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif

		};
	}
}
#include "Light.inl"
#endif // Light_h__