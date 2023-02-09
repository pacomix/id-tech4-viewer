#ifndef Entity3D_h__
#define Entity3D_h__

namespace Gae
{
	namespace Graphics
	{
		class Mesh;
	}

	namespace Entities
	{
		class Entity3D : public IEntity
		{
		public:
			Entity3D();
			~Entity3D();

			// IEntity Interface
			virtual bool Init();
			virtual bool Update();
			virtual bool Uninit();

		private:
			Graphics::Mesh* m_pMesh;	//!< Mesh if any.
			
		};
	}
}

#endif // Entity3D_h__