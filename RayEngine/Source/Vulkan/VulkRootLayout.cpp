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

#include "RayEngine.h"
#include "../../Include/Vulkan/VulkRootLayout.h"
#include "../../Include/Vulkan/VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRootLayout::VulkRootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc)
			: m_Device(nullptr),
			m_Layout(VK_NULL_HANDLE),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(pDevice, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRootLayout::~VulkRootLayout()
		{
			if (m_Layout != VK_NULL_HANDLE)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyPipelineLayout(vkDevice, m_Layout, nullptr);
				m_Layout = VK_NULL_HANDLE;
			}

			ReRelease_S(m_Device);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRootLayout::SetName(const std::string& name)
		{
			//Not relevant
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRootLayout::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<VulkDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRootLayout::GetDesc(RootLayoutDesc * pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRootLayout::GetReferenceCount() const
		{
			return m_References;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRootLayout::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRootLayout::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRootLayout::Create(IDevice* pDevice, const RootLayoutDesc* pDesc)
		{
			VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
			pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			pipelineLayoutInfo.pNext = nullptr;
			pipelineLayoutInfo.flags = 0;

			pipelineLayoutInfo.setLayoutCount = 0;
			pipelineLayoutInfo.pSetLayouts = nullptr;
			pipelineLayoutInfo.pushConstantRangeCount = 0;
			pipelineLayoutInfo.pPushConstantRanges = nullptr;

			VkDevice vkDevice = reinterpret_cast<VulkDevice*>(pDevice)->GetVkDevice();
			VkResult result = vkCreatePipelineLayout(vkDevice, &pipelineLayoutInfo, nullptr, &m_Layout);
			if (result != VK_SUCCESS)
			{
				LOG_ERROR("Vulkan: Could not create pipelinelayout");
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}