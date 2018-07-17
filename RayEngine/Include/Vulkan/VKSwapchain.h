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

			const ITexture* GetBuffer(int32 index) const override final;
			void Present() const override final;
			void Release(VkInstance instance, VkDevice device);

		private:
			void Create(VkDevice device, VkPhysicalDevice adapter, VkSurfaceKHR surface, const SwapchainInfo& info);

		private:
			VkSurfaceFormatKHR m_Format;
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;
		};
	}
}