#include "stdafx.h"
#include "Graphics/Geometry/Intersections.h"

namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			using namespace Gae::Entities;
			using namespace Gae::Graphics;

			GAE_CONTAINER_IMPL( Manager, Gae::Entities::Camera, Camera );
			GAE_CONTAINER_IMPL( Manager, Gae::Graphics::Geometry::ShadowVolume, ShadowVolume );
			GAE_CONTAINER_IMPL( Manager, Gae::Entities::Light, Light );

			SINGLETON_IMPL(Manager);

			bool Manager::Init()
			{

				m_pPortalAreaDetermination = NULL;
				m_pCameraActive = NULL;
				m_pRenderer = Gae::Core::Application::Singleton().GetRenderer();

				//m_bUseLightScissor = true;

				// Register the input callbacks
				GAE_INPUT_REGISTER_EVENT(OnKeyPress, this, &Manager::KeyPressed);

#ifdef _DEBUG_DRAW
				//m_uVisualDebug = 0xFFFFFFFF;
				m_uVisualDebug = 0x00000000;
#endif
#ifdef _DEBUG_MESHES
				m_bDebugMeshes = false;
				m_iIdxMeshDebug = 0;
				m_pCameraCulling = NULL;

				m_bDebugLights = false;
				m_iIdxLightDebug = 0;
#endif
				m_iIdxShadowDebug = 0;
				m_bDebugShadows = false;

				return TRUE;
			}

			void Manager::Update()
			{
				static Stats stats;
				stats.m_iLights = 0;
				stats.m_iShadowVolumes = 0;
				stats.m_iTriangles = 0;
				stats.m_iMeshes = 0;

				GAE_ASSERTM(!m_VecCamera.empty(), L"There are no cameras in the universe!!!");

				// Select a default active camera if we do not have one yet.
				GAE_WARNINGM(m_pCameraActive, L"A camera was not active. Setting a the first one in the list!");
				if(!m_pCameraActive)
				{
					ActivateCamera(m_VecCamera.at(0));
				}
				
				// first thing... the camera
				#ifdef _DEBUG_MESHES
					// If we are debugging the meshes the camera that makes 
					// the object's frustum culling is not the active camera.
					if(m_pCameraActive != m_pCameraCulling)
					{
						m_pCameraCulling->Update();
					}
				#endif

				static PortalArea::VecMesh vecMesh(1024);


				for each(Entities::Entity3D* pEnt in m_VecEntities3D)
				{
					pEnt->Update();
				}

				if(m_pCameraActive->Update())
				{
					vecMesh.clear();
					#ifdef _DEBUG_DRAW
						m_pRenderer->DrawDebugClear();
					#endif

					// Determine in which area the camera lies on. If no area is defined do nothing.
					GAE_WARNINGM(!m_VecPortalArea.empty(), L"No areas defined yet. Empty universe!");
					if(!m_VecPortalArea.empty()/* && NULL != m_pPortalAreaDetermination*/)
					{
						static Vector3f v3fLastCamPos = m_pCameraActive->GetWorldPosition();
						int iAreaCam = 0;

						// If we have a valid object to get the area of a 3D point in the universe get it!
						if(m_pPortalAreaDetermination)
						{
							#ifdef _DEBUG_MESHES
								iAreaCam = m_pPortalAreaDetermination->GetAreaPortal(m_pCameraCulling->GetWorldPosition());
							#else
								iAreaCam = m_pPortalAreaDetermination->GetAreaPortal(m_pCameraActive->GetWorldPosition());
							#endif
						}
						
						// If the camera was outside of any valid area try getting the last position
						// in order to see if we tried to go out of the area.
						if(iAreaCam < 0)
						{
							m_pCameraActive->SetPosition(v3fLastCamPos);
							iAreaCam = m_pPortalAreaDetermination->GetAreaPortal(v3fLastCamPos);

							// If the camera was outside of any valid area just add the whole areas to the render list.
							if(iAreaCam < 0)
							{
								// Set the initial area and add the rest.
								m_pRenderer->SetRenderStatic(m_VecPortalArea.at(0)->GetMeshes() );
								for(iAreaCam = 1; iAreaCam < (int)m_VecPortalArea.size(); ++iAreaCam)
								{
									m_pRenderer->AddToRenderStatic(m_VecPortalArea.at(iAreaCam)->GetMeshes());
								}
							}
						}

						// We finally have the camera settled in a valid position so now we can transfer
						// the needed properties to the Renderer.
						m_pRenderer->SetViewMatrix(m_pCameraActive->GetViewMatrix());
						m_pRenderer->SetProjectionMatrix(m_pCameraActive->GetProjectionMatrix());
						m_pRenderer->SetViewProjectionMatrix(m_pCameraActive->GetViewProjectionMatrix());

						// Gets the visible meshes in the area and add it to the render queue.
						//Gae::Renderer::Driver::DeqRenderList deqMesh;
						if(iAreaCam >= 0)
						{
							RECT rect;
							rect.top = 0L;
							rect.bottom = (LONG)Gae::Core::ApplicationConfig::m_RendererConfig.m_dwHeight;
							rect.left = 0L;
							rect.right = (LONG)Gae::Core::ApplicationConfig::m_RendererConfig.m_dwWidth;

							if(::GetAsyncKeyState('L'))
							{
								vecMesh = m_VecAllMeshes;
								stats.m_iTriangles = 0;
								for each(const Mesh* pMesh in m_VecAllMeshes)
								{
									stats.m_iTriangles += pMesh->GetIndexBuffer()->GetCount() / 3;
								}
							}
							else
							{
	#ifdef _DEBUG_MESHES
								//int iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(*m_pCameraCulling, vecMesh);
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraCulling->GetFrustumForClipping(), m_pCameraCulling->GetWorldPosition(), vecMesh);							
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraCulling->GetFrustumForClipping(), m_pCameraCulling->GetWorldPosition(), vecMesh, NULL);
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraCulling->GetFrustumForClipping(), m_pCameraCulling->GetWorldPosition(), vecMesh, rect);
								stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraCulling->GetFrustumForClipping(), m_pCameraCulling->GetWorldPosition(), vecMesh, NULL);
	#else
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(*m_pCameraActive, vecMesh);
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraActive->GetFrustumForClipping(), m_pCameraActive->GetWorldPosition(), vecMesh);
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraActive->GetFrustumForClipping(), m_pCameraActive->GetWorldPosition(), vecMesh, NULL);
								//stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraActive->GetFrustumForClipping(), m_pCameraActive->GetWorldPosition(), vecMesh, rect);
								stats.m_iTriangles = m_VecPortalArea.at(iAreaCam)->GetVisibleMeshes(m_pCameraActive->GetFrustumForClipping(), m_pCameraActive->GetWorldPosition(), vecMesh, NULL);
	#endif
							}

	#ifdef _DEBUG_MESHES
							if(m_bDebugMeshes)
							{
								if(m_iIdxMeshDebug >= (int)vecMesh.size())
								{
									m_iIdxMeshDebug = 0;
									m_bDebugMeshes = false;
								}
								const Mesh* pMesh = vecMesh.at(m_iIdxMeshDebug);
								vecMesh.clear();
								vecMesh.push_back(pMesh);	
							}
	#endif
							m_pRenderer->SetRenderStatic(vecMesh);	// Set the render list with the visible meshes.
							stats.m_iMeshes = (int) vecMesh.size();

							// Populate the lights based on the visible meshes affected by them.
							Gae::Renderer::Driver::VecRenderLight deqLight;
							for each(Entities::Light* pLight in m_VecLight)
							{
								pLight->ClearMeshes();
							}

							for each(const Entities::Mesh* pMesh in vecMesh)
							{
								for each(Entities::Light* pLight in pMesh->GetVecLight())
								{
									pLight->AddMesh(pMesh);

									// Avoid adding a light more than once
									if(std::find(deqLight.begin(), deqLight.end(), pLight) == deqLight.end())
									{
	#ifdef _DEBUG
										if(m_pCameraCulling->GetFrustum().IsInside(pLight->GetAABB()))
	#else
										if(m_pCameraActive->GetFrustum().IsInside(pLight->GetAABB()))
	#endif
										{
											// TODO: This can be improved by clipping later the clipped light's 
											// AABB against the clipped mesh's AABB.
											pLight->ComputeClippedAABB(m_pCameraActive);

											//pLight->SetDrawn(false);
											deqLight.push_back(pLight);
										}
									}
								}
							}

	#ifdef _DEBUG_MESHES
							if(m_bDebugLights)
							{
								if(m_iIdxLightDebug >= (int)deqLight.size())
								{
									m_iIdxLightDebug = 0;
									m_bDebugLights = false;
								}
								const Entities::Light* pLight = deqLight.at(m_iIdxLightDebug);
								deqLight.clear();
								deqLight.push_back(pLight);
							}
	#endif
							stats.m_iLights = (int)deqLight.size();
							m_pRenderer->SetRenderLight(deqLight);

							// Now populate the shadows of each light.
							Gae::Renderer::Driver::VecRenderList deqShadowVol;
							for each(const Entities::Light* pLight in deqLight)
							{
								if(pLight->GetShadowVolume() != NULL)
								{
									deqShadowVol.push_back(pLight->GetShadowVolume()->GetMesh());
								}
							}
							m_pRenderer->SetRenderShadowStatic(deqShadowVol);
							stats.m_iShadowVolumes = (int)deqShadowVol.size();

							SetWindowText(
								Gae::Core::ApplicationConfig::m_RendererConfig.m_hWnd, 
								Gae::Core::Util::LogString(L"Lights: [%i]\tShadows: [%i]\tMeshes: [%i]\tTris: [%i]",
								stats.m_iLights, stats.m_iShadowVolumes, stats.m_iMeshes, stats.m_iTriangles).c_str()
								);
						}


						v3fLastCamPos = m_pCameraActive->GetWorldPosition();

	#ifdef _DEBUG_DRAW
						if(VisualDebugEnabled(VD_PORTAL))
						{	// Add the portals to be rendered.
							m_pRenderer->AddToDrawDebug(m_VecPortalArea.at(iAreaCam));
							m_VecPortalArea.at(iAreaCam)->SetDrawDebug(true);

							/*
							for each(const Portal* pPortal in m_VecPortalArea.at(iAreaCam)->GetPortals())
							{
								m_pRenderer->AddToDrawDebug(pPortal);
							}
							*/
						}

						if(VisualDebugEnabled(VD_FRUSTUM))
						{	// Add the camera's Frustum
							for each(Camera* pCam in m_VecCamera)
							{
								if(pCam != m_pCameraActive)
								{
									m_pRenderer->AddToDrawDebug(const_cast<Gae::Graphics::Geometry::Frustum*>(&pCam->GetFrustum()));
									const_cast<Gae::Graphics::Geometry::Frustum*>(&pCam->GetFrustum())->SetDrawDebug(true);
								}
							}

							// Adds the clipped frustum of each portal.
							const PortalArea::VecPortal& vecPortal = m_VecPortalArea.at(iAreaCam)->GetPortals();
							for(UINT32 i = 0; i < vecPortal.size(); ++i)
							{
								m_pRenderer->AddToDrawDebug(vecPortal.at(i)->GetClippedFrustum());
								(const_cast<Graphics::Geometry::Frustum*>(vecPortal.at(i)->GetClippedFrustum()))->SetDrawDebug(true);
							}
						}

						if(VisualDebugEnabled(VD_AABB))
						{	// Add the AABB of the meshes
							/*
							for each(const Mesh* pMesh in deqMesh)
							{
								(const_cast<Graphics::Geometry::AABB*>(&pMesh->GetAABB()))->SetDrawDebug(true);
								m_pRenderer->AddToDrawDebug(&pMesh->GetAABB());
							}
							*/
							for each(const Entities::Light* pLight in m_pRenderer->GetDeqRenderLight())
							{
								for each(const Mesh* pMesh in pLight->GetMeshes())
								{
									m_pRenderer->AddToDrawDebug(pMesh);
								}
							}
						}

						if(VisualDebugEnabled(VD_LIGHTVOL))
						{	// Add the AABB of the visible lights
							const Gae::Renderer::Driver::VecRenderLight& deqRenderLight = m_pRenderer->GetDeqRenderLight();
							for each(const Gae::Entities::Light* pLight in deqRenderLight)
							{
								//m_pRenderer->AddToDrawDebug(&pLight->GetAABB());
								(const_cast<Entities::Light*>(pLight))->SetDrawDebug(true);
								m_pRenderer->AddToDrawDebug(pLight);
							}
						}
	#endif
					}
				}
			}

			void Manager::AddShadowModel(Geometry::ShadowVolume* pShadowModel)
			{
				GAE_ASSERTM(NULL != pShadowModel, L"ShadowModel is NULL!!!");

				m_pRenderer->InitMesh(const_cast<Mesh*>(pShadowModel->GetMesh()));
				AddShadowVolume(pShadowModel);
			}

			bool Manager::AddLightIntoCorrectArea(Entities::Light* pLight)
			{
				GAE_ASSERTM(NULL != pLight, L"Invalid light! NULL!!!");
				GAE_WARNINGM(NULL != m_pPortalAreaDetermination, L"You must assign a valid Portal Area Determination objecto to the Universe::Manager prior calling this method!");

				int iLightArea = m_pPortalAreaDetermination ? m_pPortalAreaDetermination->GetAreaPortal(pLight->GetOrigin()) : 0;
				//GAE_ASSERTM(iLightArea != -1, L"Light is not located in any valid area!");
				if(iLightArea != -1)
				{
					m_VecPortalArea.at(iLightArea)->AddLight(pLight);
					return true;
				}
				else
				{
					iLightArea = m_pPortalAreaDetermination->GetAreaPortal(pLight->GetOrigin() + pLight->GetCenter());
					if(iLightArea != -1)
					{
						m_VecPortalArea.at(iLightArea)->AddLight(pLight);
						return true;
					}
					else
					{
						iLightArea = m_pPortalAreaDetermination->GetAreaPortal(pLight->GetOrigin() - pLight->GetCenter());
						if(iLightArea != -1)
						{
							m_VecPortalArea.at(iLightArea)->AddLight(pLight);
							return true;
						}
					}
				}
				return false;
			}

			void Manager::AddEntity3D(Gae::Entities::Entity3D* pEntity)
			{
				GAE_ASSERT(NULL != pEntity);

				VecEntities3D::iterator itFound = std::find(m_VecEntities3D.begin(), m_VecEntities3D.end(), pEntity);

				if(itFound != m_VecEntities3D.end()) return;

				m_VecEntities3D.push_back(pEntity);
			}

			void Manager::AddStaticMultiMesh(Gae::Graphics::MultiMesh* pMultiMesh, UINT32 uPortalArea)
			{
				GAE_ASSERTM(uPortalArea < m_VecPortalArea.size(), L"Portal area doesn't exists!");
				GAE_ASSERT(NULL != pMultiMesh);

				for(Gae::Graphics::MultiMesh::VecMeshes::const_iterator it = pMultiMesh->GetMeshes().begin(), itEnd = pMultiMesh->GetMeshes().end(); it != itEnd; ++it)
				{
					AddStaticMesh(*it, uPortalArea);
				}
			}

			void Manager::AddStaticMultiMesh(Gae::Graphics::MultiMesh* pMultiMesh)
			{
				GAE_ASSERT(NULL != pMultiMesh);
				GAE_ASSERTM(std::find(m_VecStaticGeometry.begin(), m_VecStaticGeometry.end(), pMultiMesh) == m_VecStaticGeometry.end(), L"MultiMesh was already added!!!");

				m_VecStaticGeometry.push_back(pMultiMesh);

				//GAE_TRACE(L"Adding MultiMesh: [%s]\n", pMultiMesh->m_wsTAG.c_str());
				GAE_TRACE(L"Adding MultiMesh: [%s]\n", pMultiMesh->GetTAG().c_str());

				if(m_VecPortalArea.empty() || NULL == m_pPortalAreaDetermination)
				{	// Create one default area if there are no portals/areas created.
					// TODO: Finish this branch.
					m_VecPortalArea.push_back(new PortalArea);
				}
				else
				{	// We have portals and areas defined. Calculate where to put each model's mesh.
					using namespace Gae::Graphics;
					for(MultiMesh::VecMeshes::const_iterator it = pMultiMesh->GetMeshes().begin(), itEnd = pMultiMesh->GetMeshes().end(); it != itEnd; ++it)
					{
						AddStaticMesh(*it);
					}
				}
			}

			void Manager::AddStaticMesh(Gae::Graphics::Mesh* pMesh, UINT32 uPortalArea)
			{
				GAE_ASSERTM(uPortalArea < m_VecPortalArea.size(), L"Portal area doesn't exists!");
				GAE_ASSERT(NULL != pMesh);

				m_VecAllMeshes.push_back(pMesh);
				if(!m_VecPortalArea.at(uPortalArea)->IsMeshAdded(pMesh))
				{
					m_VecPortalArea.at(uPortalArea)->AddMesh(pMesh);
				}

				Gae::Renderer::Driver& renderer = *Gae::Core::Application::Singleton().GetRenderer();

				if(!pMesh->GetVertexBuffer()->IsInitialized() && !pMesh->GetIndexBuffer()->IsInitialized())	// Only initializes the mesh if it was not initialized before.
				{
					m_pRenderer->InitMesh(pMesh);
				}
			}

			void Manager::AddStaticMesh(Gae::Graphics::Mesh* pMesh)
			{	
				m_VecAllMeshes.push_back(pMesh);
				AddMeshToArea(pMesh);
				Gae::Renderer::Driver& renderer = *Gae::Core::Application::Singleton().GetRenderer();
				m_pRenderer->InitMesh(pMesh);
			}

			void Manager::AddMeshToArea(Gae::Graphics::Mesh* pMesh)
			{
				// TODO: Add those mesh that doesn't pertain to any area to a common mesh vector in order be always taken in account.
				GAE_ASSERT(NULL != pMesh);

				char* pVertexData = (char*)pMesh->GetVertexBuffer()->GetData();
				size_t stVertexSize = pMesh->GetVertexBuffer()->GetSizeVertex();
				UINT32 uVertexCount = (UINT32)(pMesh->GetVertexBuffer()->GetSizeData() / stVertexSize);

#ifdef _DEBUG
				static int iCount = 0;
				//GAE_TRACE(L"\tMesh[%i]: [%s]\n", iCount++, pMesh->m_wsTAG.c_str());
				GAE_TRACE(L"\tMesh[%i]: [%s]\n", iCount++, pMesh->GetTAG().c_str());
				bool bAdded = false;
#endif

				for(UINT32 i = 0; i < uVertexCount; ++i, pVertexData += stVertexSize)
				{	// Iterate through the vertexes for determining the area of the mesh.
					Vector3f v3fVertex;
					v3fVertex << *((float*)pVertexData), *((float*)(pVertexData + 4)), *((float*)(pVertexData + 8));

					int iArea = m_pPortalAreaDetermination->GetAreaPortal(v3fVertex);
					//GAE_ASSERTM(iArea >= 0, L"The vertex is not inside any area!!! PANIC!!!!");

					//GAE_ASSERTM(iArea == m_pPortalAreaDetermination->GetAreaPortal(v3fVertex), L"Panic!!! The mesh are in more than one area at the same time!!!");
					//GAE_ASSERT(iArea == -1);

					{	// Add the mesh to the area where the vertex lies on.

						if(iArea >= 0)
						{
							//once we added the mesh ignore the rest of vertexes!!!!
							// TODO: This is not always true. A mesh can be in the middle of two areas (or more). Add the mesh to both areas and modify the portal traversing in order to take this in account.
						#ifdef _DEBUG
							bAdded = true;
						#endif // _DEBUG
							GAE_ASSERTM((int)m_VecPortalArea.size() > iArea, L"The area is not defined in the area vectors! Did you forget to create the areas before?");
							if(!m_VecPortalArea.at(iArea)->IsMeshAdded(pMesh))
							{
								m_VecPortalArea.at(iArea)->AddMesh(pMesh);
							}
						}
					}
				}

				GAE_ASSERTM(bAdded, L"Mesh was not added to any area!");
			}

			void Manager::KeyPressed(UINT uKeyCode)
			{
				static int iIdxCam = 0;
				if(DIK_TAB == uKeyCode)
				{
					Gae::Entities::Camera* pCam = GetCamera(++iIdxCam % (int)m_VecCamera.size());
					ActivateCamera(pCam);
				}
				/*
				else if(DIK_M == uKeyCode)
				{
					m_bUseLightScissor = !m_bUseLightScissor;
				}
				*/
				/*	Temporarily disabled for Trinigy
				else if(DIK_MULTIPLY == uKeyCode)
				{
					if(m_bDebugShadows)
					{
						m_iIdxShadowDebug++;
						m_iIdxShadowDebug %= m_VecShadowVolume.size();
					}
					else
					{
						m_bDebugShadows = true;
					}
				}
				else if(DIK_DIVIDE == uKeyCode)
				{
					m_bDebugShadows = false;
				}
				*/
				
#ifdef _DEBUG_MESHES
				else if(DIK_CAPSLOCK == uKeyCode)
				{
					if(m_bDebugMeshes)
					{
						m_iIdxMeshDebug++;
					}
					else
					{
						m_bDebugMeshes = true;
						m_bDebugLights = false;
						m_iIdxMeshDebug = 0;
					}
				}
				else if(DIK_LCONTROL == uKeyCode)
				{
					if(m_bDebugLights)
					{
						m_iIdxLightDebug++;
					}
					else
					{
						m_bDebugMeshes = false;
						m_bDebugLights = true;
						m_iIdxLightDebug = 0;
					}
				}
				else if(DIK_LSHIFT == uKeyCode)
				{
					static int iIdxCullingCamera = 0;

					m_pCameraCulling = GetCamera(++iIdxCullingCamera % (int)m_VecCamera.size());
				}
#endif
#ifdef _DEBUG_DRAW
				// Temporarily disabled for Trinigy
				else if(DIK_0 == uKeyCode)
				{	// Toggle visual debugging of the frustum
					VisualDebugToggle(VD_FRUSTUM);
				}
				else if(DIK_9 == uKeyCode)
				{	// Toggle visual debugging of the AABB's
					VisualDebugToggle(VD_AABB);
				}
				else if(DIK_8 == uKeyCode)
				{	// Toggle visual debugging of the portals
					VisualDebugToggle(VD_PORTAL);
				}
				else if(DIK_7 == uKeyCode)
				{	// Toggle visual debugging of the light's AABB volumes
					VisualDebugToggle(VD_LIGHTVOL);
				}
#endif

			}

			void Manager::ActivateCamera(Camera* pCam)
			{
				GAE_ASSERT(NULL != pCam);
				m_pCameraActive = pCam;
				m_pRenderer->SetViewMatrix(pCam->GetViewMatrix());
				m_pRenderer->SetProjectionMatrix(pCam->GetProjectionMatrix());
				m_pRenderer->SetViewProjectionMatrix(pCam->GetViewProjectionMatrix());

#ifdef _DEBUG_MESHES
				m_pCameraCulling = pCam;
#endif
			}

			bool Manager::Uninit()
			{
				// Unregister the input callbacks.
				GAE_INPUT_UNREGISTER_EVENT(OnKeyPress, this);

				do 
				{
					VecCamera::iterator it = m_VecCamera.begin();
					(*it)->Uninit();
					GAE_DELETESAFE(*it);
					m_VecCamera.erase(it);
				} while (!m_VecCamera.empty());

				return true;

				using namespace Gae::Graphics;
				Gae::Renderer::Driver& renderer = *Gae::Core::Application::Singleton().GetRenderer();
				for each(MultiMesh* pMultiMesh in m_VecStaticGeometry)
				{
					for each(Mesh* pMesh in pMultiMesh->GetMeshes())
					{

						m_pRenderer->UninitMesh(pMesh);
					}
					pMultiMesh->Uninit();
				}

				for each(MultiMesh* pMultiMesh in m_VecStaticGeometry)
				{
					
				}

				for(size_t i = 0, cuVecEntity3DSize = m_VecEntities3D.size(); i < cuVecEntity3DSize; ++i)
				{
					Gae::Entities::Entity3D* pEnt = m_VecEntities3D.at(i);

					pEnt->Uninit();
					GAE_DELETESAFE(pEnt);
				}

				m_VecEntities3D.clear();

				return TRUE;
			}
		}
	}
}