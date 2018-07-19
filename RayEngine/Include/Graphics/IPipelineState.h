#pragma once

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
		struct RasterizerInfo
		{

		};



		/////////////////////////////////////////////////////////////
		struct DepthStencilInfo
		{

		};



		/////////////////////////////////////////////////////////////
		struct BlendInfo
		{

		};



		/////////////////////////////////////////////////////////////
		struct PipelineStateInfo
		{
			PIPELINETYPE Type = PIPELINETYPE_UNKNOWN;
			IRootSignature* RootSignature = nullptr;

			union
			{
				struct
				{
					IShader* ComputeShader = nullptr;
				} ComputePipeline;
				
				struct
				{
					RasterizerInfo RasterizerState;
					DepthStencilInfo DepthStencilState;
					BlendInfo BlendState;
					IShader* VertexShader = nullptr;
					IShader* HullShader = nullptr;
					IShader* DomainShadr = nullptr;
					IShader* GeometryShader = nullptr;
					IShader* PixelShader = nullptr;
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
		};
	}
}