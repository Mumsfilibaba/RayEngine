#pragma once

#include <string>
#include "..\Defines.h"
#include "..\Types.h"
#include "IShader.h"
#include "IRootSignature.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		enum PIPELINE_TYPE : int32
		{
			PIPELINE_TYPE_UNKNOWN = 0,
			PIPELINE_TYPE_GRAPHICS = 1,
			PIPELINE_TYPE_COMPUTE = 2,
		};



		/////////////////////////////////////////////////////////////
		enum ELEMENT_STEP_TYPE : int32
		{
			ELEMENT_STEP_TYPE_UNKNOWN = 0,
			ELEMENT_STEP_TYPE_VERTEX = 1,
			ELEMENT_STEP_TYPE_INSTANCE = 2,
		};



		/////////////////////////////////////////////////////////////
		struct InputElementInfo
		{
			std::string Semantic;
			int32 SemanticIndex = 0;
			FORMAT Format = FORMAT_UNKNOWN;
			ELEMENT_STEP_TYPE StepType = ELEMENT_STEP_TYPE_UNKNOWN;
			int32 DataStepRate = 0;
			int32 InputSlot = 0;
			int32 ElementOffset = 0;
		};



		/////////////////////////////////////////////////////////////
		struct InputLayoutInfo
		{
			InputElementInfo* pElements = nullptr;
			int32 ElementCount = 0;
		};



		/////////////////////////////////////////////////////////////
		struct RasterizerStateInfo
		{
			bool ConservativeRasterizerEnable = false;
			int32 ForcedSampleCount = 0;

			FILL_MODE FillMode = FILL_MODE_SOLID;
			CULL_MODE CullMode = CULL_MODE_BACK;
			bool FrontCounterClockwise = false;
			
			bool DepthClipEnable = true;
			int32 DepthBias = 0;
			float DepthBiasClamp = 0.0f;
			float SlopeScaleDepthBias = 0.0f;

			bool ScissorEnable = false;
			bool MultisampleEnable = false;
			bool AntialiasedLineEnable = false;
		};



		/////////////////////////////////////////////////////////////
		struct DepthStencilStateInfo
		{
			bool DepthEnable = true;
			DEPTH_WRITE_MASK DepthWriteMask = DEPTH_WRITE_MASK_ALL;
			COMPARISON_FUNC DepthFunc = COMPARISON_FUNC_LESS;
			
			bool StencilEnable = false;
			uint8 StencilReadMask = -1;
			uint8 StencilWriteMask = -1;

			StencilOperationInfo Frontface;
			StencilOperationInfo BackFace;
		};



		/////////////////////////////////////////////////////////////
		struct BlendStateInfo
		{
			bool AlphaToCoverageEnable = false;
			bool IndependentBlendEnable = false;
			const float BlendFactor[4];
			uint32 SampleMask = -1;
			
			struct
			{
				bool BlendEnable = false;
				BLEND_TYPE SrcBlend = BLEND_TYPE_ONE;
				BLEND_TYPE DstBlend = BLEND_TYPE_ZERO;
				BLEND_OPERATION BlendOperation = BLEND_OPERATION_ADD;
				BLEND_TYPE SrcAlphaBlend = BLEND_TYPE_ONE;
				BLEND_TYPE DstAlphaBlend = BLEND_TYPE_ZERO;
				BLEND_OPERATION AlphaBlendOperation = BLEND_OPERATION_ADD;
				uint8 RenderTargetWriteMask = COLOR_WRITE_ENABLE_ALL;

			} RenderTargets[8];
		};



		/////////////////////////////////////////////////////////////
		struct PipelineStateInfo
		{
			std::string Name = "";
			PIPELINE_TYPE Type = PIPELINE_TYPE_UNKNOWN;
			IRootSignature* pRootSignature = nullptr;

			//TODO: Maybe use a union

			struct
			{
				IShader* pComputeShader = nullptr;
			} ComputePipeline;
				
			struct
			{
				InputLayoutInfo InputLayout;
				RasterizerStateInfo RasterizerState;
				DepthStencilStateInfo DepthStencilState;
				BlendStateInfo BlendState;
				IShader* pVertexShader = nullptr;
				IShader* pHullShader = nullptr;
				IShader* pDomainShader = nullptr;
				IShader* pGeometryShader = nullptr;
				IShader* pPixelShader = nullptr;
			} GraphicsPipeline;
		};



		/////////////////////////////////////////////////////////////
		class IPipelineState : public IReferenceCounter
		{
		public:
			IPipelineState(IPipelineState&& other) = delete;
			IPipelineState(const IPipelineState& other) = delete;
			IPipelineState& operator=(IPipelineState&& other) = delete;
			IPipelineState& operator=(const IPipelineState& other) = delete;

		public:
			IPipelineState() {}
			virtual ~IPipelineState() {}

			//Get type
			virtual PIPELINE_TYPE GetPipelineType() const = 0;
			//Get the device that created the pipelinestate
			virtual IDevice* GetDevice() const = 0;
		};
	}
}