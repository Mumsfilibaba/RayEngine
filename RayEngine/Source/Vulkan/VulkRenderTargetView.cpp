#include "..\..\Include\Vulkan\VulkRenderTargetView.h"
#include "..\..\Include\Vulkan\VulkTexture.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkRenderTargetView::VulkRenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info)
			: VulkImageView(pDevice)
		{
			AddRef();
			Create(info);
		}



		/////////////////////////////////////////////////////////////
		VulkRenderTargetView::~VulkRenderTargetView()
		{
		}



		/////////////////////////////////////////////////////////////
		void VulkRenderTargetView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryVulkDevice(m_Device);
		}



		/////////////////////////////////////////////////////////////
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