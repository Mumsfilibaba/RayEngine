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
		struct PipelineStateInfo
		{
			IRootSignature* RootSignature = nullptr;
			IShader* VertexShader = nullptr;
			IShader* HullShader = nullptr;
			IShader* DomainShadr = nullptr;
			IShader* GeometryShader = nullptr;
			IShader* PixelShader = nullptr;
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