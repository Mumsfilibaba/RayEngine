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
			
			A structure containg information about a inputattribute.
			Describes information in the vertexbuffers.

			Semantic - Semantic to bind the attribute to. If GLSL
			is used as sourceshaderlanguage this parameter can be
			discarded. However it is needed if any directx api will
			be used.

			SemanticIndex - Index of the semantic. Allows multiple
			attributes to have the same semantic but with different
			indices.

			Format - Format of the attribute.

			StepType - Should the attribute be updated for each 
			vertex or instance.

			InputSlot - An index telling what vertexbuffer to read 
			from.

			DataStepRate - How many steeps in the vertexbuffer to
			take between updates.

			ElementOffset - The number of bytes between elements
			in the datastructure. E.g 
			
			struct Vertex
			{
				float x;
				float y;
			};
			
			should be sizeof(float) if describing the variable y.
			
			StrideBytes - The number of bytes of this element.

		////////////////////////////////////////////////////////////*/
		struct InputElementInfo
		{
			std::string Semantic = "";
			int32 SemanticIndex = 0;
			FORMAT Format = FORMAT_UNKNOWN;
			ELEMENT_STEP_TYPE StepType = ELEMENT_STEP_TYPE_UNKNOWN;
			int32 InputSlot = 0;
			int32 DataStepRate = 0;
			int32 ElementOffset = 0;
			int32 StrideBytes = 0;
		};



		/*////////////////////////////////////////////////////////////

			A structure containing an array of InputElementInfo.
			Describes all the input attributes that will be used in
			a pipelinestate.

			pElements - An array of InputElementInfo-structures.

			ElementCount - Number of elements in pElements.

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
			
			A structure describing the RasterizerState of the 
			pipeline.

			ConservativeRasterizerEnable - Turn the conservative
			rasterizer on/off.

			ForcedSampleCount - Force multisampling with this 
			sample. This is when rendering to a UnorderedAccessView
			instead of rendering to a RenderTargetView and/or 
			DepthStencilView.

			FillMode - How to fill triangleprimitives.

			CullMode - How to cull primitives.

			FrontCounterClockwise - Triangleprimitives are 
			frontfaceing if vertexorder is clockwise. Set to true
			to make them facing forward if they are counterclockwise.

			DepthClipEnable - Clip pixels that has a z-coordinate
			beyond the depth-clippingplane.

			DepthBias - A value added to the depth of each pixel.

			DepthBiasClamp - A value clamping the DepthBias.

			SlopeScaleDepthBias - A scalar on a given pixels slope.

			ScissorEnable - Enables scissor rectangles

			MultisampleEnable - Enables multisampling when rendering
			to a RenderTargetView and/or DepthStencilView.

			AntialiasedLineEnable - Enables antialiasing when 
			rendering lineprimitives or wireframefillmodes. If set
			to true then MultisampleEnable should be set to false.

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

			A structure defining the DepthStencilState of the 
			pipeline.
			
			DepthEnable - Enable depthtesting.

			DepthWriteMask - Describes if writes to the 
			depthstencilbuffer shold be turned on or not.

			DepthFunc - Sets the function for when the depthtest 
			should yelid a positive result.

			StencilEnable - Enables stenciltesting.

			StencilReadMask - Identifies a portion of the 
			stencilbuffer available for readoperations.

			StencilWrite - Identifies a portion of the stencilbuffer
			available for writeoperations.

			Frontface - Describes stencil operations on frontfacing
			triangles.

			Backface - Describes stencil operations on backfacing
			triangles.

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

			A structure describing how a RenderTarget should blend.

			BlendEnable - Set to true to enable blending.

			SrcBlend - Part of the source to use in the 
			blendoperation. RGB values.

			DstBlend -  Part of the destination to use in the 
			blendoperation. RGB values.

			BlendOperation - What type of blendoperation to 
			perform on the RGB values.
			
			SrcAlphaBlend - Part of the source to use in the
			blendoperation. The alpha value.

			DstAlphaBlend -  Part of the destination to use in the
			blendoperation. The alpha value.

			AlphaBlendOperation - What type of blendoperation to
			perform on the alphavalues.

			RenderTargetWriteMask - Mask that enables different
			components to take part in the blendoperation.

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

			A structure describing the blendstate of the pipeline.

			AlphaToCoverageEnable - Enable AlphaToCoverage. Takes 
			the alpha component of the output rendertarget at slot
			0 and uses it as a n-step coverage mask on an n-sampled
			rendertarget. A AND operation determines what samples
			to update in the rendertarget.

			IndependentBlendEnable - If set to false only the 
			RenderTargets[0] member is used in the blendstate.
			If true the rendertargets at slot [1 - 7] will have
			independent blendsettings.

			LogicOpEnable - Enables logic oeprations

			BlendFactor - Four-component floatarray to use when the 
			blendoperation is using the blendfactor in either the 
			src or dest.

			RenderTargets - A structure describing each rendertarget
			that can be bound to the pipeline at one time.

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

			Describes a pipelinestate

			Name - Name of the state

			Type - Type of pipelinestate

			pRootLayout - A valid pointer to a RootLayout to be used
			with the pipelinestate. 

			GraphicsPipeline - Descibes a graphicspipeline;

			ComputePipeline - Descibes a computepipeline;

		////////////////////////////////////////////////////////////*/
		struct PipelineStateInfo
		{
		public:
			/*////////////////////////////////////////////////////////////

				A structure describing a compute pipeline.

				pComputeShader - The computeshader to use in the 
				pipeline.

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

				A structure describing the graphics pipeline.

				RenderTargetCount - Number of rendertargets to use.

				RenderTargetFormats - The format of each rendertarget
				that will be used.

				DepthStencilFormat - The format of the depthstencil.

				SampleCount - The samples of the rendertargets.

				InputLayout - Describes the input to the vertexshader.

				RasterizerState - Describes how the outputimage will be 
				rasterized.

				DepthStencilState - Descibes how depth- and 
				stenciltesting will be performed.

				BlendState - Describes how blending will be performed.

				SampleMask - Samplemask for the blendstate.

				Topology - The topology to use for the tesselation and
				geometrystages.

				StripCutEnable - Enable the value -1 to restart the 
				strip. Requires an indexbuffer.

				pVertexShader - Shader for the vertexstage

				pHullShader - Shader for the tesselationstage

				pDomainShader - Shader for the tesselationstage

				pGeometryShader - Shader for the geometrystage

				pPixelShader - Shader for the pixelstage

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
			RE_INTERFACE(IPipelineState);

		public:
			IPipelineState() {}
			~IPipelineState() {}

			/*////////////////////////////////////////////////////////////
				Return the type of the pipeline.
			////////////////////////////////////////////////////////////*/
			virtual PIPELINE_TYPE GetPipelineType() const = 0;
		};
	}
}