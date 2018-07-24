#pragma once

#include "..\Graphics\ISwapchain.h"
#include "DX12Texture.h"

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
			DX12Swapchain(IDXGIFactory5* pFactory, const SwapchainInfo& info);
			DX12Swapchain(DX12Swapchain&& other);
			~DX12Swapchain();

			int32 GetCurrentBuffer() const override final;
			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;
			void Present() const override final;

			DX12Swapchain& operator=(DX12Swapchain&& other);

		private:
			void Create(IDXGIFactory5* pFactory, const SwapchainInfo& info);
			void CreateTextures();

		private:
			IDXGISwapChain1* m_Swapchain;
			int32 m_BufferCount;
			mutable int32 m_CurrentBuffer;
			std::vector<DX12Texture> m_Textures;
		};
	}
}