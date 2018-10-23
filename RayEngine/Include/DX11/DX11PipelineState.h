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
#include "..\DX11\DX11Shader.h"

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
			RE_IMPLEMENT_INTERFACE(DX11PipelineState);

		public:
			DX11PipelineState(IDevice* pdevice, const PipelineStateDesc* pDesc);
			~DX11PipelineState();

			inline ID3D11InputLayout* GetD3D11InputLayout() const
			{
				return m_InputLayout;
			}
			
			inline ID3D11RasterizerState* GetD3D11RasterizerState() const
			{
				return m_RasterizerState;
			}
			
			inline ID3D11DepthStencilState* GetD3D11DepthStencilState() const
			{
				return m_DepthStencilState;
			}
			
			inline ID3D11BlendState* GetD3D11BlendState() const
			{
				return m_BlendState;
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
				return (m_VS == nullptr) ? nullptr : m_VS->GetD3D11Shader<ID3D11VertexShader>();
			}
			
			inline ID3D11HullShader* GetD3D11HullShader() const
			{
				return (m_HS == nullptr) ? nullptr : m_HS->GetD3D11Shader<ID3D11HullShader>();
			}
			
			inline ID3D11DomainShader* GetD3D11DomainShader() const
			{
				return (m_DS == nullptr) ? nullptr : m_DS->GetD3D11Shader<ID3D11DomainShader>();
			}
			
			inline ID3D11GeometryShader* GetD3D11GeometryShader() const
			{
				return (m_GS == nullptr) ? nullptr : m_GS->GetD3D11Shader<ID3D11GeometryShader>();
			}
			
			inline ID3D11PixelShader* GetD3D11PixelShader() const
			{
				return (m_PS == nullptr) ? nullptr : m_PS->GetD3D11Shader<ID3D11PixelShader>();
			}
			
			inline ID3D11ComputeShader* GetD3D11ComputeShader() const
			{
				return (m_CS == nullptr) ? nullptr : m_CS->GetD3D11Shader<ID3D11ComputeShader>();
			}
			
			inline DX11Shader* GetDX11VertexShader() const
			{
				return m_VS;
			}
			
			inline DX11Shader* GetDX11HullShader() const
			{
				return m_HS;
			}
			
			inline DX11Shader* GetDX11DomainShader() const
			{
				return m_DS;
			}
			
			inline DX11Shader* GetDX11GeometryShader() const
			{
				return m_GS;
			}
			
			inline DX11Shader* GetDX11PixelShader() const
			{
				return m_PS;
			}

			inline DX11Shader* GetDX11ComputeShader() const
			{
				return m_CS;
			}

			PIPELINE_TYPE GetPipelineType() const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void ReleaseInterfaces();
			
			void Create(const PipelineStateDesc* pDesc);
			
			void CreateGraphicsState(const PipelineStateDesc* pDesc);
			
			void CreateComputeState(const PipelineStateDesc* pDesc);
			
			void CreateInputLayout(const PipelineStateDesc* pDesc);
			
			void CreateRasterizerState(const PipelineStateDesc* pDesc);
			
			void CreateDepthStencilState(const PipelineStateDesc* pDesc);
			
			void CreateBlendState(const PipelineStateDesc* pDesc);

		private:
			static void SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC* desc, const InputElementDesc* info);

		private:
			DX11Device* m_Device;
			ID3D11InputLayout* m_InputLayout;
			ID3D11BlendState* m_BlendState;
			ID3D11RasterizerState* m_RasterizerState;
			ID3D11DepthStencilState* m_DepthStencilState;
			DX11Shader* m_VS;
			DX11Shader* m_HS;
			DX11Shader* m_DS;
			DX11Shader* m_GS;
			DX11Shader* m_PS;
			DX11Shader* m_CS;
			
			float m_BlendFactor[4];
			
			uint32 m_SampleMask;
			
			PIPELINE_TYPE m_Type;

			IObject::CounterType m_References;
		};
	}
}

#endif