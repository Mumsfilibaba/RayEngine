#include "..\..\Include\Vulkan\VulkTexture.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkTexture::VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
			: m_Device(nullptr),
			m_Image(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, pInitialData, info);
		}



		/////////////////////////////////////////////////////////////
		VulkTexture::VulkTexture(IDevice* pDevice, VkImage image)
			: m_Device(nullptr),
			m_Image(image)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		VulkTexture::~VulkTexture()
		{
			if (m_Image != nullptr)
			{
				VkDevice device = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();
				
				vkDestroyImage(device, m_Image, nullptr);
				m_Image = nullptr;
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void VulkTexture::InvalidateResource()
		{
			m_Image = nullptr;
		}



		/////////////////////////////////////////////////////////////
		VkImage VulkTexture::GetVkImage() const
		{
			return m_Image;
		}



		/////////////////////////////////////////////////////////////
		RESOURCE_STATE VulkTexture::GetResourceState() const
		{
			return RESOURCE_STATE_UNKNOWN;
		}



		/////////////////////////////////////////////////////////////
		IDevice* VulkTexture::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void VulkTexture::Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			using namespace System;
			
			VkImageCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			desc.pNext = nullptr;
			//desc.pQueueFamilyIndices = ;
			//desc.queueFamilyIndexCount = ;
			//desc.initialLayout = ;
			//desc.samples = ;
			//desc.sharingMode = ;
			//desc.tiling = ;
			//desc.usage = ;

			if (info.Type == TEXTURE_TYPE_1D)
			{
				desc.imageType = VK_IMAGE_TYPE_1D;
			}
			else if (info.Type == TEXTURE_TYPE_2D)
			{
				desc.imageType = VK_IMAGE_TYPE_2D;
			}
			else if (info.Type == TEXTURE_TYPE_3D)
			{
				desc.imageType = VK_IMAGE_TYPE_3D;
			}

			desc.format = ReToVkFormat(info.Format);
			//desc.flags = ;
			desc.arrayLayers = info.DepthOrArraySize;
			desc.mipLevels = info.MipLevels;
			desc.extent.width = info.Width;
			desc.extent.height = info.Height;
			desc.extent.depth = info.DepthOrArraySize;

			VkDevice vkDevice = reinterpret_cast<VulkDevice*>(pDevice)->GetVkDevice();
			VkResult result = vkCreateImage(vkDevice, &desc, nullptr, &m_Image);
			if (result != VK_SUCCESS)
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create image.");
				return;
			}
		}
	}
}