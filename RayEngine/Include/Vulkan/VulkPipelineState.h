#pragma once

#include "..\Graphics\IPipelineState.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkPipelineState : public IPipelineState
		{
		public:
			VulkPipelineState(const VulkPipelineState& other) = delete;
			VulkPipelineState& operator=(const VulkPipelineState& other) = delete;
			VulkPipelineState(VulkPipelineState&& other) = delete;
			VulkPipelineState& operator=(VulkPipelineState&& other) = delete;

		public:
			VulkPipelineState(IDevice* pdevice, const PipelineStateInfo& info);
			~VulkPipelineState();

			VkPipeline GetVkPipeline() const;
			VkRenderPass GetVkRenderPass() const;
			PIPELINE_TYPE GetPipelineType() const override final;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const PipelineStateInfo& info);

		private:
			IDevice * m_Device;
			VkPipeline m_Pipeline;
			VkRenderPass m_RenderPass;
			PIPELINE_TYPE m_Type;
		};
	}
}
