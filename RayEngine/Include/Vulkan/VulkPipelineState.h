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
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		typedef std::pair<VkVertexInputAttributeDescription, VkVertexInputBindingDescription> VkInputElement;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class VulkPipelineState final : public IPipelineState
		{
			RE_IMPLEMENT_INTERFACE(VulkPipelineState);

		public:
			VulkPipelineState(IDevice* pdevice, const PipelineStateDesc* pDesc);
			~VulkPipelineState();

			inline VkPipeline GetVkPipeline() const
			{
				return m_Pipeline;
			}
			
			inline VkRenderPass GetVkRenderPass() const
			{
				return m_RenderPass;
			}

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(PipelineStateDesc* pDesc) const;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const PipelineStateDesc* pDesc);

			void CreateComputePipeline();
			
			void CreateGraphicsPipeline();

			bool CreateRenderPass();

		private:
			static VkPipelineShaderStageCreateInfo CreateVkPipelineShaderStageCreateInfo(const IShader* pShader);
			
			static VkInputElement CreateVkInputElement(const InputElementDesc* pDesc, int32 location);
			
			static void CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo* pInfo, const PipelineStateDesc* pDesc);
			
			static void SetColorBlendAttachmentState(VkPipelineColorBlendAttachmentState* pInfo, const RenderTargetBlendDesc* pDesc);
			
			static void SetRasterizerState(VkPipelineRasterizationStateCreateInfo* pInfo, const RasterizerStateDesc* pDesc);
			
			static void SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo* pInfo, const DepthStencilStateDesc* pDesc);

		private:
			VulkDevice* m_Device;
			VulkRootLayout* m_RootLayout;
			
			VkPipeline m_Pipeline;
			VkRenderPass m_RenderPass;
			
			PipelineStateDesc m_Desc;
			
			IObject::CounterType m_References;
		};
	}
}
