#ifndef Modifier_h__
#define Modifier_h__

namespace Gae
{
	namespace Entities
	{
		class Modifier
		{
		public:
			Modifier(){};
			virtual ~Modifier(){};
			virtual bool Init(IEntity* pEntity);
			virtual void Run() = 0;
			virtual void Uninit() = 0;

		protected:
			Gae::Entities::IEntity* m_pParent;
		};
	}
}

#endif // Modifier_h__