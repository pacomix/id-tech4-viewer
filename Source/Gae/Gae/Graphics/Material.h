#ifndef Material_h__
#define Material_h__

using namespace Gae::Core::Resources;

namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			class Texture;
		}
	}
	namespace Graphics
	{
		// \TODO Add support for several blending stages.
		class Material : public Gae::Core::Resources::Resource
		{
		public:
			friend class Gae::Renderer::DirectX9;
			friend class Gae::Renderer::OpenGL;

			struct BlendStage
			{
				BlendStage() : m_pTexture(NULL), m_dwBlendSrc(0), m_dwBlendDst(0), m_dwMaskComponent(0xffffffff), m_bMakeAlpha(false) {}
				~BlendStage() {}

				Texture* m_pTexture;
				DWORD m_dwBlendSrc;
				DWORD m_dwBlendDst;
				DWORD m_dwMaskComponent;
				bool m_bMakeAlpha;
			};

			GAE_CONTAINER_DECL(Gae::Graphics::Material::BlendStage, BlendStage);

		public:

			enum E_FLAGS
			{
				M_ALPHATEST = 0x1,
				M_DECAL = 0x2,
				M_BLEND = 0x4,
				M_SOLID = 0x8,
				M_TWOSIDED = 0x10,
				M_HEIGHTMAP = 0x20
			};

			/*
			gl_one	Constant 1
			gl_zero	Constant 0
			gl_dst_color	The color currently on the screen
			gl_one_minus_dst_color	One minus the color currently on the screen
			gl_src_alpha	The alpha channel of the source image
			gl_one_minus_src_alpha	One minus the alpha channel of the source image
			gl_dst_alpha	The alpha channel of the screen image
			gl_one_minus_dst_alpha	One minus the alpha channel of the screen image
			gl_src_alpha_saturate	Minimum of the source alpha and one minus screen alpha


			Destination blend modes: gl_one	Constant 1
			gl_zero	Constant 0
			gl_src_color	The color of the source image
			gl_one_minus_src_color	One minus the color of the source image
			gl_src_alpha	The alpha channel of the source image
			gl_one_minus_src_alpha	One minus the alpha channel of the source image
			gl_dst_alpha	The alpha channel of the screen image
			gl_one_minus_dst_alpha	One minus the alpha channel of the screen image
			*/

			enum E_BLEND_TYPE
			{
				BT_ONE,
				BT_ZERO,
				BT_DST_COLOR,
				BT_ONE_MINUS_DST_COLOR,
				BT_SRC_ALPHA,
				BT_ONE_MINUS_SRC_ALPHA,
				BT_DST_ALPHA,
				BT_ONE_MINUS_DST_ALPHA,
				BT_SRC_ALPHA_SATURATE,
				BT_SRC_COLOR,
				BT_ONE_MINUS_SRC_COLOR
			};

			Material();
			~Material();

			static inline DWORD GetRendererBlendValue(E_BLEND_TYPE eBlendType);
 
			inline void SetMapDiffuse(Texture* pTexture);
			inline void SetMapSpecular(Texture* pTexture);
			inline void SetMapHeight(Texture* pTexture);
			inline void SetMapNormal(Texture* pTexture);

			inline Texture* GetMapDiffuse() const;
			inline Texture* GetMapSpecular() const;
			inline Texture* GetMapHeight() const;
			inline Texture* GetMapNormal() const;

			inline void SetBlendSrcDst(const E_BLEND_TYPE eBlendSrc, const E_BLEND_TYPE eBlendDst, bool bDirectX = true);
			inline DWORD GetBlendSrc() const;
			inline DWORD GetBlendDst() const;

			inline void SetAlphaTestValue(float fAlphaTest);
			inline void SetPolygonOffset(DWORD dwPolygonOffset);
			inline DWORD GetPolygonOffset() const;

			inline void FlagSet(int uFlag)
			{
				m_uFlags |= uFlag;
			}

			inline void FlagUnset(int uFlag)
			{
				m_uFlags &= ~uFlag;
			}

			inline void FlagToggle(int uFlag)
			{
				m_uFlags ^= uFlag;
			}

			inline const bool FlagEnabled(int uFlag) const
			{
				return (m_uFlags & uFlag) > 0;
			}

		private:

			Gae::Core::Resources::Texture* m_pMapDiffuse;
			Gae::Core::Resources::Texture* m_pMapSpecular;
			Gae::Core::Resources::Texture* m_pMapHeight;
			Gae::Core::Resources::Texture* m_pMapNormal;

			float m_fAlphaTest;
			UINT32 m_uFlags;

			E_BLEND_TYPE m_eBlendSrc;
			E_BLEND_TYPE m_eBlendDst;

			DWORD m_dwBlendSrc;
			DWORD m_dwBlendDst;

			DWORD m_dwPolygonOffset;

		};
	}
}

#include "Material.inl"

#endif // Material_h__