#ifndef Manager_h__
#define Manager_h__

#ifdef _DEBUG
#define _DEBUG_MESHES
#endif

namespace Gae
{
	namespace Entities
	{
		class Entity3D;
		class Camera;
		class Light;
	}

	namespace Graphics
	{
		class MultiMesh;
		class Mesh;

		namespace Geometry
		{
			class ShadowVolume;
		}
	}

	namespace Core
	{
		namespace Universe
		{
			class PortalArea;

			using namespace Gae::Entities;
			using namespace Gae::Graphics;

			class Manager
			{

				typedef std::vector< Entity3D* > VecEntities3D;
				typedef std::vector< MultiMesh* > VecStaticGeometry;
				typedef std::vector< const Mesh* > VecMesh;
				typedef std::vector< PortalArea* > VecPortalArea;
				//typedef std::vector< Camera* > VecCameras;

				// Testing the macro for general containers.
				GAE_CONTAINER_DECL( Gae::Entities::Camera, Camera );
				GAE_CONTAINER_DECL( Gae::Graphics::Geometry::ShadowVolume, ShadowVolume );
				GAE_CONTAINER_DECL( Gae::Entities::Light, Light );

				SINGLETON_DECL(Manager);



			public:
				void Update();

				// TODO: Delete this method and make a CreateEntity3D with a file as input.
				// TODO: Implement in the add/create methods the sorting for material, alpha and blablabla
				void AddEntity3D(Entity3D* pEntity);
				void AddStaticMultiMesh(MultiMesh* pMultiMesh);
				void AddStaticMultiMesh(MultiMesh* pMultiMesh, UINT32 uPortalArea);

				/*	\brief	Adds a Mesh object to the universe manager.
					\param[in]	Mesh*	Mesh to be added.
					\note	This method try to look a valid area for the mesh. The mesh 
						will be initialized whether if a valid area was found for the mesh 
						or not. This is carried out in this way because it is possible another 
						entity (like in example a Light object)	could have a reference to 
						this mesh and thus the mesh could be send to the renderer.
				*/
				void AddStaticMesh(Mesh* pMesh);
				void AddStaticMesh(Mesh* pMesh, UINT32 uPortalArea);
				void AddShadowModel(Geometry::ShadowVolume* pShadowModel);
				bool AddLightIntoCorrectArea(Entities::Light* pLight);

				inline Gae::Entities::Camera& GetActiveCamera() const;

				void CreatePortalAreas(UINT32 uCount);
				void SetPortalAreaDetermination(IPortalAreaDetermination* pPortalAreaDetermination);
				const IPortalAreaDetermination* GetPortalAreaDetermination() const;
				void AddPortal(Portal* pPortal);

			private:

				struct Stats
				{
					int m_iTriangles;
					int m_iShadowVolumes;
					int m_iLights;
					int m_iMeshes;
				};
				void AddMeshToArea(Mesh* pMesh);
				void ActivateCamera(Camera* pCam);

				// Input callbacks
				void KeyPressed(UINT uKeyCode);		//!< DIK_xxxxx

				VecEntities3D m_VecEntities3D;
				VecStaticGeometry m_VecStaticGeometry;

				Gae::Entities::Camera* m_pCameraActive;

				VecPortalArea m_VecPortalArea;
				UINT32 m_uCurrentPortalArea;
				IPortalAreaDetermination* m_pPortalAreaDetermination;
				Gae::Renderer::Driver* m_pRenderer;

#ifdef _DEBUG_DRAW
				#define VD_FRUSTUM		0x00000001
				#define VD_AABB			0x00000002
				#define VD_PORTAL		0x00000004
				#define VD_SHADOWVOL	0x00000008
				#define VD_LIGHTVOL		0x00000010

				UINT32 m_uVisualDebug;	//!< Flags for Visual Debug output.
			public:
				inline void VisualDebugSet(UINT32 uFlag)
				{
					m_uVisualDebug |= uFlag;
				}

				inline void VisualDebugUnset(UINT32 uFlag)
				{
					m_uVisualDebug &= ~uFlag;
				}

				inline void VisualDebugToggle(UINT32 uFlag)
				{
					m_uVisualDebug ^= uFlag;
				}

				inline const bool VisualDebugEnabled(UINT32 uFlag) const
				{
					return (m_uVisualDebug & uFlag) > 0;
				}
#endif
			private:
#ifdef _DEBUG_MESHES
				bool m_bDebugMeshes;
				int m_iIdxMeshDebug;
				Gae::Entities::Camera* m_pCameraCulling;
				bool m_bDebugLights;
				int m_iIdxLightDebug;
#endif
				bool m_bDebugShadows;
				int m_iIdxShadowDebug;
				VecMesh m_VecAllMeshes;
				//bool m_bUseLightScissor;
			};
		}
	}
}

#include "Core/Universe/Manager.inl"

#endif // Manager_h__