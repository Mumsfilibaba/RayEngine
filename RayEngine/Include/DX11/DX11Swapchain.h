#pragma once

#include <vector>
#include "..\..\Include\Graphics\ISwapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Swapchain : public ISwapchain
		{
		public:
			DX11Swapchain(const DX11Swapchain& other) = delete;
			DX11Swapchain& operator=(const DX11Swapchain& other) = delete;
			DX11Swapchain(DX11Swapchain&& other) = delete;
			DX11Swapchain& operator=(DX11Swapchain&& other) = delete;

		public:
			DX11Swapchain(IFactory* pFactory, const SwapchainInfo& info);
			~DX11Swapchain();

			int32 GetCurrentBuffer() const override final;
			void Present() const override final;

			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;
			IFactory* GetFactory() const override final;
			ICommandQueue* GetCommandQueue() const override final;

		private:
			void Create(IFactory* pFactory, const SwapchainInfo& info);
			void CreateTextures();

		private:
			IDevice* m_Device;
			IFactory* m_Factory;
			ICommandQueue* m_CommandQueue;
			IDXGISwapChain* m_Swapchain;
			DX11Texture* m_Texture;
			int32 m_BufferCount;
			mutable int32 m_CurrentBuffer;
		};
	}
}

#endif