namespace Gae
{
	namespace Gifts
	{
		namespace Quake4
		{
			MapLoader::VecModel& MapLoader::GetStaticGeometry()
			{
				return m_VecModel;
			}

			/*
			const Vector3f& MapLoader::GetSpawnPosition() const
			{
				return m_v3fSpawn;
			}
			*/
			const MapLoader::VecSpawnPoints& MapLoader::GetSpawnPositions() const
			{
				return m_VecSpawnPoints;
			}

			bool MapLoader::FileExist(const std::wstring& wsFilename) const
			{
				std::ifstream FileLevel;

				FileLevel.open(wsFilename.c_str(), std::ios::ate);

				bool bExist = FileLevel.is_open();

				if(bExist)	FileLevel.close();

				return bExist;
			}
		}
	}
}