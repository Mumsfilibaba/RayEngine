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
#include <string>
#include "IDeviceObject.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class IShader;
		class IRootLayout;



		/////////////////////////////////////////////////////////////
		enum ELEMENT_STEP_TYPE : int32
		{
			ELEMENT_STEP_TYPE_UNKNOWN = 0,
			ELEMENT_STEP_TYPE_VERTEX = 1,
			ELEMENT_STEP_TYPE_INSTANCE = 2,
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct InputElementInfo
		{
			std::string Semantic;
			int32 SemanticIndex = 0;
			FORMAT Format = FORMAT_UNKNOWN;
			ELEMENT_STEP_TYPE StepType = ELEMENT_STEP_TYPE_UNKNOWN;
			int32 DataStepRate = 0;
			int32 InputSlot = 0;
			int32 ElementOffset = 0;
			int32 StrideBytes = 0;
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct InputLayoutInfo
		{
		public:
			InputLayoutInfo() {};
			~InputLayoutInfo() {};

		public:
			InputElementInfo* pElements = nullptr;
			int32 ElementCount = 0;
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
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



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
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



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct RenderTargetBlendInfo
		{
			bool BlendEnable = false;
			BLEND_TYPE SrcBlend = BLEND_TYPE_ONE;
			BLEND_TYPE DstBlend = BLEND_TYPE_ZERO;
			BLEND_OPERATION BlendOperation = BLEND_OPERATION_ADD;
			BLEND_TYPE SrcAlphaBlend = BLEND_TYPE_ONE;
			BLEND_TYPE DstAlphaBlend = BLEND_TYPE_ZERO;
			BLEND_OPERATION AlphaBlendOperation = BLEND_OPERATION_ADD;
			uint8 RenderTargetWriteMask = COLOR_WRITE_ENABLE_ALL;
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct BlendStateInfo
		{
			bool AlphaToCoverageEnable = false;
			bool IndependentBlendEnable = false;
			bool LogicOpEnable = false;
			float BlendFactor[4];
			RenderTargetBlendInfo RenderTargets[8];
		};



		/*////////////////////////////////////////////////////////////

		////////////////////////////////////////////////////////////*/
		struct PipelineStateInfo
		{
		public:
			/*////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////*/
			struct ComputePipelineInfo
			{
			public:
				ComputePipelineInfo() {};
				~ComputePipelineInfo() {};

			public:
				IShader* pComputeShader = nullptr;
			};



			/*////////////////////////////////////////////////////////////

			////////////////////////////////////////////////////////////*/
			struct GraphicsPipelineInfo
			{
			public:
				GraphicsPipelineInfo() {};
				~GraphicsPipelineInfo() {};

			public:
				int32 RenderTargetCount = 0;
				FORMAT RenderTargetFormats[8];
				FORMAT DepthStencilFormat = FORMAT_UNKNOWN;
				MSAA_SAMPLE_COUNT SampleCount = MSAA_SAMPLE_COUNT_1;

				InputLayoutInfo InputLayout;
				RasterizerStateInfo RasterizerState;
				DepthStencilStateInfo DepthStencilState;
				BlendStateInfo BlendState;

				uint32 SampleMask = -1;

				PRIMITIVE_TOPOLOGY Topology = PRIMITIVE_TOPOLOGY_UNKNOWN;
				bool StripCutEnable = false;

				IShader* pVertexShader = nullptr;
				IShader* pHullShader = nullptr;
				IShader* pDomainShader = nullptr;
				IShader* pGeometryShader = nullptr;
				IShader* pPixelShader = nullptr;
			};
		public:
			PipelineStateInfo() {}
			~PipelineStateInfo() {}

		public:
			std::string Name = "";
			PIPELINE_TYPE Type = PIPELINE_TYPE_UNKNOWN;
			IRootLayout* pRootLayout = nullptr;
			GraphicsPipelineInfo GraphicsPipeline;
			ComputePipelineInfo ComputePipeline;
		};



		/////////////////////////////////////////////////////////////
		class IPipelineState : public IDeviceObject
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
		};
	}
}