#include "..\..\Include\Vulkan\VKSwapchain.h"
#include <vector>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VKSwapchain::VKSwapchain(VkDevice device, VkPhysicalDevice adapter, VkSurfaceKHR surface, const SwapchainInfo& info)
			: m_Swapchain(0),
			m_Surface(0),
			m_Format()
		{
			Create(device, adapter, surface, info);
		}



		/////////////////////////////////////////////////////////////
		VKSwapchain::~VKSwapchain()
		{
		}



		/////////////////////////////////////////////////////////////
		int32 VKSwapchain::GetCurrentBuffer() const
		{
			return int32();
		}



		/////////////////////////////////////////////////////////////
		ITexture* VKSwapchain::GetBuffer(int32 index)
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		const ITexture* VKSwapchain::GetBuffer(int32 index) const
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		void VKSwapchain::Present() const
		{
		}



		/////////////////////////////////////////////////////////////
		void VKSwapchain::Release(VkInstance instance, VkDevice device)
		{
			if (m_Surface != 0)
			{
				vkDestroySurfaceKHR(instance, m_Surface, nullptr);
				m_Surface = 0;
			}

			if (m_Swapchain != 0)
			{
				vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
				m_Swapchain = 0;
			}
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* VKSwapchain::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VKSwapchain::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VKSwapchain::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VKSwapchain::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VKSwapchain::Create(VkDevice device, VkPhysicalDevice adapter, VkSurfaceKHR surface, const SwapchainInfo& info)
		{
			m_Surface = surface;


			VkSurfaceCapabilitiesKHR capabilities = {};
			VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(adapter, m_Surface, &capabilities);
			if (result != VK_SUCCESS)
				return;


			uint32 count = 0;
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, m_Surface, &count, nullptr);
			if (result != VK_SUCCESS)
				return;

			std::vector<VkSurfaceFormatKHR> formats;
			formats.resize(count);
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, m_Surface, &count, formats.data());
			if (result != VK_SUCCESS)
				return;


			result = vkGetPhysicalDeviceSurfacePresentModesKHR(adapter, m_Surface, &count, nullptr);
			if (result != VK_SUCCESS)
				return;

			std::vector<VkPresentModeKHR> presentModes;
			presentModes.resize(count);
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(adapter, m_Surface, &count, presentModes.data());
			if (result != VK_SUCCESS)
				return;


			VkFormat format;
			if (info.Buffer.Format == FORMAT_R8G8B8A8_UINT)
				format = VK_FORMAT_R8G8B8A8_UNORM;
			else if (info.Buffer.Format == FORMAT_B8G8R8A8_UNORM)
				format = VK_FORMAT_B8G8R8A8_UNORM;
			else if (info.Buffer.Format == FORMAT_R32G32B32A32_FLOAT)
				format = VK_FORMAT_R32G32B32A32_SFLOAT;

			for (int32 i = 0; i < static_cast<int32>(formats.size()); i--)
			{
				if (formats[i].format == format)
				{
					m_Format.format = format;
					break;
				}
			}

			

			VkSwapchainCreateInfoKHR scInfo = {};
			scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			scInfo.pNext = nullptr;
			scInfo.flags = 0;
			scInfo.oldSwapchain = 0;
			scInfo.minImageCount = info.Buffer.Count;
			scInfo.surface = m_Surface;
			//scInfo.imageFormat = format.format;
			//scInfo.imageColorSpace = format.colorSpace;
			scInfo.imageArrayLayers = 1;
			//scInfo.imageExtent = dimensions;
			scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			scInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			//scInfo.presentMode = mode;
			scInfo.queueFamilyIndexCount = 0;
			scInfo.pQueueFamilyIndices = nullptr;
			scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			scInfo.clipped = VK_TRUE;
			//scInfo.preTransform = scInfo.capabilites.currentTransform;

			result = vkCreateSwapchainKHR(device, &scInfo, nullptr, &m_Swapchain);
			if (result != VK_SUCCESS)
				return;
		}
	}
}