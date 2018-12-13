/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the 
License. You may obtain a copy of the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can be traced back to the use of this software, shall no contributor be 
held liable. This includes computer failure and or malfunction of any kind.

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/

#pragma once
#include <RayEngine.h>
#include <Interfaces/IObject.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IShader;
		class IRootLayout;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		enum ELEMENT_STEP_TYPE : int32
		{
			ELEMENT_STEP_TYPE_UNKNOWN = 0,
			ELEMENT_STEP_TYPE_VERTEX = 1,
			ELEMENT_STEP_TYPE_INSTANCE = 2,
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
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

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API InputElementDesc
		{
			const char* pSemantic;
			int32 SemanticIndex;
			FORMAT Format;
			ELEMENT_STEP_TYPE StepType;
			int32 InputSlot;
			int32 DataStepRate;
			int32 ElementOffset;
			int32 StrideBytes;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			A structure containing an array of InputElementDesc.
			Describes all the input attributes that will be used in
			a pipelinestate.

			pElements - An array of InputElementDesc-structures.

			ElementCount - Number of elements in pElements.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API InputLayoutDesc
		{
			InputElementDesc* pElements;
			int32 ElementCount;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			
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

			DEFUALT STATE:
			ConservativeRasterizerEnable = false;
			ForcedSampleCount = 0;
			FillMode = FILL_MODE_SOLID;
			CullMode = CULL_MODE_BACK;
			FrontCounterClockwise = false;
			DepthClipEnable = true;
			DepthBias = 0;
			DepthBiasClamp = 0.0f;
			SlopeScaleDepthBias = 0.0f;
			ScissorEnable = false;
			MultisampleEnable = false;
			AntialiasedLineEnable = false;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API RasterizerStateDesc
		{
			bool ConservativeRasterizerEnable;
			int32 ForcedSampleCount;

			FILL_MODE FillMode;
			CULL_MODE CullMode;
			bool FrontCounterClockwise;
			
			bool DepthClipEnable;
			int32 DepthBias;
			float DepthBiasClamp;
			float SlopeScaleDepthBias;

			bool ScissorEnable;
			bool MultisampleEnable;
			bool AntialiasedLineEnable;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

			FrontFace - Describes stencil operations on frontfacing
			triangles.

			Backface - Describes stencil operations on backfacing
			triangles.

			DEFAULT STATE:
			DepthEnable = true;
			DepthWriteMask = DEPTH_WRITE_MASK_ALL;
			DepthFunc = COMPARISON_FUNC_LESS;

			StencilEnable = false;
			StencilReadMask = -1;
			StencilWriteMask = -1;
			
			FrontFace.StencilFailOperation = STENCIL_OPERATION_KEEP
			FrontFace.StencilDepthFailOperation = STENCIL_OPERATION_KEEP
			FrontFace.StencilPassOperation = STENCIL_OPERATION_KEEP
			FrontFace.StencilFunc = COMPARISON_FUNC_ALWAYS

			BackFace.StencilFailOperation = STENCIL_OPERATION_KEEP
			BackFace.StencilDepthFailOperation = STENCIL_OPERATION_KEEP
			BackFace.StencilPassOperation = STENCIL_OPERATION_KEEP
			BackFace.StencilFunc = COMPARISON_FUNC_ALWAYS

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API DepthStencilStateDesc
		{
			bool DepthEnable;
			DEPTH_WRITE_MASK DepthWriteMask;
			COMPARISON_FUNC DepthFunc;
			
			bool StencilEnable;
			uint8 StencilReadMask;
			uint8 StencilWriteMask;

			StencilOperationInfo FrontFace;
			StencilOperationInfo BackFace;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

			DEFAULT STATE:
			BlendEnable = false
			SrcBlend = BLEND_TYPE_ONE
			DstBlend = BLEND_TYPE_ZERO
			BlendOperation = BLEND_OPERATION_ADD
			SrcAlphaBlend = BLEND_TYPE_ONE
			DstAlphaBlend = BLEND_TYPE_ZERO
			AlphaBlendOperation = BLEND_OPERATION_ADD
			RenderTargetWriteMask = COLOR_WRITE_ENABLE_ALL

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API RenderTargetBlendDesc
		{
			bool BlendEnable;
			BLEND_TYPE SrcBlend;
			BLEND_TYPE DstBlend;
			BLEND_OPERATION BlendOperation;
			BLEND_TYPE SrcAlphaBlend;
			BLEND_TYPE DstAlphaBlend;
			BLEND_OPERATION AlphaBlendOperation;
			uint8 WriteMask;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

			DEFAULT STATE:
			AlphaToCoverageEnable = false;
			IndependentBlendEnable = false;
			LogicOpEnable = false;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API BlendStateDesc
		{
			bool AlphaToCoverageEnable;
			bool IndependentBlendEnable;
			bool LogicOpEnable;
			float BlendFactor[4];
			RenderTargetBlendDesc RenderTargets[RE_MAX_RENDERTARGETS];
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
			A structure describing a compute pipeline.

			pComputeShader - The computeshader to use in the pipeline.

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API ComputePipelineDesc
		{
			IShader* pComputeShader;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

			pVertexShader - Shader for the vertexstage

			pHullShader - Shader for the tesselationstage

			pDomainShader - Shader for the tesselationstage

			pGeometryShader - Shader for the geometrystage

			pPixelShader - Shader for the pixelstage

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API GraphicsPipelineDesc
		{
			IShader* pVertexShader;
			IShader* pHullShader;
			IShader* pDomainShader;
			IShader* pGeometryShader;
			IShader* pPixelShader;

			FORMAT DepthStencilFormat;
			FORMAT RenderTargetFormats[8];
			int32 RenderTargetCount;
			int32 SampleCount;
			uint32 SampleMask;

			InputLayoutDesc InputLayout;
			RasterizerStateDesc RasterizerState;
			DepthStencilStateDesc DepthStencilState;
			BlendStateDesc BlendState;

			PRIMITIVE_TOPOLOGY Topology;
		};


		/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			Describes a pipelinestate

			Name - Name of the state

			Type - Type of pipelinestate

			pRootLayout - A valid pointer to a RootLayout to be used
			with the pipelinestate. 

			Graphics - Descibes a graphicspipeline;

			Compute - Descibes a computepipeline;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////*/
		struct RAYENGINE_API PipelineStateDesc
		{
			PIPELINE_TYPE Type;
			IRootLayout* pRootLayout;

			union
			{
				GraphicsPipelineDesc Graphics;
				ComputePipelineDesc Compute;
			};

			inline static PipelineStateDesc DefaultGraphicsPipeline()
			{
				PipelineStateDesc desc = {};
				desc.Type = PIPELINE_TYPE_GRAPHICS;
				desc.pRootLayout = nullptr;
				
				desc.Graphics.BlendState.AlphaToCoverageEnable = false;
				desc.Graphics.BlendState.IndependentBlendEnable = false;
				desc.Graphics.BlendState.LogicOpEnable = false;
				
				desc.Graphics.BlendState.BlendFactor[0] = 0.0f;
				desc.Graphics.BlendState.BlendFactor[1] = 0.0f;
				desc.Graphics.BlendState.BlendFactor[2] = 0.0f;
				desc.Graphics.BlendState.BlendFactor[3] = 0.0f;

				desc.Graphics.BlendState.RenderTargets[0].BlendEnable = false;
				desc.Graphics.BlendState.RenderTargets[0].SrcBlend = BLEND_TYPE_ONE;
				desc.Graphics.BlendState.RenderTargets[0].DstBlend = BLEND_TYPE_ZERO;
				desc.Graphics.BlendState.RenderTargets[0].BlendOperation = BLEND_OPERATION_ADD;
				desc.Graphics.BlendState.RenderTargets[0].SrcAlphaBlend = BLEND_TYPE_ONE;
				desc.Graphics.BlendState.RenderTargets[0].DstAlphaBlend = BLEND_TYPE_ZERO;
				desc.Graphics.BlendState.RenderTargets[0].AlphaBlendOperation = BLEND_OPERATION_ADD;
				desc.Graphics.BlendState.RenderTargets[0].WriteMask = COLOR_WRITE_ENABLE_ALL;
				for (int32 i = 1; i < RE_MAX_RENDERTARGETS; i++)
					desc.Graphics.BlendState.RenderTargets[i] = desc.Graphics.BlendState.RenderTargets[0];

				desc.Graphics.SampleCount = 0;
				desc.Graphics.RenderTargetCount = 0;
				for (int32 i = 1; i < RE_MAX_RENDERTARGETS; i++)
					desc.Graphics.RenderTargetFormats[i] = FORMAT_UNKNOWN;
				desc.Graphics.DepthStencilFormat = FORMAT_UNKNOWN;

				desc.Graphics.DepthStencilState.DepthEnable = true;
				desc.Graphics.DepthStencilState.DepthWriteMask = DEPTH_WRITE_MASK_ALL;
				desc.Graphics.DepthStencilState.DepthFunc = COMPARISON_FUNC_LESS;
				desc.Graphics.DepthStencilState.StencilEnable = false;
				desc.Graphics.DepthStencilState.StencilReadMask = 0;
				desc.Graphics.DepthStencilState.StencilWriteMask = 0;
				desc.Graphics.DepthStencilState.FrontFace.StencilFunc = COMPARISON_FUNC_ALWAYS;
				desc.Graphics.DepthStencilState.FrontFace.StencilFailOperation = STENCIL_OPERATION_KEEP;
				desc.Graphics.DepthStencilState.FrontFace.StencilPassOperation = STENCIL_OPERATION_KEEP;
				desc.Graphics.DepthStencilState.FrontFace.StencilDepthFailOperation = STENCIL_OPERATION_KEEP;
				desc.Graphics.DepthStencilState.BackFace = desc.Graphics.DepthStencilState.FrontFace;

				desc.Graphics.InputLayout.ElementCount = 0;
				desc.Graphics.InputLayout.pElements = nullptr;

				desc.Graphics.RasterizerState.FillMode = FILL_MODE_SOLID;
				desc.Graphics.RasterizerState.CullMode = CULL_MODE_BACK;
				desc.Graphics.RasterizerState.FrontCounterClockwise = false;
				desc.Graphics.RasterizerState.ForcedSampleCount = 0;
				desc.Graphics.RasterizerState.DepthBias = 0;
				desc.Graphics.RasterizerState.DepthBiasClamp = 0.0f;
				desc.Graphics.RasterizerState.SlopeScaleDepthBias = 0.0f;
				desc.Graphics.RasterizerState.DepthClipEnable = true;
				desc.Graphics.RasterizerState.ScissorEnable = false;
				desc.Graphics.RasterizerState.MultisampleEnable = false;
				desc.Graphics.RasterizerState.AntialiasedLineEnable = false;
				desc.Graphics.RasterizerState.ConservativeRasterizerEnable = false;

				desc.Graphics.pVertexShader = nullptr;
				desc.Graphics.pHullShader = nullptr;
				desc.Graphics.pDomainShader = nullptr;
				desc.Graphics.pGeometryShader = nullptr;
				desc.Graphics.pPixelShader = nullptr;

				desc.Graphics.SampleMask = 0xffffffff;
				desc.Graphics.Topology = PRIMITIVE_TOPOLOGY_UNKNOWN;

				return desc;
			}
		};


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API IPipelineState : public IObject
		{
			RE_INTERFACE(IPipelineState);

		public:
			IPipelineState() {}
			~IPipelineState() {}
		};
	}
}