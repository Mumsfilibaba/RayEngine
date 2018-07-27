#pragma once

#include "..\Graphics\ISwapchain.h"
#include "VkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VKSwapchain : public ISwapchain
		{
		public:
			VKSwapchain(VkDevice device, VkPhysicalDevice adapter, VkSurfaceKHR surface, const SwapchainInfo& info);
			~VKSwapchain();

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
			void Create(VkDevice device, VkPhysicalDevice adapter, VkSurfaceKHR surface, const SwapchainInfo& info);

		private:
			VkSurfaceFormatKHR m_Format;
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;

			mutable uint32 m_ReferenceCount;
		};
	}
}