#ifndef PortalArea_h__
#define PortalArea_h__

namespace Gae
{
	namespace Entities
	{
		class Camera;
		class Light;
	}
	namespace Graphics
	{
		class Mesh;

		namespace Geometry
		{
			class Frustum;
		}
	}

	namespace Renderer
	{
		class Driver;
	}

	namespace Core
	{
		namespace Universe
		{
			using namespace Gae::Graphics;

			/*! \brief Holds the model data inside the area portal.
			*/
			class PortalArea
#ifdef _DEBUG_DRAW
				:	public Gae::Renderer::IDrawDebug
#endif
			{
				GAE_CONTAINER_DECL( Gae::Entities::Light, Light );

			public:
				typedef std::vector< const Mesh* > DeqMesh;
				typedef std::vector< const Mesh* > VecMesh;
				typedef std::vector< const Portal* > VecPortal;

				PortalArea() : m_bVisited(false) {};
				~PortalArea(){};

				bool IsMeshAdded(const Mesh* pMesh) const;

				void AddMesh(const Mesh* pMesh);
				void DelMesh(const Mesh* pMesh);
				const DeqMesh& GetMeshes() const;

				void AddPortal(Portal* pPortal);
				const VecPortal& GetPortals() const;

				//int GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh) const;
				//int GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh, const Portal* pPortal) const;
				//int GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh, const RECT& rect) const;
				int GetVisibleMeshes(const Geometry::Frustum* pFrustum, const Vector3f& v3fViewerPos, VecMesh& vecMesh, const Portal* pPortal) const;

#ifdef _DEBUG_DRAW
				//const DeqMesh& GetMeshesPortal() const;
				void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif

			private:
				DeqMesh m_VecMesh;
				VecPortal m_VecPortal;	//!< Portals that connect this area with another one.

				mutable bool m_bVisited;
			};
		}
	}
}
#include "PortalArea.inl"
#endif // PortalArea_h__