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
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const PipelineStateInfo& info);
			void CreateComputePipeline(const PipelineStateInfo& info);
			void CreateGraphicsPipeline(const PipelineStateInfo& info);
			bool CreateRenderPass(const PipelineStateInfo& info);

		private:
			static VkPipelineShaderStageCreateInfo CreateVkPipelineShaderStageCreateInfo(const IShader* pShader);
			static VkInputElement CreateVkInputElement(const InputElementInfo& info, int32 location);
			static void CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& desc, const PipelineStateInfo& info);
			static void SetColorBlendAttachmentState(VkPipelineColorBlendAttachmentState& desc, const RenderTargetBlendInfo& info);
			static void SetRasterizerState(VkPipelineRasterizationStateCreateInfo& desc, const RasterizerStateInfo& info);
			static void SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo& desc, const DepthStencilStateInfo& info);

		private:
			VulkDevice* m_Device;
			const VulkRootSignature* m_RootSignature;
			VkPipeline m_Pipeline;
			VkRenderPass m_RenderPass;
			PIPELINE_TYPE m_Type;
		};
	}
}
