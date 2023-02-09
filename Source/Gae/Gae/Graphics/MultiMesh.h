#ifndef MultiMesh_h__
#define MultiMesh_h__

namespace Gae
{
	namespace Graphics
	{
		class MultiMesh : public Gae::Core::Resources::Resource
		{
		public:
			typedef std::vector< Mesh* > VecMeshes;

			MultiMesh();
			~MultiMesh();

			void AddMesh(Mesh* pMesh);
			void SetMeshCount(int iCount);
			const VecMeshes& GetMeshes() const;

			bool Init();
			bool Uninit();

		private:
			VecMeshes m_VecMeshes;
		};
	}
}

#include "MultiMesh.inl"

#endif // MultiMesh_h__