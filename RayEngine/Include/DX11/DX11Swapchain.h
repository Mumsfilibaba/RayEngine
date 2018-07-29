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

		public:
			DX11Swapchain(IFactory* pFactory, const SwapchainInfo& info);
			DX11Swapchain(DX11Swapchain&& other);
			~DX11Swapchain();

			DX11Swapchain& operator=(DX11Swapchain&& other);

			int32 GetCurrentBuffer() const override final;
			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;
			void Present() const override final;

			IFactory* GetFactory() const override final;
			ICommandQueue* GetCommandQueue() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IFactory* pFactory, const SwapchainInfo& info);
			void CreateTextures();

		private:
			IDevice* m_Device;
			IFactory* m_Factory;
			ICommandQueue* m_CommandQueue;

			IDXGISwapChain* m_Swapchain;
			int32 m_BufferCount;
			DX11Texture m_Texture;
			mutable int32 m_CurrentBuffer;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif