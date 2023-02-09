namespace Gae
{
	namespace Entities
	{
		void IEntity::SetName(const std::wstring& wsName)
		{
			m_wsName = wsName;
		}

		const std::wstring& IEntity::GetName() const
		{
			return m_wsName;
		}
	}
}