#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12Renderer.h>

namespace RayEngine
{
	namespace Graphics
	{
		DX12Renderer::DX12Renderer(DX12Device* pDevice)
			: m_References(0)
		{
		}

		DX12Renderer::~DX12Renderer()
		{
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