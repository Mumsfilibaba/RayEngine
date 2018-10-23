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

#include <vector>
#include <utility>
#include "..\..\Include\Vulkan\VulkPipelineState.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkPipelineState::VulkPipelineState(IDevice* pDevice, const PipelineStateDesc* pDesc)
			: m_Device(nullptr),
			m_RootLayout(nullptr),
			m_RenderPass(VK_NULL_HANDLE),
			m_Pipeline(VK_NULL_HANDLE),
			m_Type(PIPELINE_TYPE_UNKNOWN),
			m_References(0)
		{
			AddRef();
			m_Device = pDevice->QueryReference<VulkDevice>();

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VulkPipelineState::~VulkPipelineState()
		{
			VkDevice vkDevice = reinterpret_cast<VulkDevice*>(m_Device)->GetVkDevice();
			if (m_Pipeline != VK_NULL_HANDLE)
			{
				vkDestroyPipeline(vkDevice, m_Pipeline, nullptr);
				m_Pipeline = VK_NULL_HANDLE;
			}

			if (m_RenderPass != VK_NULL_HANDLE)
			{
				vkDestroyRenderPass(vkDevice, m_RenderPass, nullptr);
				m_RenderPass = VK_NULL_HANDLE;
			}

			ReRelease_S(m_Device);
			ReRelease_S(m_RootLayout);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PIPELINE_TYPE VulkPipelineState::GetPipelineType() const
		{
			return m_Type;
		}


		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::SetName(const std::string& name)
		{
			// not relvant yet
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<VulkDevice>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkPipelineState::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkPipelineState::Release()
		{
			IObject::CounterType counter = m_References--;
			if (m_References < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType VulkPipelineState::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::Create(const PipelineStateDesc* pDesc)
		{
			if (pDesc->Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsPipeline(pDesc);
			else if (pDesc->Type == PIPELINE_TYPE_COMPUTE)
				CreateComputePipeline(pDesc);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::SetRasterizerState(VkPipelineRasterizationStateCreateInfo* pInfo, const RasterizerStateDesc* pDesc)
		{
			pInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			pInfo->flags = 0;
			pInfo->pNext = nullptr;
			//TODO: Look up depthclamp
			pInfo->depthClampEnable = pDesc->DepthClipEnable ? VK_TRUE : VK_FALSE;
			pInfo->rasterizerDiscardEnable = VK_FALSE;
			
			if (pDesc->FillMode == FILL_MODE_SOLID)
				pInfo->polygonMode = VK_POLYGON_MODE_FILL;
			else if (pDesc->FillMode == FILL_MODE_WIREFRAME)
				pInfo->polygonMode = VK_POLYGON_MODE_LINE;

			if (pDesc->CullMode == CULL_MODE_BACK)
				pInfo->cullMode = VK_CULL_MODE_BACK_BIT;
			else if (pDesc->CullMode == CULL_MODE_FRONT)
				pInfo->cullMode = VK_CULL_MODE_FRONT_BIT;
			else if (pDesc->CullMode == CULL_MODE_NONE)
				pInfo->cullMode = VK_CULL_MODE_NONE;

			//TODO: Wider?
			pInfo->lineWidth = 1.0f;

			pInfo->frontFace = pDesc->FrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
			
			pInfo->depthBiasEnable = (pDesc->DepthBias > 0);
			pInfo->depthBiasClamp = pDesc->DepthBiasClamp;
			pInfo->depthBiasSlopeFactor = pDesc->SlopeScaleDepthBias;
			pInfo->depthBiasConstantFactor = static_cast<float>(pDesc->DepthBias);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo* pInfo, const DepthStencilStateDesc* pDesc)
		{
			pInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			pInfo->pNext = nullptr;
			pInfo->flags = 0;

			pInfo->depthTestEnable = pDesc->DepthEnable ? VK_TRUE : VK_FALSE;
			pInfo->depthWriteEnable = pInfo->depthTestEnable;

			pInfo->depthCompareOp = ReToVkCompareOp(pDesc->DepthFunc);
			pInfo->depthBoundsTestEnable = VK_FALSE; //Maybe?

			pInfo->stencilTestEnable = pDesc->StencilEnable ? VK_TRUE : VK_FALSE;

			pInfo->front = ReToVkStencilOpState(pDesc->FrontFace);
			pInfo->back = ReToVkStencilOpState(pDesc->BackFace);
			
			pInfo->minDepthBounds = 0.0f;
			pInfo->maxDepthBounds = 0.0f;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::CreateComputePipeline(const PipelineStateDesc* pDesc)
		{
			//TODO: Implement ComputeShaders
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::CreateGraphicsPipeline(const PipelineStateDesc* pDesc)
		{
			using namespace System;

			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			if (pDesc->GraphicsPipeline.pVertexShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(pDesc->GraphicsPipeline.pVertexShader));

			if (pDesc->GraphicsPipeline.pHullShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(pDesc->GraphicsPipeline.pHullShader));

			if (pDesc->GraphicsPipeline.pDomainShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(pDesc->GraphicsPipeline.pDomainShader));

			if (pDesc->GraphicsPipeline.pGeometryShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(pDesc->GraphicsPipeline.pGeometryShader));

			if (pDesc->GraphicsPipeline.pPixelShader!= nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(pDesc->GraphicsPipeline.pPixelShader));



			int32 elementCount = pDesc->GraphicsPipeline.InputLayout.ElementCount;
			std::vector<VkVertexInputBindingDescription> vertexBindings;
			vertexBindings.resize(elementCount);
			
			std::vector<VkVertexInputAttributeDescription> vertexAttributes;
			vertexAttributes.resize(elementCount);

			for (int32 i = 0; i < elementCount; i++)
			{
				VkInputElement element = CreateVkInputElement(&pDesc->GraphicsPipeline.InputLayout.pElements[i], i);

				vertexAttributes[i] = element.first;
				vertexBindings[i] = element.second;
			}

			VkPipelineVertexInputStateCreateInfo inputState = {};
			inputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			inputState.pNext = nullptr;
			inputState.flags = 0;

			inputState.vertexBindingDescriptionCount = elementCount;
			inputState.pVertexBindingDescriptions = vertexBindings.data();
			inputState.vertexAttributeDescriptionCount = elementCount;
			inputState.pVertexAttributeDescriptions = vertexAttributes.data();



			VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {};
			CreateInputAssemblyStateInfo(&inputAssemblyState, pDesc);



			VkPipelineViewportStateCreateInfo viewportState = {};
			viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			viewportState.flags = 0;
			viewportState.pNext = nullptr;

			viewportState.viewportCount = 0;
			viewportState.pViewports = nullptr;
			viewportState.scissorCount = 0;
			viewportState.pScissors = nullptr;



			VkDynamicState dynamicStates[] = {
				VK_DYNAMIC_STATE_VIEWPORT,
				VK_DYNAMIC_STATE_SCISSOR,
			};

			VkPipelineDynamicStateCreateInfo dynamicState = {};
			dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			dynamicState.pNext = nullptr;
			dynamicState.flags = 0;

			dynamicState.dynamicStateCount = 2;
			dynamicState.pDynamicStates = dynamicStates;



			std::vector<VkPipelineColorBlendAttachmentState> colorBlendAttachments;
			colorBlendAttachments.resize(pDesc->GraphicsPipeline.RenderTargetCount);

			for (int32 i = 0; i < pDesc->GraphicsPipeline.RenderTargetCount; i++)
				SetColorBlendAttachmentState(&colorBlendAttachments[i], &pDesc->GraphicsPipeline.BlendState.RenderTargets[i]);

			VkPipelineColorBlendStateCreateInfo blendState = {};
			blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			blendState.pNext = nullptr;
			blendState.flags = 0;

			blendState.logicOpEnable = pDesc->GraphicsPipeline.BlendState.LogicOpEnable ? VK_TRUE : VK_FALSE;
			blendState.logicOp = VK_LOGIC_OP_NO_OP;
			blendState.attachmentCount = pDesc->GraphicsPipeline.RenderTargetCount;
			blendState.pAttachments = colorBlendAttachments.data();

			blendState.blendConstants[0] = pDesc->GraphicsPipeline.BlendState.BlendFactor[0];
			blendState.blendConstants[1] = pDesc->GraphicsPipeline.BlendState.BlendFactor[1];
			blendState.blendConstants[2] = pDesc->GraphicsPipeline.BlendState.BlendFactor[2];
			blendState.blendConstants[3] = pDesc->GraphicsPipeline.BlendState.BlendFactor[3];



			VkPipelineRasterizationStateCreateInfo rasterizerState = {};
			SetRasterizerState(&rasterizerState, &pDesc->GraphicsPipeline.RasterizerState);



			VkPipelineMultisampleStateCreateInfo multisamplingState = {};
			multisamplingState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			multisamplingState.flags = 0;
			multisamplingState.pNext = nullptr;

			multisamplingState.sampleShadingEnable = VK_FALSE;
			multisamplingState.minSampleShading = 1.0f;

			multisamplingState.alphaToCoverageEnable = VK_FALSE;
			multisamplingState.alphaToOneEnable = VK_FALSE;

			//TODO: Fix msaa here
			multisamplingState.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			VkSampleMask sampleMask = 0xFFFFFFFF;
			multisamplingState.pSampleMask = &sampleMask;
			//multisamplingState.rasterizationSamples = ReToVkSampleCount(pDesc->GraphicsPipeline.SampleCount);


			
			VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
			SetDepthStencilState(&depthStencilState, &pDesc->GraphicsPipeline.DepthStencilState);


			if (!CreateRenderPass(pDesc))
				return;


			VkGraphicsPipelineCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;

			desc.stageCount = static_cast<uint32_t>(shaderStages.size());
			desc.pStages = shaderStages.data();

			desc.pVertexInputState = &inputState;
			desc.pInputAssemblyState = &inputAssemblyState;
			//TODO: Enable tesselation
			desc.pTessellationState = nullptr;
			desc.pViewportState = &viewportState;
			desc.pRasterizationState = &rasterizerState;
			desc.pMultisampleState = &multisamplingState;
			desc.pDepthStencilState = &depthStencilState;
			desc.pColorBlendState = &blendState;
			desc.pDynamicState = &dynamicState;

			m_RootLayout = pDesc->pRootLayout->QueryReference<VulkRootLayout>();
			VkPipelineLayout vkLayout = m_RootLayout->GetVkPipelineLayout();
			desc.layout = vkLayout;

			//TODO: Multiple subpasses?
			desc.renderPass = m_RenderPass;
			desc.subpass = 0;

			desc.basePipelineHandle = VK_NULL_HANDLE;
			desc.basePipelineIndex = -1;


			VkDevice vkDevice = m_Device->GetVkDevice();
			VkResult result = vkCreateGraphicsPipelines(vkDevice, VK_NULL_HANDLE, 1, &desc, nullptr, &m_Pipeline);
			if (result != VK_SUCCESS)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create pipeline");
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool VulkPipelineState::CreateRenderPass(const PipelineStateDesc* pDesc)
		{
			using namespace System;

			std::vector<VkAttachmentDescription> attachments;
			attachments.resize(pDesc->GraphicsPipeline.RenderTargetCount);

			std::vector<VkAttachmentReference> attachmentsRefs;
			attachmentsRefs.resize(pDesc->GraphicsPipeline.RenderTargetCount);

			for (int32 i = 0; i < pDesc->GraphicsPipeline.RenderTargetCount; i++)
			{
				VkAttachmentDescription& desc = attachments[i];
				desc.format = ReToVkFormat(pDesc->GraphicsPipeline.RenderTargetFormats[i]);

				desc.samples = ReToVkSampleCount(pDesc->GraphicsPipeline.SampleCount);

				//TODO: Fix operation based on if it is a color or depth/stencil attachment
				desc.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				desc.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				desc.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				desc.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				desc.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				desc.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;


				VkAttachmentReference& ref = attachmentsRefs[i];
				ref.attachment = i;
				ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}



			if (pDesc->GraphicsPipeline.DepthStencilFormat != FORMAT_UNKNOWN)
			{
				VkAttachmentDescription attachment = {};
				attachment.format = ReToVkFormat(pDesc->GraphicsPipeline.DepthStencilFormat);
				attachment.samples = ReToVkSampleCount(pDesc->GraphicsPipeline.SampleCount);

				//TODO: Fix operation based on if it is a color or depth/stencil attachment
				attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
				attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
				attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
				attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
				attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;


				VkAttachmentReference ref = {};
				ref.attachment = static_cast<uint32_t>(attachments.size());
				ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
				
				attachments.push_back(attachment);
				attachmentsRefs.push_back(ref);
			}



			VkSubpassDescription subpass = {};
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.flags = 0;

			subpass.colorAttachmentCount = static_cast<uint32_t>(attachmentsRefs.size());
			subpass.pColorAttachments = attachmentsRefs.data();


			VkRenderPassCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;

			desc.attachmentCount = static_cast<uint32_t>(attachments.size());
			desc.pAttachments = attachments.data();
			desc.subpassCount = 1;
			desc.pSubpasses = &subpass;

			desc.dependencyCount = 0;
			desc.pDependencies = nullptr;


			VkDevice vkDevice = m_Device->GetVkDevice();
			VkResult result = vkCreateRenderPass(vkDevice, &desc, nullptr, &m_RenderPass);
			if (result != VK_SUCCESS) 
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "Vulkan: Could not create renderpass");
				return false;
			}

			return true;
		}
		


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkPipelineShaderStageCreateInfo VulkPipelineState::CreateVkPipelineShaderStageCreateInfo(const IShader* pShader)
		{
			const VulkShader* pVulkShader = reinterpret_cast<const VulkShader*>(pShader);
			VkShaderModule module = pVulkShader->GetVkShaderModule();

			VkPipelineShaderStageCreateInfo desc = {};
			desc.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;

			desc.pSpecializationInfo = nullptr;
			desc.module = module;
			desc.pName = pVulkShader->GetEntryPoint().c_str();
			
			SHADER_TYPE type = pVulkShader->GetType();
			if (type == SHADER_TYPE_VERTEX)
				desc.stage = VK_SHADER_STAGE_VERTEX_BIT;
			else if (type == SHADER_TYPE_HULL)
				desc.stage = VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			else if (type == SHADER_TYPE_DOMAIN)
				desc.stage = VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			else if (type == SHADER_TYPE_GEOMETRY)
				desc.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
			else if (type == SHADER_TYPE_PIXEL)
				desc.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			else if (type == SHADER_TYPE_COMPUTE)
				desc.stage = VK_SHADER_STAGE_COMPUTE_BIT;

			return desc;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		VkInputElement VulkPipelineState::CreateVkInputElement(const InputElementDesc* pDesc, int32 location)
		{
			VkVertexInputBindingDescription binding = {};
			binding.stride = pDesc->StrideBytes;
			binding.binding = pDesc->InputSlot;

			if (pDesc->StepType == ELEMENT_STEP_TYPE_VERTEX)
				binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			else if (pDesc->StepType == ELEMENT_STEP_TYPE_INSTANCE)
				binding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;


			VkVertexInputAttributeDescription attribute = {};
			attribute.binding = binding.binding;
			attribute.format = ReToVkFormat(pDesc->Format);
			attribute.location = location;
			attribute.offset = pDesc->ElementOffset;


			VkInputElement element;
			element.first = attribute;
			element.second = binding;

			return element;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo* pInfo, const PipelineStateDesc* pDesc)
		{
			pInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			pInfo->pNext = nullptr;
			pInfo->flags = 0;
			pInfo->topology = ReToVkPrimitiveTopology(pDesc->GraphicsPipeline.Topology);
			pInfo->primitiveRestartEnable = VK_FALSE;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void VulkPipelineState::SetColorBlendAttachmentState(VkPipelineColorBlendAttachmentState* pInfo, const RenderTargetBlendDesc* pDesc)
		{
			pInfo->blendEnable = pDesc->BlendEnable ? VK_TRUE : VK_FALSE;

			if (pDesc->WriteMask & COLOR_WRITE_ENABLE_RED)
				pInfo->colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
			if (pDesc->WriteMask & COLOR_WRITE_ENABLE_GREEN)
				pInfo->colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
			if (pDesc->WriteMask & COLOR_WRITE_ENABLE_BLUE)
				pInfo->colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
			if (pDesc->WriteMask & COLOR_WRITE_ENABLE_ALPHA)
				pInfo->colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;

			pInfo->srcColorBlendFactor = ReToVkBlendFactor(pDesc->SrcBlend);
			pInfo->dstColorBlendFactor = ReToVkBlendFactor(pDesc->DstBlend); 
			pInfo->colorBlendOp = ReToVkBlendOp(pDesc->BlendOperation);
			pInfo->srcAlphaBlendFactor = ReToVkBlendFactor(pDesc->SrcAlphaBlend);
			pInfo->dstAlphaBlendFactor = ReToVkBlendFactor(pDesc->DstAlphaBlend);
			pInfo->alphaBlendOp = ReToVkBlendOp(pDesc->AlphaBlendOperation);
		}
	}
}