#pragma once

#include <vector>
#include "..\..\Include\Graphics\ISwapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Factory.h"
#include "DX11Texture.h"
#include "DX11CommandQueue.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Swapchain final : public ISwapchain
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
			void QueryCommandQueue(ICommandQueue** ppCommandQueue) const override final;
			void QueryFactory(IFactory** ppFactory) const override final;

		private:
			void Create(const SwapchainInfo& info);
			void CreateTextures(const SwapchainInfo& info);

		private:
			DX11Device* m_Device;
			DX11Factory* m_Factory;
			DX11CommandQueue* m_CommandQueue;
			IDXGISwapChain* m_Swapchain;
			DX11Texture* m_Texture;
			int32 m_BufferCount;
			mutable int32 m_CurrentBuffer;
		};
	}
}

#endif