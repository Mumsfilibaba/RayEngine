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

#include "..\..\Include\Vulkan\VulkImageView.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkImageView::VulkImageView(IDevice* pDevice)
			: m_Device(nullptr),
			m_View(VK_NULL_HANDLE)
		{
			m_Device = pDevice->QueryReference<VulkDevice>();
		}



		/////////////////////////////////////////////////////////////
		VulkImageView::~VulkImageView()
		{
			if (m_View != VK_NULL_HANDLE)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyImageView(vkDevice, m_View, nullptr);
				m_View = VK_NULL_HANDLE;
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		VkImageView VulkImageView::GetVkImageView() const
		{
			return m_View;
		}
	}
}