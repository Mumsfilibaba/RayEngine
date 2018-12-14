#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12Renderer.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Swapchain.h>

namespace RayEngine
{
	namespace Graphics
	{
		DX12Renderer::DX12Renderer(DX12Device* pDevice, DX12Swapchain* pSwapChain)
			: m_pDevice(nullptr),
			m_pSwapChain(nullptr),
			m_References(0)
		{
			AddRef();

			m_pDevice = pDevice;
			m_pDevice->AddRef();

			m_pSwapChain = pSwapChain;
			m_pSwapChain->AddRef();
		}

		DX12Renderer::~DX12Renderer()
		{
			ReRelease_S(m_pDevice);
			ReRelease_S(m_pSwapChain);
		}

		void DX12Renderer::Clear(float color[4]) const
		{
		}

		void DX12Renderer::Draw() const
		{
		}

		void DX12Renderer::Present() const
		{
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