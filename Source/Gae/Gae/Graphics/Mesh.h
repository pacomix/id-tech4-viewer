#ifndef Mesh_h__
#define Mesh_h__

namespace Gae
{
	namespace Core
	{
		namespace Resources
		{
			class Texture;
		}

		namespace Universe
		{
			class Portal;
		}
	}

	namespace Entities
	{
		class Light;
	}

	namespace Graphics
	{
		namespace Geometry
		{
			class VertexBuffer;
			class IndexBuffer;
			class AABB;
		}

		class Material;

		using namespace Gae::Core::Universe;

		class Mesh : public Gae::Core::Resources::Resource
#ifdef _DEBUG_DRAW
			, public Gae::Renderer::IDrawDebug
#endif
		{
		public:
			EIGEN_MAKE_ALIGNED_OPERATOR_NEW

			friend class Gae::Renderer::DirectX9;
			friend class Gae::Renderer::OpenGL;

			GAE_CONTAINER_DECL(Gae::Entities::Light, Light);
			//GAE_CONTAINER_DECL(Gae::Core::Universe::Portal, Portal);
			//GAE_CONTAINER_DECL(RECT, ScreenRect);
			typedef std::vector< RECT > VecRect;
			typedef std::vector< const Portal* > VecPortal;

		public:

			typedef Gae::Core::Resources::Resource super;

			Mesh();
			virtual ~Mesh();

			bool IsUsingTransparency() const;

			void SetVertexBuffer(Geometry::VertexBuffer* pVB);
			void SetIndexBuffer(Geometry::IndexBuffer* pIB);
			void SetMaterial(Material* pMaterial);
			void ComputeAABB();
			Material* GetMaterial() const;
			Geometry::VertexBuffer* GetVertexBuffer() const;
			Geometry::IndexBuffer* GetIndexBuffer() const;
			const Geometry::AABB& GetAABB() const;

			inline void SetDrawnTick(const UINT64 u64DrawnTick)
			{
				m_u64DrawnTick = u64DrawnTick;
			}

			inline const UINT64 GetDrawnTick() const
			{
				return m_u64DrawnTick;
			}

			/*
			inline void SetPortal(const Core::Universe::Portal* pPortal)
			{
				m_pPortal = pPortal;
			}

			inline const Core::Universe::Portal* GetPortal() const
			{
				return m_pPortal;
			}
			*/

#ifdef _DEBUG
			void ToDebugOutput();
#endif

			inline const VecRect& GetVecRect() const
			{
				return m_VecRect;
			}

			inline void AddRect(const RECT& rect)
			{
				m_VecRect.push_back(rect);
			}

			inline void ClearVecRect()
			{
				m_VecRect.clear();
			}

			inline const VecPortal& GetVecPortal() const
			{
				return m_VecPortal;
			}

			inline void AddPortal(const Portal* pPortal)
			{
				m_VecPortal.push_back(pPortal);
			}

			inline void ClearVecPortal()
			{
				m_VecPortal.clear();
			}

#ifdef _DEBUG_DRAW
			void DrawDebug(const Gae::Renderer::Driver* pRenderer) const;
#endif

		protected:
			Geometry::VertexBuffer* m_pVertexBuffer;
			Geometry::IndexBuffer* m_pIndexBuffer;
			Material* m_pMaterial;

			Geometry::AABB m_AABB;

			UINT64 m_u64DrawnTick;

			VecRect m_VecRect;
			VecPortal m_VecPortal;	//!< Portals this mesh is seen through.
		};
	}
}

#include "Mesh.inl"
#endif // Mesh_h__