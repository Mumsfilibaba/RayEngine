#pragma once

#include "..\Graphics\IPipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Shader.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12PipelineState final : public IPipelineState
		{
		public:
			DX12PipelineState(const DX12PipelineState& other) = delete;
			DX12PipelineState& operator=(const DX12PipelineState& other) = delete;
			DX12PipelineState(DX12PipelineState&& other) = delete;
			DX12PipelineState& operator=(DX12PipelineState&& other) = delete;

		public:
			DX12PipelineState(IDevice* pdevice, const PipelineStateInfo& info);
			~DX12PipelineState();

			ID3D12RootSignature* GetD3D12RootSignature() const;
			ID3D12PipelineState* GetD3D12PipelineState() const;

			PIPELINE_TYPE GetPipelineType() const override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const PipelineStateInfo& info);
			void CreateGraphicsState(const PipelineStateInfo& info);
			void CreateComputeState(const PipelineStateInfo& info);
			bool CreateRootSignature(const PipelineStateInfo& info);
			
		private:
			static void SetShaderByteCode(D3D12_SHADER_BYTECODE& byteCode, const DX12Shader* shader);
			static void SetInputElementDesc(D3D12_INPUT_ELEMENT_DESC& desc, const InputElementInfo& element);
			static void SetRasterizerDesc(D3D12_RASTERIZER_DESC& desc, const RasterizerStateInfo& info);
			static void SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC& desc, const DepthStencilStateInfo& info);
			static void SetBlendDesc(D3D12_BLEND_DESC& desc, const BlendStateInfo& info);
			static void GetStaticSamplersFromShader(std::vector<D3D12_STATIC_SAMPLER_DESC>& samplers, DX12Shader* pShader);
			static void GetVariablesFromShader(std::vector<D3D12_ROOT_PARAMETER1>& parameters, DX12Shader* pShader);

		private:
			DX12Device* m_Device;
			ID3D12RootSignature* m_RootSignature;
			ID3D12PipelineState* m_PipelineState;
			PIPELINE_TYPE m_Type;
		};
	}
}

#endif