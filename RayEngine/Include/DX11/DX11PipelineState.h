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
#include <Graphics\IPipelineState.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX11\DX11Shader.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device;
		class DX11Shader;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11PipelineState final : public IPipelineState
		{
			friend class DX11DeviceContext;

			RE_IMPLEMENT_INTERFACE(DX11PipelineState);

		public:
			DX11PipelineState(DX11Device* pdevice, const PipelineStateDesc* pDesc);
			~DX11PipelineState();

			PIPELINE_TYPE GetPipelineType() const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			inline ID3D11InputLayout* GetD3D11InputLayout() const
			{
				return m_InputLayout.Get();
			}

			inline ID3D11RasterizerState* GetD3D11RasterizerState() const
			{
				return m_RasterizerState.Get();
			}

			inline ID3D11DepthStencilState* GetD3D11DepthStencilState() const
			{
				return m_DepthStencilState.Get();
			}

			inline ID3D11BlendState* GetD3D11BlendState() const
			{
				return m_BlendState.Get();
			}

			inline const float* GetBlendFactor() const
			{
				return m_BlendFactor;
			}

			inline uint8 GetSampleMask() const
			{
				return m_SampleMask;
			}

			inline ID3D11VertexShader* GetD3D11VertexShader() const
			{
				return m_VS.Get();
			}

			inline ID3D11HullShader* GetD3D11HullShader() const
			{
				return m_HS.Get();
			}

			inline ID3D11DomainShader* GetD3D11DomainShader() const
			{
				return m_DS.Get();
			}

			inline ID3D11GeometryShader* GetD3D11GeometryShader() const
			{
				return m_GS.Get();
			}

			inline ID3D11PixelShader* GetD3D11PixelShader() const
			{
				return m_PS.Get();
			}

			inline ID3D11ComputeShader* GetD3D11ComputeShader() const
			{
				return m_CS.Get();
			}

			void ReleaseInterfaces();
			
			void Create(DX11Device* pDevice, const PipelineStateDesc* pDesc);
			
			void CreateGraphicsState(DX11Device* pDevice, const PipelineStateDesc* pDesc);
			
			void CreateComputeState(const PipelineStateDesc* pDesc);
			
			void CreateInputLayout(DX11Device* pDevice, const PipelineStateDesc* pDesc);
			
			void CreateRasterizerState(DX11Device* pDevice, const PipelineStateDesc* pDesc);
			
			void CreateDepthStencilState(DX11Device* pDevice, const PipelineStateDesc* pDesc);
			
			void CreateBlendState(DX11Device* pDevice, const PipelineStateDesc* pDesc);

		private:
			static void SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC* pD3D11Desc, const InputElementDesc* pDesc);

		private:
			Microsoft::WRL::ComPtr<ID3D11InputLayout> m_InputLayout;
			Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendState;
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterizerState;
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStencilState;
			Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VS;
			Microsoft::WRL::ComPtr<ID3D11HullShader> m_HS;
			Microsoft::WRL::ComPtr<ID3D11DomainShader> m_DS;
			Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GS;
			Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS;
			Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_CS;

			PIPELINE_TYPE m_Type;

			float m_BlendFactor[4];
			
			uint32 m_SampleMask;

			CounterType m_References;
		};
	}
}

#endif