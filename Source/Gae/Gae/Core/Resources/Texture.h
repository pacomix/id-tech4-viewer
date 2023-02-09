#ifndef Texture_h__
#define Texture_h__

namespace Gae
{
	namespace Renderer
	{
		class DirectX9;
		class OpenGL;
	}

	namespace Core
	{
		namespace Resources
		{
			class Texture : public Gae::Core::Resources::Resource
			{
			public:
				friend class Gae::Renderer::DirectX9;
				friend class Gae::Renderer::OpenGL;

				typedef Gae::Core::Resources::Resource super;
				Texture();
				virtual ~Texture();

				void SetFilename(const std::wstring& wsFilename);
				void SetFilename(const std::string& strFilename);
				const Vector2f& GetTexelSize() const;
				bool IsAlpha() const;
				
#ifdef _DEBUG
				void ToDebugOutput();
#endif

			protected:
				std::wstring m_wsFilename;

				// OpenGL/DirectX Texture. Renderer must cast this to its
				// correct data type.
				void* m_pTexture;

				// The renderer is the responsible to determine
				// whether if the texture has to be processed as
				// an alpha texture or not.
				bool m_bIsAlpha;

				Vector2f m_v2fTexelSize;
				float m_fTexelsPerSide;
			};
		}
	}
}

#include "Texture.inl"

#endif // Texture_h__