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
		enum PIPELINETYPE : int32
		{
			PIPELINETYPE_UNKNOWN = 0,
			PIPELINETYPE_GRAPHICS = 1,
			PIPELINETYPE_COMPUTE = 2,
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

		};



		/////////////////////////////////////////////////////////////
		struct DepthStencilStateInfo
		{

		};



		/////////////////////////////////////////////////////////////
		struct BlendStateInfo
		{

		};



		/////////////////////////////////////////////////////////////
		struct PipelineStateInfo
		{
			PIPELINETYPE Type = PIPELINETYPE_UNKNOWN;
			IRootSignature* pRootSignature = nullptr;

			union
			{
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
		};



		/////////////////////////////////////////////////////////////
		class IPipelineState
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
			virtual PIPELINETYPE GetPipelineType() const = 0;
		};
	}
}