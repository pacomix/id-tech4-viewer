namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			Camera& Manager::GetActiveCamera() const
			{
				GAE_ASSERTM(NULL != m_pCameraActive, L"Error! There are no valid active cameras!!");

#ifdef _DEBUG_MESHES
				return *m_pCameraCulling;
#else
				return *m_pCameraActive;
#endif
			}

			inline void Manager::CreatePortalAreas(UINT32 uCount)
			{
				GAE_ASSERTM(m_VecPortalArea.empty(), L"Portal's Areas are already defined!!!");

				m_VecPortalArea.reserve(uCount);
				for(UINT32 i = 0; i < uCount; ++i)
				{
					m_VecPortalArea.push_back(new PortalArea);
				}
			}

			inline const IPortalAreaDetermination* Manager::GetPortalAreaDetermination() const
			{
				return m_pPortalAreaDetermination;
			}

			inline void Manager::SetPortalAreaDetermination(IPortalAreaDetermination* pPortalAreaDetermination)
			{
				GAE_ASSERT(NULL != pPortalAreaDetermination);
				GAE_ASSERT(NULL == m_pPortalAreaDetermination);

				m_pPortalAreaDetermination = pPortalAreaDetermination;
			}

			inline void Manager::AddPortal(Portal* pPortal)
			{
				GAE_ASSERT(NULL != pPortal);
				GAE_ASSERTM(!m_VecPortalArea.empty(), L"Portal's Areas were not created! Did you forget to call Universe::Manager::CreatePortalAreas?");
				GAE_ASSERTM((int)m_VecPortalArea.size() > pPortal->GetPositiveSideAreaIndex() && (int)m_VecPortalArea.size() > pPortal->GetNegativeSideAreaIndex(), L"One of the areas associated to the specified Portal doesn't exist!");
				m_VecPortalArea.at(pPortal->GetNegativeSideAreaIndex())->AddPortal(pPortal);
				m_VecPortalArea.at(pPortal->GetPositiveSideAreaIndex())->AddPortal(pPortal);
				pPortal->SetPositiveSideArea(m_VecPortalArea.at(pPortal->GetPositiveSideAreaIndex()));
				pPortal->SetNegativeSideArea(m_VecPortalArea.at(pPortal->GetNegativeSideAreaIndex()));
			}
		}
	}
}