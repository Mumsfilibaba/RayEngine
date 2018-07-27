#pragma once

#include "..\Graphics\ISwapchain.h"
#include "DX12Texture.h"

#if defined(RE_PLATFORM_WINDOWS)

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
			DX12Swapchain(IFactory* pFactory, const SwapchainInfo& info);
			DX12Swapchain(DX12Swapchain&& other);
			~DX12Swapchain();

			DX12Swapchain& operator=(DX12Swapchain&& other);

			int32 GetCurrentBuffer() const override final;
			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;
			void Present() const override final;
			
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
			ICommandQueue* m_CommandQueue;
			
			IDXGISwapChain1* m_Swapchain;
			int32 m_BufferCount;
			std::vector<DX12Texture> m_Textures;
			mutable int32 m_CurrentBuffer;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif