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

#include "..\..\Include\Vulkan\VulkTexture.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkTexture::VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
			: m_Device(nullptr),
			m_Image(VK_NULL_HANDLE)
		{
			AddRef();
			m_Device = QueryVulkDevice(pDevice);

			Create(pInitialData, info);
		}



		/////////////////////////////////////////////////////////////
		VulkTexture::VulkTexture(IDevice* pDevice, VkImage image)
			: m_Device(nullptr),
			m_Image(image)
		{
			AddRef();
			m_Device = QueryVulkDevice(pDevice);
		}



		/////////////////////////////////////////////////////////////
		VulkTexture::~VulkTexture()
		{
			if (m_Image != VK_NULL_HANDLE)
			{
				VkDevice device = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();
				
				vkDestroyImage(device, m_Image, nullptr);
				m_Image = VK_NULL_HANDLE;
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void VulkTexture::InvalidateResource()
		{
			m_Image = VK_NULL_HANDLE;
		}



		/////////////////////////////////////////////////////////////
		VkImage VulkTexture::GetVkImage() const
		{
			return m_Image;
		}



		/////////////////////////////////////////////////////////////
		void VulkTexture::SetName(const std::string& name)
		{
		}



		/////////////////////////////////////////////////////////////
		void VulkTexture::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryVulkDevice(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void VulkTexture::Create(const ResourceData* const pInitialData, const TextureInfo& info)
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

			VkDevice vkDevice = m_Device->GetVkDevice();
			VkResult result = vkCreateImage(vkDevice, &desc, nullptr, &m_Image);
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create image.");
			}
		}
	}
}