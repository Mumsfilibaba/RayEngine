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
#include "../../Include/Debug/Debug.h"
#include "../../Include/Vulkan/VulkSwapchain.h"
#include "../../Include/Vulkan/VulkFactory.h"
#include "../../Include/Vulkan/VulkDevice.h"

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
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkSwapchain::VulkSwapchain(IDevice* pDevice, const SwapchainDesc* pDesc)
			: m_Device(nullptr),
			m_CommandQueue(nullptr),
			m_Swapchain(VK_NULL_HANDLE),
			m_Surface(VK_NULL_HANDLE),
			m_Format(),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkSwapchain::~VulkSwapchain()
		{
			ReleaseObjects();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::Resize(int32 width, int32 height)
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::Present() const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<VulkDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::GetDesc(SwapchainDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkSwapchain::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkSwapchain::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkSwapchain::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::ReleaseObjects()
		{
			if (m_Surface != 0)
			{
				VkInstance instance = 0;// reinterpret_cast<VulkFactory*>(m_Factory)->GetVkInstance();

				vkDestroySurfaceKHR(instance, m_Surface, nullptr);
				m_Surface = 0;
			}

			if (m_Swapchain != 0)
			{
				VkDevice device = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroySwapchainKHR(device, m_Swapchain, nullptr);
				m_Swapchain = 0;
			}


			ReRelease_S(m_Device);

			for (int32 i = 0; i < static_cast<int32>(m_Textures.size()); i++)
			{
				if (m_Textures[i] != nullptr)
				{
					m_Textures[i]->InvalidateResource();

					ReRelease(m_Textures[i]);
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkSwapchain::Create(const SwapchainDesc* pDesc)
		{
			NativeWindowHandle handle = 0;

			VkInstance instance = 0;// m_Factory->GetVkInstance();
			VkResult result = VulkanCreateSwapchainSurface(instance, &m_Surface, handle);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not create surface.");
				return;
			}


			VkPhysicalDevice adapter = m_Device->GetVkPhysicalDevice();
			VkSurfaceCapabilitiesKHR capabilities = {};
			result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(adapter, m_Surface, &capabilities);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not create surface.");
				return;
			}


			uint32 count = 0;
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(adapter, m_Surface, &count, nullptr);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not get supported presentation modes.");
				return;
			}

			std::vector<VkPresentModeKHR> presentModes;
			presentModes.resize(count);
			result = vkGetPhysicalDeviceSurfacePresentModesKHR(adapter, m_Surface, &count, presentModes.data());
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not get supported presentation modes.");
				return;
			}


			m_Format = GetSupportedFormat(m_Device, m_Surface, ReToVkFormat(pDesc->BackBuffer.Format));
			if (m_Format.format == VK_FORMAT_UNDEFINED)
			{
				LOG_ERROR("Vulkan: Format is not supported.");
				return;
			}

			VkExtent2D size = GetSupportedSize(&capabilities, pDesc->Width, pDesc->Height);

			VkSwapchainCreateInfoKHR scInfo = {};
			scInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			scInfo.pNext = nullptr;
			scInfo.flags = 0;
			scInfo.oldSwapchain = 0;
			scInfo.minImageCount = pDesc->BackBuffer.Count;
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
				LOG_ERROR("Vulkan: Could not create swapchain.");
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkExtent2D VulkSwapchain::GetSupportedSize(const VkSurfaceCapabilitiesKHR* pCapabilities, int32 width, int32 height)
		{
			if (pCapabilities->currentExtent.width != std::numeric_limits<uint32_t>::max())
				return pCapabilities->currentExtent;

			VkExtent2D dimension = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

			dimension.width = std::max(pCapabilities->minImageExtent.width,
				std::min(pCapabilities->maxImageExtent.width, dimension.width));
			dimension.height = std::max(pCapabilities->minImageExtent.height,
				std::min(pCapabilities->maxImageExtent.height, dimension.height));

			return dimension;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkSurfaceFormatKHR VulkSwapchain::GetSupportedFormat(VulkDevice* pDevice, VkSurfaceKHR surface, VkFormat desiredFormat)
		{
			VkSurfaceFormatKHR format = {};
			format.format = VK_FORMAT_UNDEFINED;

			VkPhysicalDevice adapter = pDevice->GetVkPhysicalDevice();

			uint32 count = 0;
			VkResult result = vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &count, nullptr);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not get supported surface formats.");
				return format;
			}

			std::vector<VkSurfaceFormatKHR> formats;
			formats.resize(count);
			result = vkGetPhysicalDeviceSurfaceFormatsKHR(adapter, surface, &count, formats.data());
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not get supported surface formats.");
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