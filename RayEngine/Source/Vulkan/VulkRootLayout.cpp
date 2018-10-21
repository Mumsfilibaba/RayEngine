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

#include "..\..\Include\Vulkan\VulkRootLayout.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkRootLayout::VulkRootLayout(IDevice* pDevice, const RootLayoutDesc& info)
			: m_Device(nullptr),
			m_Layout(VK_NULL_HANDLE),
			mReferences(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(pDevice, info);
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
		VkPipelineLayout VulkRootLayout::GetVkPipelineLayout() const
		{
			return m_Layout;
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
		IObject::CounterType VulkRootLayout::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRootLayout::Release()
		{
			IObject::CounterType counter = mReferences--;
			if (mReferences < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkRootLayout::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkRootLayout::Create(IDevice* pDevice, const RootLayoutDesc& info)
		{
			using namespace System;

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
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create pipelinelayout");
				return;
			}
		}
	}
}