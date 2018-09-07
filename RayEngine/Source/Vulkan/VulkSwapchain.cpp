/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

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
		void VulkSwapchain::QueryBuffer(ITexture** ppBuffer, int32 index) const
		{
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::Present() const
		{
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::SetName(const std::string& name)
		{
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryVulkDevice(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = reinterpret_cast<VulkFactory*>(m_Factory->QueryReference());
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
			//Release DeviceContext

			for (int32 i = 0; i < static_cast<int32>(m_Textures.size()); i++)
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
			VkResult result = VulkanCreateSwapchainSurface(instance, &m_Surface, info.WindowHandle);
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

			VkExtent2D size = GetSupportedSize(capabilities, info.Buffer.Width, info.Buffer.Height);

			VkSwapchainCreateInfoKHR scInfo = {};
			scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			scInfo.pNext = nullptr;
			scInfo.flags = 0;
			scInfo.oldSwapchain = 0;
			scInfo.minImageCount = info.Count;
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
			for (int32 i = 0; i < static_cast<int32>(swapchainImages.size()); i++)
			{
				m_Textures[i] = new VulkTexture(m_Device, swapchainImages[i]);
			}
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