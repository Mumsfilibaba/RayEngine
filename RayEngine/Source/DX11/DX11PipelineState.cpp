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

#include <vector>
#include "..\..\Include\DX11\DX11PipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Shader.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11PipelineState::DX11PipelineState(IDevice* pDevice, const PipelineStateInfo& info)
			: m_Device(nullptr),
			m_InputLayout(nullptr),
			m_BlendState(nullptr),
			m_RasterizerState(nullptr),
			m_DepthStencilState(nullptr),
			m_VS(nullptr),
			m_HS(nullptr),
			m_DS(nullptr),
			m_GS(nullptr),
			m_PS(nullptr),
			m_CS(nullptr),
			m_SampleMask(0),
			m_Type(PIPELINE_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11PipelineState::~DX11PipelineState()
		{
			ReleaseInterfaces();
		}



		/////////////////////////////////////////////////////////////
		ID3D11InputLayout* DX11PipelineState::GetD3D11InputLayout() const
		{
			return m_InputLayout;
		}



		/////////////////////////////////////////////////////////////
		ID3D11RasterizerState* DX11PipelineState::GetD3D11RasterizerState() const
		{
			return m_RasterizerState;
		}



		/////////////////////////////////////////////////////////////
		ID3D11DepthStencilState* DX11PipelineState::GetD3D11DepthStencilState() const
		{
			return m_DepthStencilState;
		}



		/////////////////////////////////////////////////////////////
		ID3D11BlendState* DX11PipelineState::GetD3D11BlendState() const
		{
			return m_BlendState;
		}



		/////////////////////////////////////////////////////////////
		const float* DX11PipelineState::GetBlendFactor() const
		{
			return m_BlendFactor;
		}



		/////////////////////////////////////////////////////////////
		uint8 DX11PipelineState::GetSampleMask() const
		{
			return m_SampleMask;
		}



		/////////////////////////////////////////////////////////////
		ID3D11VertexShader* DX11PipelineState::GetD3D11VertexShader() const
		{
			return (m_VS == nullptr) ? nullptr : m_VS->GetD3D11Shader<ID3D11VertexShader>();
		}



		/////////////////////////////////////////////////////////////
		ID3D11HullShader* DX11PipelineState::GetD3D11HullShader() const
		{
			return (m_HS == nullptr) ? nullptr : m_HS->GetD3D11Shader<ID3D11HullShader>();
		}



		/////////////////////////////////////////////////////////////
		ID3D11DomainShader* DX11PipelineState::GetD3D11DomainShader() const
		{
			return (m_DS == nullptr) ? nullptr : m_DS->GetD3D11Shader<ID3D11DomainShader>();
		}



		/////////////////////////////////////////////////////////////
		ID3D11GeometryShader* DX11PipelineState::GetD3D11GeometryShader() const
		{
			return (m_GS == nullptr) ? nullptr : m_GS->GetD3D11Shader<ID3D11GeometryShader>();
		}



		/////////////////////////////////////////////////////////////
		ID3D11PixelShader* DX11PipelineState::GetD3D11PixelShader() const
		{
			return (m_PS == nullptr) ? nullptr : m_PS->GetD3D11Shader<ID3D11PixelShader>();
		}



		/////////////////////////////////////////////////////////////
		ID3D11ComputeShader* DX11PipelineState::GetD3D11ComputeShader() const
		{
			return (m_CS == nullptr) ? nullptr : m_CS->GetD3D11Shader<ID3D11ComputeShader>();
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11VertexShader() const
		{
			return m_VS;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11HullShader() const
		{
			return m_HS;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11DomainShader() const
		{
			return m_DS;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11GeometryShader() const
		{
			return m_GS;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11PixelShader() const
		{
			return m_PS;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11ComputeShader() const
		{
			return m_CS;
		}



		/////////////////////////////////////////////////////////////
		PIPELINE_TYPE DX11PipelineState::GetPipelineType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::ReleaseInterfaces()
		{
			D3DRelease_S(m_InputLayout);
			D3DRelease_S(m_BlendState);
			D3DRelease_S(m_DepthStencilState);
			D3DRelease_S(m_RasterizerState);

			ReRelease_S(m_Device);
			ReRelease_S(m_VS);
			ReRelease_S(m_HS);
			ReRelease_S(m_DS);
			ReRelease_S(m_GS);
			ReRelease_S(m_PS);
			ReRelease_S(m_CS);
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::Create(const PipelineStateInfo& info)
		{
			m_Type = info.Type;
			if (info.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState(info);
			else if (info.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState(info);
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateGraphicsState(const PipelineStateInfo& info)
		{
			if (info.GraphicsPipeline.pVertexShader != nullptr)
				m_VS = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pVertexShader->QueryReference());
			if (info.GraphicsPipeline.pHullShader != nullptr)
				m_HS = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pHullShader->QueryReference());
			if (info.GraphicsPipeline.pDomainShader != nullptr)
				m_DS = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pDomainShader->QueryReference());
			if (info.GraphicsPipeline.pGeometryShader != nullptr)
				m_GS = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pGeometryShader->QueryReference());
			if (info.GraphicsPipeline.pPixelShader != nullptr)
				m_PS = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pPixelShader->QueryReference());


			CreateInputLayout(info);
			CreateBlendState(info);
			CreateRasterizerState(info);
			CreateDepthStencilState(info);
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateComputeState(const PipelineStateInfo& info)
		{
			//TODO: Compute states
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateInputLayout(const PipelineStateInfo& info)
		{
			using namespace System;

			if (info.GraphicsPipeline.InputLayout.ElementCount < 1)
				return;


			if (m_VS == nullptr)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Cannot create a inputlayout without a VertexShader.");
				return;
			}



			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(info.GraphicsPipeline.InputLayout.ElementCount);

			for (int32 i = 0; i < info.GraphicsPipeline.InputLayout.ElementCount; i++)
				SetInputElementDesc(inputLayout[i], info.GraphicsPipeline.InputLayout.pElements[i]);



			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			ID3DBlob* pD3DBlob = m_VS->GetBlob();
			
			HRESULT hr = pD3D11Device->CreateInputLayout(inputLayout.data(), static_cast<uint32>(inputLayout.size()), 
				pD3DBlob->GetBufferPointer(), static_cast<size_t>(pD3DBlob->GetBufferSize()), &m_InputLayout);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create InputLayout. " + DXErrorString(hr));
			}
			else
			{
				std::string name = info.Name + ": InputLayout";
				m_InputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateRasterizerState(const PipelineStateInfo& info)
		{
			using namespace System;

			D3D11_RASTERIZER_DESC desc = {};
			desc.FillMode = (info.GraphicsPipeline.RasterizerState.FillMode == FILL_MODE_SOLID) 
				? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

			if (info.GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_BACK)
				desc.CullMode = D3D11_CULL_BACK;
			else if (info.GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_FRONT)
				desc.CullMode = D3D11_CULL_FRONT;
			else if (info.GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_NONE)
				desc.CullMode = D3D11_CULL_NONE;

			desc.FrontCounterClockwise = info.GraphicsPipeline.RasterizerState.FrontCounterClockwise;

			desc.DepthClipEnable = info.GraphicsPipeline.RasterizerState.DepthClipEnable;
			desc.DepthBias = info.GraphicsPipeline.RasterizerState.DepthBias;
			desc.DepthBiasClamp = info.GraphicsPipeline.RasterizerState.DepthBiasClamp;
			desc.SlopeScaledDepthBias = info.GraphicsPipeline.RasterizerState.SlopeScaleDepthBias;
			
			desc.ScissorEnable = info.GraphicsPipeline.RasterizerState.ScissorEnable;
			desc.MultisampleEnable = info.GraphicsPipeline.RasterizerState.MultisampleEnable;
			desc.AntialiasedLineEnable = info.GraphicsPipeline.RasterizerState.AntialiasedLineEnable;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateRasterizerState(&desc, &m_RasterizerState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create RasterizerState." + DXErrorString(hr));
			}
			else
			{
				std::string name = info.Name + ": RasterizerState";
				m_RasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateDepthStencilState(const PipelineStateInfo& info)
		{
			using namespace System;

			D3D11_DEPTH_STENCIL_DESC desc = {};
			desc.DepthEnable = info.GraphicsPipeline.DepthStencilState.DepthEnable;
			desc.DepthFunc = ReToDX11ComparisonFunc(info.GraphicsPipeline.DepthStencilState.DepthFunc);
			desc.DepthWriteMask = (info.GraphicsPipeline.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL) 
				? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			desc.StencilEnable = info.GraphicsPipeline.DepthStencilState.StencilEnable;
			desc.StencilReadMask = info.GraphicsPipeline.DepthStencilState.StencilReadMask;
			desc.StencilWriteMask = info.GraphicsPipeline.DepthStencilState.StencilWriteMask;

			desc.BackFace = ReToDX11StencilOpDesc(info.GraphicsPipeline.DepthStencilState.BackFace);
			desc.FrontFace = ReToDX11StencilOpDesc(info.GraphicsPipeline.DepthStencilState.Frontface);


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateDepthStencilState(&desc, &m_DepthStencilState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DepthStencilState" + DXErrorString(hr));
			}
			else
			{
				std::string name = info.Name + ": DepthStencilState";
				m_DepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateBlendState(const PipelineStateInfo& info)
		{
			using namespace System;

			D3D11_BLEND_DESC desc = {};
			desc.AlphaToCoverageEnable = info.GraphicsPipeline.BlendState.AlphaToCoverageEnable;
			desc.IndependentBlendEnable = info.GraphicsPipeline.BlendState.IndependentBlendEnable;
			
			for (int32 i = 0; i < 8; i++)
			{
				desc.RenderTarget[i].BlendEnable = info.GraphicsPipeline.BlendState.RenderTargets[i].BlendEnable;
				
				desc.RenderTarget[i].SrcBlend = ReToDX11Blend(info.GraphicsPipeline.BlendState.RenderTargets[i].SrcBlend);
				desc.RenderTarget[i].DestBlend = ReToDX11Blend(info.GraphicsPipeline.BlendState.RenderTargets[i].DstBlend);
				desc.RenderTarget[i].BlendOp = ReToDX11BlendOp(info.GraphicsPipeline.BlendState.RenderTargets[i].BlendOperation);
				
				desc.RenderTarget[i].SrcBlendAlpha = ReToDX11Blend(info.GraphicsPipeline.BlendState.RenderTargets[i].SrcAlphaBlend);
				desc.RenderTarget[i].DestBlendAlpha = ReToDX11Blend(info.GraphicsPipeline.BlendState.RenderTargets[i].DstAlphaBlend);
				desc.RenderTarget[i].BlendOpAlpha = ReToDX11BlendOp(info.GraphicsPipeline.BlendState.RenderTargets[i].AlphaBlendOperation);

				uint8 renderTargetWriteMask = 0;
				if (info.GraphicsPipeline.BlendState.RenderTargets[i].RenderTargetWriteMask == COLOR_WRITE_ENABLE_ALL)
				{
					renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				}
				else
				{
					if (info.GraphicsPipeline.BlendState.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_RED)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
					if (info.GraphicsPipeline.BlendState.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_GREEN)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
					if (info.GraphicsPipeline.BlendState.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_BLUE)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
					if (info.GraphicsPipeline.BlendState.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_ALPHA)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
				}

				desc.RenderTarget[i].RenderTargetWriteMask = renderTargetWriteMask;
			}

			m_BlendFactor[0] = info.GraphicsPipeline.BlendState.BlendFactor[0];
			m_BlendFactor[1] = info.GraphicsPipeline.BlendState.BlendFactor[1];
			m_BlendFactor[2] = info.GraphicsPipeline.BlendState.BlendFactor[2];
			m_BlendFactor[3] = info.GraphicsPipeline.BlendState.BlendFactor[3];
			m_SampleMask = info.GraphicsPipeline.SampleMask;


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBlendState(&desc, &m_BlendState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create BlendState" + DXErrorString(hr));
			}
			else
			{
				std::string name = info.Name + ": BlendState";
				m_BlendState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC& desc, const InputElementInfo& info)
		{
			desc.AlignedByteOffset = static_cast<uint32>(info.ElementOffset);
			desc.Format = ReToDXFormat(info.Format);
			desc.InputSlot = static_cast<uint32>(info.InputSlot);
			desc.InputSlotClass = (info.StepType == ELEMENT_STEP_TYPE_VERTEX) ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
			desc.InstanceDataStepRate = static_cast<uint32>(info.DataStepRate);
			desc.SemanticName = info.Semantic.c_str();
			desc.SemanticIndex = info.SemanticIndex;
		}
	}
}

#endif