#ifndef ShadowVolume_h__
#define ShadowVolume_h__

namespace Gae
{
	namespace Graphics
	{
		class Mesh;

		namespace Geometry
		{
			class ShadowVolume
#ifdef _DEBUG_DRAW
				: public Gae::Renderer::IDrawDebug
#endif
			{

			public:
				ShadowVolume();
				~ShadowVolume();

				inline const UINT32 GetCountCaps() const;
				inline const UINT32 GetCountCapsFront() const;
				inline const UINT32 GetPlaneBits() const;
				inline const Mesh* GetMesh() const;

				inline void SetCountCaps(const UINT32 uVal);
				inline void SetCountCapsFront(const UINT32 uVal);
				inline void SetPlaneBits(const UINT32 uVal);
				inline void SetMesh(Mesh* pMesh);

			protected:
				Mesh* m_pMesh;	// Volume mesh.
				UINT32 m_uCountCaps;
				UINT32 m_uCountCapsFront;
				UINT32 m_uPlaneBits;

#ifdef _DEBUG_DRAW
				void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif
			};
		}
	}
}

#include "ShadowVolume.inl"
#endif // ShadowVolume_h__