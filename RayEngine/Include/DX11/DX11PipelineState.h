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
#include "..\Graphics\IPipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\DX11\DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;
		class DX11Shader;



		/////////////////////////////////////////////////////////////
		class DX11PipelineState final : public IPipelineState
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

			void SetName(const std::string& name) override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void ReleaseInterfaces();
			void Create(const PipelineStateInfo& info);
			void CreateGraphicsState(const PipelineStateInfo& info);
			void CreateComputeState(const PipelineStateInfo& info);
			void CreateInputLayout(const PipelineStateInfo& info);
			void CreateRasterizerState(const PipelineStateInfo& info);
			void CreateDepthStencilState(const PipelineStateInfo& info);
			void CreateBlendState(const PipelineStateInfo& info);

		private:
			static void SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC& desc, const InputElementInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11InputLayout* m_InputLayout;
			ID3D11BlendState* m_BlendState;
			float m_BlendFactor[4];
			uint32 m_SampleMask;
			ID3D11RasterizerState* m_RasterizerState;
			ID3D11DepthStencilState* m_DepthStencilState;
			DX11Shader* m_VS;
			DX11Shader* m_HS;
			DX11Shader* m_DS;
			DX11Shader* m_GS;
			DX11Shader* m_PS;
			DX11Shader* m_CS;
			PIPELINE_TYPE m_Type;
		};
	}
}

#endif