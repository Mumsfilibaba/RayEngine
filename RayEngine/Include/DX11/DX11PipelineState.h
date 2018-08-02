#pragma once

#include "..\Graphics\IPipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\DX11\DX11Shader.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11PipelineState : public IPipelineState
		{
		public:
			DX11PipelineState(const DX11PipelineState& other) = delete;
			DX11PipelineState& operator=(const DX11PipelineState& other) = delete;
			DX11PipelineState(DX11PipelineState&& other) = delete;
			DX11PipelineState& operator=(DX11PipelineState&& other) = delete;

		public:
			DX11PipelineState(IDevice* pdevice, const PipelineStateInfo& info);
			~DX11PipelineState();

			ID3D11InputLayout* GetD3D11InputLayout() const;
			ID3D11RasterizerState* GetD3D11RasterizerState() const;
			ID3D11DepthStencilState* GetD3D11DepthStencilState() const;
			ID3D11BlendState* GetD3D11BlendState() const;
			const float* GetBlendFactor() const;
			uint8 GetSampleMask() const;
			ID3D11VertexShader* GetD3D11VertexShader() const;
			ID3D11HullShader* GetD3D11HullShader() const;
			ID3D11DomainShader* GetD3D11DomainShader() const;
			ID3D11GeometryShader* GetD3D11GeometryShader() const;
			ID3D11PixelShader* GetD3D11PixelShader() const;
			ID3D11ComputeShader* GetD3D11ComputeShader() const;
			DX11Shader* GetDX11VertexShader() const;
			DX11Shader* GetDX11HullShader() const;
			DX11Shader* GetDX11DomainShader() const;
			DX11Shader* GetDX11GeometryShader() const;
			DX11Shader* GetDX11PixelShader() const;
			DX11Shader* GetDX11ComputeShader() const;

			PIPELINE_TYPE GetPipelineType() const override final;
			IDevice* GetDevice() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void ReleaseInterfaces();
			void Create(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateGraphicsState(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateComputeState(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateInputLayout(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateRasterizerState(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateDepthStencilState(IDevice* pDevice, const PipelineStateInfo& info);
			void CreateBlendState(IDevice* pDevice, const PipelineStateInfo& info);

		private:
			static void SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC& desc, const InputElementInfo& info);

		private:
			IDevice* m_Device;

			ID3D11InputLayout* m_InputLayout;
			ID3D11BlendState* m_BlendState;
			float m_BlendFactor[4];
			uint32 m_SampleMask;
			ID3D11RasterizerState* m_RasterizerState;
			ID3D11DepthStencilState* m_DepthStencilState;
			DX11Shader* m_VertexShader;
			DX11Shader* m_HullShader;
			DX11Shader* m_DomainShader;
			DX11Shader* m_GeometryShader;
			DX11Shader* m_PixelShader;
			DX11Shader* m_ComputeShader;
			PIPELINE_TYPE m_Type;

			mutable uint32 m_ReferenceCount;
		};
	}
}

#endif