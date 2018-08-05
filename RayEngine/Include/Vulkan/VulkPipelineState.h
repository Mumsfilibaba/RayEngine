#pragma once

#include "..\Graphics\IPipelineState.h"
#include "VulkRootSignature.h"
#include "VulkShader.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		typedef std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription> VkInputElement;



		/////////////////////////////////////////////////////////////
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
			void CreateComputePipeline(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateGraphicsPipeline(IDevice* pDevice, const PipelineStateInfo& info);
			bool CreateRenderPass(IDevice* pDevice, const PipelineStateInfo& info);

		private:
			static VkPipelineShaderStageCreateInfo CreateVkPipelineShaderStageCreateInfo(const IShader* pShader);
			static VkInputElement CreateVkInputElement(const InputElementInfo& info, int32 location);
			static void CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& desc, const PipelineStateInfo& info);
			static void SetColorBlendAttachmentState(VkPipelineColorBlendAttachmentState& desc, const RenderTargetBlendInfo& info);
			static void SetRasterizerState(VkPipelineRasterizationStateCreateInfo& desc, const RasterizerStateInfo& info);
			static void SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo& desc, const DepthStencilStateInfo& info);

		private:
			IDevice* m_Device;
			const VulkRootSignature* m_RootSignature;
			VkPipeline m_Pipeline;
			VkRenderPass m_RenderPass;
			PIPELINE_TYPE m_Type;
		};
	}
}
