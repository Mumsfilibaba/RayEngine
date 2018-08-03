#pragma once

#include "..\Graphics\IPipelineState.h"
#include "DX12Common.h"
#include "DX12Shader.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12PipelineState : public IPipelineState
		{
		public:
			DX12PipelineState(const DX12PipelineState& other) = delete;
			DX12PipelineState& operator=(const DX12PipelineState& other) = delete;
			DX12PipelineState(DX12PipelineState&& other) = delete;
			DX12PipelineState& operator=(DX12PipelineState&& other) = delete;

		public:
			DX12PipelineState(IDevice* pdevice, const PipelineStateInfo& info);
			~DX12PipelineState();

			ID3D12PipelineState* GetD3D12PipelineState() const;
			PIPELINE_TYPE GetPipelineType() const override final;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateGraphicsState(IDevice* pDevice, ID3D12RootSignature* pRootSignature, const PipelineStateInfo& info);
			void CreateComputeState(IDevice* pDevice, ID3D12RootSignature* pRootSignature, const PipelineStateInfo& info);
			
		private:
			static void SetShaderByteCode(D3D12_SHADER_BYTECODE& byteCode, const DX12Shader* shader);
			static void SetInputElementDesc(D3D12_INPUT_ELEMENT_DESC& desc, const InputElementInfo& element);
			static void SetRasterizerDesc(D3D12_RASTERIZER_DESC& desc, const RasterizerStateInfo& info);
			static void SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC& desc, const DepthStencilStateInfo& info);
			static void SetBlendDesc(D3D12_BLEND_DESC& desc, const BlendStateInfo& info);

		private:
			IDevice* m_Device;
			ID3D12PipelineState* m_PipelineState;
			PIPELINE_TYPE m_Type;
		};
	}
}

#endif