#include <vector>
#include "..\..\Include\Vulkan\VulkSwapchain.h"
#include "..\..\Include\Vulkan\VulkFactory.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

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
			m_Format(),
			m_ReferenceCount(0)
		{
			AddRef();

			m_Factory = reinterpret_cast<IFactory*>(pFactory->QueryReference());
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		VulkSwapchain::VulkSwapchain(VulkSwapchain&& other)
			: m_Factory(other.m_Factory),
			m_Device(other.m_Device),
			m_CommandQueue(other.m_CommandQueue),
			m_Swapchain(other.m_Swapchain),
			m_Surface(other.m_Surface),
			m_Format(other.m_Format),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Factory = nullptr;
			other.m_Device = nullptr;
			other.m_CommandQueue = nullptr;
			other.m_Swapchain = nullptr;
			other.m_Surface = nullptr;
			other.m_ReferenceCount = 0;

			other.m_Format.colorSpace = VK_COLOR_SPACE_END_RANGE_KHR;
			other.m_Format.format = VK_FORMAT_UNDEFINED;
		}



		/////////////////////////////////////////////////////////////
		VulkSwapchain::~VulkSwapchain()
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


			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}


			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}


			if (m_CommandQueue != nullptr)
			{
				m_CommandQueue->Release();
				m_CommandQueue = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		VulkSwapchain& VulkSwapchain::operator=(VulkSwapchain&& other)
		{
			if (this != &other)
			{
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}

				if (m_Factory != nullptr)
				{
					m_Factory->Release();
					m_Factory = nullptr;
				}

				if (m_CommandQueue != nullptr)
				{
					m_CommandQueue->Release();
					m_CommandQueue = nullptr;
				}
			}

			return *this;
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
			return int32();
		}



		/////////////////////////////////////////////////////////////
		ITexture* VulkSwapchain::GetBuffer(int32 index)
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		const ITexture* VulkSwapchain::GetBuffer(int32 index) const
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::Present() const
		{
		}



		/////////////////////////////////////////////////////////////
		IFactory* VulkSwapchain::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		ICommandQueue* VulkSwapchain::GetCommandQueue() const
		{
			return m_CommandQueue;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* VulkSwapchain::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VulkSwapchain::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 VulkSwapchain::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void VulkSwapchain::Create(IDevice* pDevice, const SwapchainInfo& info)
		{
			//m_Surface = surface;
			VkPhysicalDevice adapter = reinterpret_cast<VulkDevice*>(pDevice)->GetVkPhysicalDevice();

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

			VkDevice device = reinterpret_cast<VulkDevice*>(pDevice)->GetVkDevice();
			result = vkCreateSwapchainKHR(device, &scInfo, nullptr, &m_Swapchain);
			if (result != VK_SUCCESS)
				return;
		}
	}
}