#pragma once

#include "..\Graphics\ISwapchain.h"
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Swapchain : public ISwapchain
		{
		public:
			DX12Swapchain(const DX12Swapchain& other) = delete;
			DX12Swapchain& operator=(const DX12Swapchain& other) = delete;

		public:
			DX12Swapchain(IDXGIFactory5* factory, const SwapchainInfo& info);
			DX12Swapchain(DX12Swapchain&& other);
			~DX12Swapchain();

			void Present() const override final;

			DX12Swapchain& operator=(DX12Swapchain&& other);

		private:
			void Create(IDXGIFactory5* factory, const SwapchainInfo& info);

		private:
			IDXGISwapChain1* m_Swapchain;
		};
	}
}