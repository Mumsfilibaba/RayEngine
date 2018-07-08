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

			void Present() const override final;
			void Release(VkInstance instance, VkDevice device);

		private:
			void Create(VkDevice device, VkPhysicalDevice adapter, VkSurfaceKHR surface, const SwapchainInfo& info);

		private:
			VkSurfaceKHR m_Surface;
			VkSwapchainKHR m_Swapchain;
		};
	}
}