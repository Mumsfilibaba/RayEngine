#pragma once

#include "..\Graphics\IPipelineState.h"
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12PipelineState : public IPipelineState
		{
		public:
			DX12PipelineState(const DX12PipelineState& other) = delete;
			DX12PipelineState& operator=(const DX12PipelineState& other) = delete;

		public:
			DX12PipelineState(ID3D12Device* device, const PipelineStateInfo& info);
			DX12PipelineState(DX12PipelineState&& other);
			~DX12PipelineState();

			DX12PipelineState& operator=(DX12PipelineState&& other);

		private:
			void Create(ID3D12Device* device, const PipelineStateInfo& info);
			void CreateGraphicsState(ID3D12Device* device, ID3D12RootSignature* rootSignature, const PipelineStateInfo& info);
			void CreateComputeState(ID3D12Device* device, ID3D12RootSignature* rootSignature, const PipelineStateInfo& info);

		private:
			ID3D12PipelineState* m_PipelineState;
		};
	}
}