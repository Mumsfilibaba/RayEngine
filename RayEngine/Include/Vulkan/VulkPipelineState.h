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

#pragma once
#include "..\Graphics\IPipelineState.h"
#include "VulkRootLayout.h"
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

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

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
			VulkRootLayout* m_RootLayout;
			
			VkPipeline m_Pipeline;
			VkRenderPass m_RenderPass;
			
			PIPELINE_TYPE m_Type;
			
			IObject::CounterType m_References;
		};
	}
}
