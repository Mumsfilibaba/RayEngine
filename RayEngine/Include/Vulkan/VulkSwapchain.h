#pragma once

#include "..\Graphics\ISwapchain.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkSwapchain : public ISwapchain
		{
		public:
			VulkSwapchain(const VulkSwapchain& other) = delete;
			VulkSwapchain& operator=(const VulkSwapchain& other) = delete;

		public:
			VulkSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info);
			VulkSwapchain(VulkSwapchain&& other);
			~VulkSwapchain();

			VulkSwapchain& operator=(VulkSwapchain&& other);

			VkSurfaceKHR GetVkSurfaceKHR() const;
			VkSwapchainKHR GetVkSwapchainKHR() const;

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
			void Create(IDevice* pDevice, const SwapchainInfo& info);

		private:
			IDevice* m_Device;
			IFactory* m_Factory;
			ICommandQueue* m_CommandQueue;

			VkSurfaceFormatKHR m_Format;
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;

			mutable uint32 m_ReferenceCount;
		};
	}
}