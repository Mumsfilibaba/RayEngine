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

#include "..\..\Include\Vulkan\VulkRenderTargetView.h"
#include "..\..\Include\Vulkan\VulkRenderTargetView.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRenderTargetView::VulkRenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info)
			: VulkImageView(pDevice),
			mReferences(0)
		{
			AddRef();
			Create(info);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRenderTargetView::~VulkRenderTargetView()
		{
		}



		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRenderTargetView::SetName(const std::string & name)
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRenderTargetView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<VulkDevice>();
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRenderTargetView::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRenderTargetView::Release()
		{
			IObject::CounterType counter = mReferences--;
			if (mReferences < 1)
				delete this;
			
			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRenderTargetView::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRenderTargetView::Create(const RenderTargetViewInfo& info)
		{
			using namespace System;

			VkImage vkImage = reinterpret_cast<const VulkTexture*>(info.pResource)->GetVkImage();
			VkImageViewCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;
			
			desc.image = vkImage;
			desc.format = ReToVkFormat(info.Format);

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
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create ImageView. ");
			}
		}
	}
}