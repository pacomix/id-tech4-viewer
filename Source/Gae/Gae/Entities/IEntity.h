#ifndef IEntity_h__
#define IEntity_h__

namespace Gae
{
	namespace Entities
	{
		class IEntity : public Gae::Graphics::Node
		{
		public:
			IEntity();
			virtual ~IEntity();

			virtual bool Init() = 0;
			virtual bool Update() = 0;
			virtual bool Uninit() = 0;

			inline void SetName(const std::wstring& wsName);
			inline const std::wstring& GetName() const;
		private:
			std::wstring m_wsName;
		};
	}
}
#include "IEntity.inl"
#endif // IEntity_h__