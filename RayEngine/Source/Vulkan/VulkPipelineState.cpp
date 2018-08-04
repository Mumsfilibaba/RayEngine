#include "..\..\Include\Vulkan\VulkPipelineState.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkPipelineState::VulkPipelineState(IDevice* pDevice, const PipelineStateInfo& info)
			: m_Device(nullptr),
			m_RenderPass(nullptr),
			m_Pipeline(nullptr),
			m_Type(PIPELINE_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		VulkPipelineState::~VulkPipelineState()
		{
			VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();
			if (m_Pipeline != nullptr)
			{
				vkDestroyPipeline(vkDevice, m_Pipeline, nullptr);
				m_Pipeline = nullptr;
			}

			if (m_RenderPass != nullptr)
			{
				vkDestroyRenderPass(vkDevice, m_RenderPass, nullptr);
				m_RenderPass = nullptr;
			}

			ReRelease_S(m_Device);
		}


		/////////////////////////////////////////////////////////////
		VkPipeline VulkPipelineState::GetVkPipeline() const
		{
			return m_Pipeline;
		}



		/////////////////////////////////////////////////////////////
		VkRenderPass VulkPipelineState::GetVkRenderPass() const
		{
			return m_RenderPass;
		}



		/////////////////////////////////////////////////////////////
		PIPELINE_TYPE VulkPipelineState::GetPipelineType() const
		{
			return m_Type;
		}


		/////////////////////////////////////////////////////////////
		IDevice* VulkPipelineState::GetDevice() const
		{
			return m_Device;
		}


		/////////////////////////////////////////////////////////////
		void VulkPipelineState::Create(IDevice* pDevice, const PipelineStateInfo& info)
		{
		}
	}
}