#pragma once
#include <Graphics/IRenderer.h>

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Device;
		class DX12Swapchain;
		class DX12CommandList;
		class DX12CommandQueue;

		class DX12Renderer final : public IRenderer
		{
			RE_IMPLEMENT_INTERFACE(DX12Renderer);

		public:
			DX12Renderer(DX12Device* pDevice, DX12Swapchain* pSwapChain, DX12CommandQueue* pQueue);
			~DX12Renderer();

			void Begin() const;

			void Clear(float color[4]) const;

			void Draw() const;

			void End() const;

			void Present() const;

			CounterType AddRef();

			CounterType Release();

		private:
			DX12Device* m_pDevice;
			DX12Swapchain* m_pSwapChain;
			DX12CommandList* m_pList;
			DX12CommandQueue* m_pQueue;

			CounterType m_References;
		};
	}
}