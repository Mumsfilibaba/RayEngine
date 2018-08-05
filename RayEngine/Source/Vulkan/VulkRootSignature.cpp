#include "..\..\Include\Vulkan\VulkRootSignature.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkRootSignature::VulkRootSignature(IDevice* pDevice, const RootSignatureInfo& info)
			: m_Device(nullptr),
			m_Layout(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		VulkRootSignature::~VulkRootSignature()
		{
			if (m_Layout != nullptr)
			{
				VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();

				vkDestroyPipelineLayout(vkDevice, m_Layout, nullptr);
				m_Layout = nullptr;
			}

			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		VkPipelineLayout VulkRootSignature::GetVkPipelineLayout() const
		{
			return m_Layout;
		}

		

		/////////////////////////////////////////////////////////////
		IDevice* VulkRootSignature::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void VulkRootSignature::Create(IDevice* pDevice, const RootSignatureInfo& info)
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