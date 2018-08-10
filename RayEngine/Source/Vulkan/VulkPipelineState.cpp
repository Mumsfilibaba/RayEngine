#include <vector>
#include <utility>
#include "..\..\Include\Vulkan\VulkPipelineState.h"
#include "..\..\Include\Vulkan\VulkDevice.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		VulkPipelineState::VulkPipelineState(IDevice* pDevice, const PipelineStateInfo& info)
			: m_Device(nullptr),
			m_RootSignature(nullptr),
			m_RenderPass(nullptr),
			m_Pipeline(nullptr),
			m_Type(PIPELINE_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = QueryVulkDevice(pDevice);

			Create(info);
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
			ReRelease_S(m_RootSignature);
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
		void VulkPipelineState::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryVulkDevice(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::Create(const PipelineStateInfo& info)
		{
			if (info.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsPipeline(info);
			else if (info.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputePipeline(info);
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::SetRasterizerState(VkPipelineRasterizationStateCreateInfo& desc, const RasterizerStateInfo& info)
		{
			desc.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			desc.flags = 0;
			desc.pNext = nullptr;
			//TODO: Look up depthclamp
			desc.depthClampEnable = info.DepthClipEnable ? VK_TRUE : VK_FALSE;
			desc.rasterizerDiscardEnable = VK_FALSE;
			
			if (info.FillMode == FILL_MODE_SOLID)
				desc.polygonMode = VK_POLYGON_MODE_FILL;
			else if (info.FillMode == FILL_MODE_WIREFRAME)
				desc.polygonMode = VK_POLYGON_MODE_LINE;

			if (info.CullMode == CULL_MODE_BACK)
				desc.cullMode = VK_CULL_MODE_BACK_BIT;
			else if (info.CullMode == CULL_MODE_FRONT)
				desc.cullMode = VK_CULL_MODE_FRONT_BIT;
			else if (info.CullMode == CULL_MODE_NONE)
				desc.cullMode = VK_CULL_MODE_NONE;

			//TODO: Wider?
			desc.lineWidth = 1.0f;

			desc.frontFace = info.FrontCounterClockwise ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;
			
			desc.depthBiasEnable = (info.DepthBias > 0);
			desc.depthBiasClamp = info.DepthBiasClamp;
			desc.depthBiasSlopeFactor = info.SlopeScaleDepthBias;
			desc.depthBiasConstantFactor = static_cast<float>(info.DepthBias);
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::SetDepthStencilState(VkPipelineDepthStencilStateCreateInfo& desc, const DepthStencilStateInfo& info)
		{
			desc.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;

			desc.depthTestEnable = info.DepthEnable ? VK_TRUE : VK_FALSE;
			desc.depthWriteEnable = desc.depthTestEnable;

			desc.depthCompareOp = ReToVkCompareOp(info.DepthFunc);
			desc.depthBoundsTestEnable = VK_FALSE; //Maybe?

			desc.stencilTestEnable = info.StencilEnable ? VK_TRUE : VK_FALSE;

			desc.front = ReToVkStencilOpState(info.Frontface);
			desc.back = ReToVkStencilOpState(info.BackFace);
			
			desc.minDepthBounds = 0.0f;
			desc.maxDepthBounds = 0.0f;
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::CreateComputePipeline(const PipelineStateInfo& info)
		{
			//TODO: Implement ComputeShaders
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::CreateGraphicsPipeline(const PipelineStateInfo& info)
		{
			using namespace System;

			std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
			if (info.GraphicsPipeline.pVertexShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(info.GraphicsPipeline.pVertexShader));

			if (info.GraphicsPipeline.pHullShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(info.GraphicsPipeline.pHullShader));

			if (info.GraphicsPipeline.pDomainShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(info.GraphicsPipeline.pDomainShader));

			if (info.GraphicsPipeline.pGeometryShader != nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(info.GraphicsPipeline.pGeometryShader));

			if (info.GraphicsPipeline.pPixelShader!= nullptr)
				shaderStages.push_back(CreateVkPipelineShaderStageCreateInfo(info.GraphicsPipeline.pPixelShader));



			int32 elementCount = info.GraphicsPipeline.InputLayout.ElementCount;
			std::vector<VkVertexInputBindingDescription> vertexBindings;
			vertexBindings.resize(elementCount);
			
			std::vector<VkVertexInputAttributeDescription> vertexAttributes;
			vertexAttributes.resize(elementCount);

			for (int32 i = 0; i < elementCount; i++)
			{
				VkInputElement element = CreateVkInputElement(info.GraphicsPipeline.InputLayout.pElements[i], i);

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
			CreateInputAssemblyStateInfo(inputAssemblyState, info);



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
			colorBlendAttachments.resize(info.GraphicsPipeline.RenderTargetCount);

			for (int32 i = 0; i < info.GraphicsPipeline.RenderTargetCount; i++)
				SetColorBlendAttachmentState(colorBlendAttachments[i], info.GraphicsPipeline.BlendState.RenderTargets[i]);

			VkPipelineColorBlendStateCreateInfo blendState = {};
			blendState.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			blendState.pNext = nullptr;
			blendState.flags = 0;

			blendState.logicOpEnable = info.GraphicsPipeline.BlendState.LogicOpEnable ? VK_TRUE : VK_FALSE;
			blendState.logicOp = VK_LOGIC_OP_NO_OP;
			blendState.attachmentCount = info.GraphicsPipeline.RenderTargetCount;
			blendState.pAttachments = colorBlendAttachments.data();

			blendState.blendConstants[0] = info.GraphicsPipeline.BlendState.BlendFactor[0];
			blendState.blendConstants[1] = info.GraphicsPipeline.BlendState.BlendFactor[1];
			blendState.blendConstants[2] = info.GraphicsPipeline.BlendState.BlendFactor[2];
			blendState.blendConstants[3] = info.GraphicsPipeline.BlendState.BlendFactor[3];



			VkPipelineRasterizationStateCreateInfo rasterizerState = {};
			SetRasterizerState(rasterizerState, info.GraphicsPipeline.RasterizerState);



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
			//multisamplingState.rasterizationSamples = ReToVkSampleCount(info.GraphicsPipeline.SampleCount);


			
			VkPipelineDepthStencilStateCreateInfo depthStencilState = {};
			SetDepthStencilState(depthStencilState, info.GraphicsPipeline.DepthStencilState);


			if (!CreateRenderPass(info))
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

			m_RootSignature = reinterpret_cast<const VulkRootSignature*>(info.pRootSignature->QueryReference());
			VkPipelineLayout vkLayout = m_RootSignature->GetVkPipelineLayout();
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



		/////////////////////////////////////////////////////////////
		bool VulkPipelineState::CreateRenderPass(const PipelineStateInfo& info)
		{
			using namespace System;

			std::vector<VkAttachmentDescription> attachments;
			attachments.resize(info.GraphicsPipeline.RenderTargetCount);

			std::vector<VkAttachmentReference> attachmentsRefs;
			attachmentsRefs.resize(info.GraphicsPipeline.RenderTargetCount);

			for (int32 i = 0; i < info.GraphicsPipeline.RenderTargetCount; i++)
			{
				VkAttachmentDescription& desc = attachments[i];
				desc.format = ReToVkFormat(info.GraphicsPipeline.RenderTargetFormats[i]);

				desc.samples = ReToVkSampleCount(info.GraphicsPipeline.SampleCount);

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



			if (info.GraphicsPipeline.DepthStencilFormat != FORMAT_UNKNOWN)
			{
				VkAttachmentDescription attachment = {};
				attachment.format = ReToVkFormat(info.GraphicsPipeline.DepthStencilFormat);
				attachment.samples = ReToVkSampleCount(info.GraphicsPipeline.SampleCount);

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
		


		/////////////////////////////////////////////////////////////
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



		/////////////////////////////////////////////////////////////
		VkInputElement VulkPipelineState::CreateVkInputElement(const InputElementInfo& info, int32 location)
		{
			VkVertexInputBindingDescription binding = {};
			binding.stride = info.StrideBytes;
			binding.binding = info.InputSlot;

			if (info.StepType == ELEMENT_STEP_TYPE_VERTEX)
				binding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
			else if (info.StepType == ELEMENT_STEP_TYPE_INSTANCE)
				binding.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;


			VkVertexInputAttributeDescription attribute = {};
			attribute.binding = binding.binding;
			attribute.format = ReToVkFormat(info.Format);
			attribute.location = location;
			attribute.offset = info.ElementOffset;


			VkInputElement element;
			element.first = attribute;
			element.second = binding;

			return element;
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::CreateInputAssemblyStateInfo(VkPipelineInputAssemblyStateCreateInfo& desc, const PipelineStateInfo& info)
		{
			desc.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			desc.pNext = nullptr;
			desc.flags = 0;
			desc.topology = ReToVkPrimitiveTopology(info.GraphicsPipeline.Topology);
			desc.primitiveRestartEnable = info.GraphicsPipeline.StripCutEnable ? VK_TRUE : VK_FALSE;
		}



		/////////////////////////////////////////////////////////////
		void VulkPipelineState::SetColorBlendAttachmentState(VkPipelineColorBlendAttachmentState& desc, const RenderTargetBlendInfo& info)
		{
			desc.blendEnable = info.BlendEnable ? VK_TRUE : VK_FALSE;

			if (info.RenderTargetWriteMask & COLOR_WRITE_ENABLE_RED)
				desc.colorWriteMask |= VK_COLOR_COMPONENT_R_BIT;
			if (info.RenderTargetWriteMask & COLOR_WRITE_ENABLE_GREEN)
				desc.colorWriteMask |= VK_COLOR_COMPONENT_G_BIT;
			if (info.RenderTargetWriteMask & COLOR_WRITE_ENABLE_BLUE)
				desc.colorWriteMask |= VK_COLOR_COMPONENT_B_BIT;
			if (info.RenderTargetWriteMask & COLOR_WRITE_ENABLE_ALPHA)
				desc.colorWriteMask |= VK_COLOR_COMPONENT_A_BIT;

			desc.srcColorBlendFactor = ReToVkBlendFactor(info.SrcBlend);
			desc.dstColorBlendFactor = ReToVkBlendFactor(info.DstBlend); 
			desc.colorBlendOp = ReToVkBlendOp(info.BlendOperation);
			desc.srcAlphaBlendFactor = ReToVkBlendFactor(info.SrcAlphaBlend);
			desc.dstAlphaBlendFactor = ReToVkBlendFactor(info.DstAlphaBlend);
			desc.alphaBlendOp = ReToVkBlendOp(info.AlphaBlendOperation);
		}
	}
}