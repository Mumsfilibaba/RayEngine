#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12Renderer.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Swapchain.h>
#include <DX12/DX12CommandList.h>

namespace RayEngine
{
	namespace Graphics
	{
		DX12Renderer::DX12Renderer(DX12Device* pDevice, DX12Swapchain* pSwapChain, DX12CommandQueue* pQueue)
			: m_pDevice(nullptr),
			m_pSwapChain(nullptr),
			m_References(0)
		{
			AddRef();

			m_pDevice = pDevice;
			m_pDevice->AddRef();

			m_pSwapChain = pSwapChain;
			m_pSwapChain->AddRef();

			m_pList = new DX12CommandList(pDevice, nullptr, D3D12_COMMAND_LIST_TYPE_DIRECT);
		}

		DX12Renderer::~DX12Renderer()
		{
			ReRelease_S(m_pDevice);
			ReRelease_S(m_pSwapChain);
		}

		void DX12Renderer::Begin() const
		{
			m_pList->ResetAllocator();
			m_pList->Reset();
		}

		void DX12Renderer::Clear(float color[4]) const
		{
			m_pList->GetGraphicsList()->ClearRenderTargetView(0, color, 0, nullptr);
			m_pList->GetGraphicsList()->ClearDepthStencilView(0, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);
		}

		void DX12Renderer::Draw() const
		{
			m_pList->GetGraphicsList()->DrawInstanced(3, 1, 0, 0);
		}

		void DX12Renderer::End() const
		{
			m_pList->Close();
		}

		void DX12Renderer::Present() const
		{
			m_pSwapChain->Present();
		}

		CounterType DX12Renderer::AddRef()
		{
			return ++m_References;
		}

		CounterType DX12Renderer::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}
	}
}
#endif