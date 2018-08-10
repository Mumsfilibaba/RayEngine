#include <limits>
#include <algorithm>
#include "..\..\Include\Vulkan\VulkSwapchain.h"
#include "..\..\Include\Vulkan\VulkFactory.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#if defined(max)
#undef max
#endif

#if defined(min)
#undef min
#endif
#endif

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkSwapchain::VulkSwapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info)
			: m_Factory(nullptr),
			m_Device(nullptr),
			m_CommandQueue(nullptr),
			m_Swapchain(0),
			m_Surface(0),
			m_Format()
		{
			AddRef();
			m_Factory = reinterpret_cast<VulkFactory*>(pFactory->QueryReference());
			m_Device = QueryVulkDevice(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		VulkSwapchain::~VulkSwapchain()
		{
			ReleaseObjects();
		}



		/////////////////////////////////////////////////////////////
		VkSwapchainKHR VulkSwapchain::GetVkSwapchainKHR() const
		{
			return m_Swapchain;
		}



		/////////////////////////////////////////////////////////////
		VkSurfaceKHR VulkSwapchain::GetVkSurfaceKHR() const
		{
			return m_Surface;
		}



		/////////////////////////////////////////////////////////////
		int32 VulkSwapchain::GetCurrentBuffer() const
		{
			return 0;
		}



		/////////////////////////////////////////////////////////////
		ITexture* VulkSwapchain::GetBuffer(int32 index)
		{
			return m_Textures[index];
		}



		/////////////////////////////////////////////////////////////
		const ITexture* VulkSwapchain::GetBuffer(int32 index) const
		{
			return m_Textures[index];
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::Present() const
		{
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = reinterpret_cast<VulkFactory*>(m_Factory->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::QueryCommandQueue(ICommandQueue** ppCommandQueue) const
		{
			(*ppCommandQueue) = reinterpret_cast<ICommandQueue*>(m_CommandQueue->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::ReleaseObjects()
		{
			if (m_Surface != 0)
			{
				VkInstance instance = reinterpret_cast<VulkFactory*>(m_Factory)->GetVkInstance();

				vkDestroySurfaceKHR(instance, m_Surface, nullptr);
				m_Surface = 0;
			}

			if (m_Swapchain != 0)
			{
				VkDevice device = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
				m_Swapchain = 0;
			}


			ReRelease_S(m_Factory);
			ReRelease_S(m_Device);
			ReRelease_S(m_CommandQueue);

			for (int32 i = 0; i < m_Textures.size(); i++)
			{
				if (m_Textures[i] != nullptr)
				{
					m_Textures[i]->InvalidateResource();

					ReRelease(m_Textures[i]);
				}
			}
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::Create(const SwapchainInfo& info)
		{
			using namespace System;

			VkInstance instance = m_Factory->GetVkInstance();
			VkResult result = VulkanCreateSwapchainSurface(instance, &m_Surface, info.pWindow->GetImplementation());
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create surface.");
				return;
			}


			VkPhysicalDevice adapter = m_Device->GetVkPhysicalDevice();
			VkSurfaceCapabilitiesKHR capabilities = {};
			result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(adapter, m_Surface, &capabilities);
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create surface.");
				return;
			}


			uint32 count = 0;
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(adapter, m_Surface, &count, nullptr);
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not get supported presentation modes.");
				return;
			}

			std::vector<VkPresentModeKHR> presentModes;
			presentModes.resize(count);
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(adapter, m_Surface, &count, presentModes.data());
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not get supported presentation modes.");
				return;
			}


			m_Format = GetSupportedFormat(m_Device, m_Surface, ReToVkFormat(info.Buffer.Format));
			if (m_Format.format == VK_FORMAT_UNDEFINED)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Format is not supported.");
				return;
			}

			VkExtent2D size = GetSupportedSize(capabilities, info.pWindow->GetWidth(), info.pWindow->GetHeight());

			VkSwapchainCreateInfoKHR scInfo = {};
			scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			scInfo.pNext = nullptr;
			scInfo.flags = 0;
			scInfo.oldSwapchain = 0;
			scInfo.minImageCount = info.Buffer.Count;
			scInfo.surface = m_Surface;
			scInfo.imageFormat = m_Format.format;
			scInfo.imageColorSpace = m_Format.colorSpace;
			scInfo.imageArrayLayers = 1;
			scInfo.imageExtent = size;
			scInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			scInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			scInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR; //For now
			scInfo.queueFamilyIndexCount = 0;
			scInfo.pQueueFamilyIndices = nullptr;
			scInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			scInfo.clipped = VK_TRUE;
			scInfo.preTransform = capabilities.currentTransform;

			VkDevice device = m_Device->GetVkDevice();
			result = vkCreateSwapchainKHR(device, &scInfo, nullptr, &m_Swapchain);
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create swapchain.");
				return;
			}


			vkGetSwapchainImagesKHR(device, m_Swapchain, &count, nullptr);
			
			std::vector<VkImage> swapchainImages;
			swapchainImages.resize(count);
			vkGetSwapchainImagesKHR(device, m_Swapchain, &count, swapchainImages.data());

			m_Textures.resize(count);
			for (int32 i = 0; i < swapchainImages.size(); i++)
				m_Textures[i] = new VulkTexture(m_Device, swapchainImages[i]);
		}



		/////////////////////////////////////////////////////////////
		VkExtent2D VulkSwapchain::GetSupportedSize(const VkSurfaceCapabilitiesKHR& capabilities, int32 width, int32 height)
		{
			if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
				return capabilities.currentExtent;

			VkExtent2D dimension = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			dimension.width = std::max(capabilities.minImageExtent.width,
				std::min(capabilities.maxImageExtent.width, dimension.width));
			dimension.height = std::max(capabilities.minImageExtent.height,
				std::min(capabilities.maxImageExtent.height, dimension.height));

			return dimension;
		}



		/////////////////////////////////////////////////////////////
		VkSurfaceFormatKHR VulkSwapchain::GetSupportedFormat(VulkDevice* pDevice, VkSurfaceKHR surface, VkFormat desiredFormat)
		{
			using namespace System;

			VkSurfaceFormatKHR format = {};
			format.format = VK_FORMAT_UNDEFINED;

			VkPhysicalDevice adapter = pDevice->GetVkPhysicalDevice();

			uint32 count = 0;
			VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &count, nullptr);
			if (result != VK_SUCCESS)
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not get supported surface formats.");
				return format;
			}

			std::vector<VkSurfaceFormatKHR> formats;
			formats.resize(count);
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &count, formats.data());
			if (result != VK_SUCCESS)
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not get supported surface formats.");
				return format;
			}


			for (int32 i = 0; i < static_cast<int32>(formats.size()); i++)
			{
				if (formats[i].format == desiredFormat)
				{
					format = formats[i];
					break;
				}
			}

			return format;
		}
	}
}