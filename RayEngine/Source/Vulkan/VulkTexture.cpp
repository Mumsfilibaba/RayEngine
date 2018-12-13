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

#include <RayEngine.h>
#include <Vulkan/VulkDevice.h>
#include <Vulkan/VulkTexture.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkTexture::VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureDesc* pDesc)
			: m_Device(nullptr),
			m_Image(VK_NULL_HANDLE),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(pInitialData, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkTexture::VulkTexture(IDevice* pDevice, VkImage image)
			: m_Device(nullptr),
			m_Image(image),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkTexture::InvalidateResource()
		{
			m_Image = VK_NULL_HANDLE;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkTexture::GetDesc(TextureDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType VulkTexture::Release()
		{
			CounterType counter = --m_References;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType VulkTexture::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkTexture::Create(const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
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

			if (pDesc->Type == TEXTURE_TYPE_1D)
			{
				desc.imageType = VK_IMAGE_TYPE_1D;
			}
			else if (pDesc->Type == TEXTURE_TYPE_2D)
			{
				desc.imageType = VK_IMAGE_TYPE_2D;
			}
			else if (pDesc->Type == TEXTURE_TYPE_3D)
			{
				desc.imageType = VK_IMAGE_TYPE_3D;
			}

			desc.format = ReToVkFormat(pDesc->Format);
			//desc.flags = ;
			desc.arrayLayers = pDesc->DepthOrArraySize;
			desc.mipLevels = pDesc->MipLevels;
			desc.extent.width = pDesc->Width;
			desc.extent.height = pDesc->Height;
			desc.extent.depth = pDesc->DepthOrArraySize;

			VkDevice vkDevice = m_Device->GetVkDevice();
			VkResult result = vkCreateImage(vkDevice, &desc, nullptr, &m_Image);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not create image.");
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}