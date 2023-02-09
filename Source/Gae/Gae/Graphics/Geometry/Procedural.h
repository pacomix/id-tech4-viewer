#ifndef Procedural_h__
#define Procedural_h__

namespace Gae
{
	namespace Graphics
	{
		class MultiMesh;

		namespace Geometry
		{
			class Procedural
			{
				SINGLETON_DECL(Procedural);

			public:
				MultiMesh* GetCube() const;

			private:
				MultiMesh* CreateCube() const;

				// Cache
				MultiMesh* m_pMeshCube;
			};
		}
	}
}

#endif // Procedural_h__