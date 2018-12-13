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
#include <Vulkan/VulkRenderTargetView.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRenderTargetView::VulkRenderTargetView(IDevice* pDevice, const RenderTargetViewDesc* pDesc)
			: VulkImageView(pDevice),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRenderTargetView::~VulkRenderTargetView()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRenderTargetView::GetDesc(RenderTargetViewDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType VulkRenderTargetView::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType VulkRenderTargetView::Release()
		{
			CounterType counter = --m_References;
			if (m_References < 1)
				delete this;
			
			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRenderTargetView::Create(const RenderTargetViewDesc* pDesc)
		{
			VkImage vkImage = reinterpret_cast<const VulkTexture*>(pDesc->pResource)->GetVkImage();
			VkImageViewCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;
			
			desc.image = vkImage;
			desc.format = ReToVkFormat(pDesc->Format);

			desc.viewType = VK_IMAGE_VIEW_TYPE_2D;
			
			desc.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			desc.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			desc.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			desc.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

			desc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			desc.subresourceRange.baseArrayLayer = 0;
			desc.subresourceRange.layerCount = 1;
			desc.subresourceRange.baseMipLevel = 0;
			desc.subresourceRange.levelCount = 1;
			

			VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();
			VkResult result = vkCreateImageView(vkDevice, &desc, nullptr, &m_View);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not create ImageView. ");
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}