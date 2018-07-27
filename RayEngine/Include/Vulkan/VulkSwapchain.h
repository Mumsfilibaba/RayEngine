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
			VulkSwapchain(IDevice* pDevice, VkSurfaceKHR surface, const SwapchainInfo& info);
			~VulkSwapchain();

			int32 GetCurrentBuffer() const override final;
			ITexture* GetBuffer(int32 index) override final;
			const ITexture* GetBuffer(int32 index) const override final;
			void Present() const override final;

			void Release(VkInstance instance, VkDevice device);

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IDevice* pDevice, VkSurfaceKHR surface, const SwapchainInfo& info);

		private:
			VkSurfaceFormatKHR m_Format;
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;

			mutable uint32 m_ReferenceCount;
		};
	}
}