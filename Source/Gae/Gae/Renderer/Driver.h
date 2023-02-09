#ifndef Driver_h__
#define Driver_h__

#include "stdafx.h"



namespace Gae
{
	namespace Core
	{
		namespace Universe
		{
			class Manager;
		}

		class Application;
	}

	namespace Graphics
	{
		class Mesh;
		class Node;
	}

	namespace Entities
	{
		class Light;
	}

	namespace Renderer
	{
		class Driver
		{
			friend class Gae::Core::Application;
			friend class Gae::Core::Universe::Manager;

		public:
			EIGEN_MAKE_ALIGNED_OPERATOR_NEW

		public:
			//typedef std::deque< const Gae::Graphics::Mesh* > DeqRenderList;
			//typedef std::deque< const Gae::Entities::Light* > DeqRenderLight;
			typedef std::vector< const Gae::Graphics::Mesh* > VecRenderList;
			typedef std::vector< const Gae::Entities::Light* > VecRenderLight;

			inline const Matrix4f& GetViewMatrix() const;
			inline const Matrix4f& GetProjectionMatrix() const;
			inline const Matrix4f& GetViewProjectionMatrix() const;

			/*	\brief Calculates the screen rect viewport of a given set of vertexes.
				\param[in]	av4fVertexes	Pointer to the vertex array.
				\param[in]	iVtxCount		Vertex count in the array.
				\param[out] outViewport		Screen rect/viewport of the screen's projected vertexes.
			*/
			virtual void ComputeViewport(const Vector4f* av4fVertexes, int iVtxCount, RECT& outViewport) const;

		protected:

			Driver();
			virtual ~Driver();

			bool Init();
			inline void SetViewMatrix(const Matrix4f& mtx4fView);
			inline void SetProjectionMatrix(const Matrix4f& mtx4fView);
			inline void SetViewProjectionMatrix(const Matrix4f& mtx4fView);

			virtual void Render() = 0;
			virtual void RenderWithShadows(){};
			virtual bool InitMesh(Gae::Graphics::Mesh* pMesh);
			virtual bool UninitMesh(Gae::Graphics::Mesh* pMesh);

			// Clear the correspondent render list and adds every element to it.
			virtual void SetRenderStatic(const VecRenderList& deqRenderList);
			virtual void SetRenderShadowStatic(const VecRenderList& deqRenderList);
			virtual void SetRenderLight(const VecRenderLight& deqRenderList);

			virtual void AddToRender(const Gae::Graphics::Mesh* pMesh);
			virtual void AddToRender(const VecRenderList& deqRenderList);
			virtual void AddToRenderStatic(const Gae::Graphics::Mesh* pMesh);
			virtual void AddToRenderStatic(const VecRenderList& deqRenderList);
			virtual void AddToRenderShadowStatic(const Gae::Graphics::Mesh* pMesh);
			virtual void AddToRenderShadowStatic(const VecRenderList& deqRenderList);
			virtual void AddToRenderLight(const Gae::Entities::Light* pLight);
			virtual void AddToRenderLight(const VecRenderLight& deqRenderLight);
			

			inline const VecRenderList& GetDeqRender() const;
			inline const VecRenderList& GetDeqRenderStatic() const;
			inline const VecRenderList& GetDeqRenderStaticTranslucent() const;
			inline const VecRenderList& GetDeqRenderShadowStatic() const;

			inline const VecRenderLight& GetDeqRenderLight() const;

			// START - Draw Debug Methods
#ifdef _DEBUG_DRAW	// Draw Debug Methods

			typedef std::vector< const IDrawDebug* > VecDrawDebug;
			VecDrawDebug m_VecDrawDebug;

			/*	\brief	Draws a single triangle.
					It draws a single triangle of the specified color without culling.
				
				\param[in]	pv3fVertex	Pointer to an structure of the three vertexes of the triangle.
				\param[in]	uColor		Vertex's color in XRGB format.
				\note	This is extreeeeeeemely slooooooooooooooow. So use it moderately. It is 
					only intended for debug purposes.
			*/
		public:
			virtual void DrawDebugTriangles(const Vector4f* pv4fVertexes, UINT32 uCount) const;
			virtual void DrawDebugTriangle(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2, const Vector4f* pv4fVertex3) const;
			virtual void DrawDebugLines(const Vector4f* pv4fVertexes, UINT32 uCount) const;
			virtual void DrawDebugLine(const Vector4f* pv4fVertex1, const Vector4f* pv4fVertex2) const;
			virtual void DrawDebugRect(const RECT* pRect, const DWORD dwColor) const;

			enum E_DEBUG_SHAPE
			{
				DS_SPHERE = 0,
				DS_BOX,
				DS_CYLINDER
			};
			/*	\Brief	Draws shapes.
				\param[in]	E_DEBUG_SHAPE			Type of shape.
				\param[in]	Gae::Graphics::Node*	Pointer to a Node object for its world transformation.
				\param[in]	DWORD					Color of the shape.
				\note	This method must be implemented in every inherited class.
			*/
			GAE_DEBUG_FN_DECL(virtual void DrawDebug(const E_DEBUG_SHAPE eShape, const Gae::Graphics::Node* pNode, const DWORD dwColor) const = 0);

		protected:
			inline void AddToDrawDebug(const IDrawDebug* pClass)
			{
				GAE_WARNINGM(std::find(m_VecDrawDebug.begin(), m_VecDrawDebug.end(), pClass) == m_VecDrawDebug.end(), L"DrawDebug class was already added!");

				m_VecDrawDebug.push_back(pClass);
			}

			inline void DrawDebug() const
			{
				for(VecDrawDebug::const_iterator it = m_VecDrawDebug.begin(), itEnd = m_VecDrawDebug.end(); it != itEnd; ++it)
				{
					if((*it)->IsDrawDebug())
					{
						(*it)->DrawDebug(this);
					}
				}
			}

			inline void DrawDebugClear()
			{
				m_VecDrawDebug.clear();
			}
#endif
			// END - Draw Debug Methods

			const RendererConfig* m_pRendererCfg;

			// view & projection
			Matrix4f m_mtx44fView;
			Matrix4f m_mtx44fProj;
			Matrix4f m_mtx44fViewProj;

			// Static render list.
			VecRenderList m_DeqRenderStatic;
			VecRenderList m_DeqRenderStaticTranslucent;
			VecRenderList m_DeqRenderShadowStatic;
			VecRenderList m_DeqRenderStaticDecal;

			// Render list. 
			VecRenderList m_DeqRender;

			// Lights render list.
			VecRenderLight m_DeqRenderLight;

		private:
			inline void ClearRenderLists()
			{
				m_DeqRenderStaticTranslucent.clear();
				m_DeqRenderStaticDecal.clear();
				m_DeqRenderStatic.clear();
			}
		};

	}
}

#include "Renderer/Driver.inl"
#endif // Driver_h__