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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX11/DX11Device.h>
#include <DX11/DX11Shader.h>
#include <DX11/DX11PipelineState.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11PipelineState::DX11PipelineState(DX11Device* pDevice, const PipelineStateDesc* pDesc)
			: m_InputLayout(nullptr),
			m_BlendState(nullptr),
			m_RasterizerState(nullptr),
			m_DepthStencilState(nullptr),
			m_VS(nullptr),
			m_HS(nullptr),
			m_DS(nullptr),
			m_GS(nullptr),
			m_PS(nullptr),
			m_CS(nullptr),
			m_Type(PIPELINE_TYPE_UNKNOWN),
			m_SampleMask(0),
			m_References(0)
		{
			AddRef();
			Create(pDevice, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11PipelineState::~DX11PipelineState()
		{
			ReleaseInterfaces();
		}

		PIPELINE_TYPE DX11PipelineState::GetPipelineType() const
		{
			return m_Type;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11PipelineState::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11PipelineState::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::ReleaseInterfaces()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::Create(DX11Device* pDevice, const PipelineStateDesc* pDesc)
		{
			m_Type = pDesc->Type;

			if (m_Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState(pDevice, pDesc);
			else if (m_Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateGraphicsState(DX11Device* pDevice, const PipelineStateDesc* pDesc)
		{
			DX11Shader* pShader = nullptr;
			if (pDesc->Graphics.pVertexShader != nullptr)
			{
				pShader = reinterpret_cast<DX11Shader*>(pDesc->Graphics.pVertexShader);
				m_VS = pShader->GetD3D11Shader<ID3D11VertexShader>();
				m_VS->AddRef();
			}
			if (pDesc->Graphics.pHullShader != nullptr)
			{
				pShader = reinterpret_cast<DX11Shader*>(pDesc->Graphics.pHullShader);
				m_HS = pShader->GetD3D11Shader<ID3D11HullShader>();
				m_HS->AddRef();
			}
			if (pDesc->Graphics.pDomainShader != nullptr)
			{
				pShader = reinterpret_cast<DX11Shader*>(pDesc->Graphics.pDomainShader);
				m_DS = pShader->GetD3D11Shader<ID3D11DomainShader>();
				m_DS->AddRef();
			}
			if (pDesc->Graphics.pGeometryShader != nullptr)
			{
				pShader = reinterpret_cast<DX11Shader*>(pDesc->Graphics.pGeometryShader);
				m_GS = pShader->GetD3D11Shader<ID3D11GeometryShader>();
				m_GS->AddRef();
			}
			if (pDesc->Graphics.pPixelShader != nullptr)
			{
				pShader = reinterpret_cast<DX11Shader*>(pDesc->Graphics.pPixelShader);
				m_PS = pShader->GetD3D11Shader<ID3D11PixelShader>();
				m_PS->AddRef();
			}

			CreateInputLayout(pDevice, pDesc);
			CreateBlendState(pDevice, pDesc);
			CreateRasterizerState(pDevice, pDesc);
			CreateDepthStencilState(pDevice, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateComputeState(const PipelineStateDesc* pDesc)
		{
			if (pDesc->Compute.pComputeShader != nullptr)
			{
				DX11Shader* pShader = reinterpret_cast<DX11Shader*>(pDesc->Compute.pComputeShader);
				m_CS = pShader->GetD3D11Shader<ID3D11ComputeShader>();
				m_CS->AddRef();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateInputLayout(DX11Device* pDevice, const PipelineStateDesc* pDesc)
		{
			if (pDesc->Graphics.InputLayout.ElementCount < 1)
				return;

			DX11Shader* pVS = reinterpret_cast<DX11Shader*>(pDesc->Graphics.pVertexShader);
			if (pVS == nullptr)
			{
				LOG_ERROR("D3D11: Cannot create a inputlayout without a VertexShader.");
				return;
			}

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(pDesc->Graphics.InputLayout.ElementCount);

			for (int32 i = 0; i < pDesc->Graphics.InputLayout.ElementCount; i++)
				SetInputElementDesc(&inputLayout[i], &pDesc->Graphics.InputLayout.pElements[i]);

			ID3D11Device* pD3D11Device = pDevice->GetD3D11Device();
			ID3DBlob* pD3DBlob = pVS->GetBlob();
			
			HRESULT hr = pD3D11Device->CreateInputLayout(inputLayout.data(), static_cast<uint32>(inputLayout.size()), 
				pD3DBlob->GetBufferPointer(), static_cast<size_t>(pD3DBlob->GetBufferSize()), &m_InputLayout);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create InputLayout. " + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateRasterizerState(DX11Device* pDevice, const PipelineStateDesc* pDesc)
		{
			D3D11_RASTERIZER_DESC desc = {};
			desc.FillMode = (pDesc->Graphics.RasterizerState.FillMode == FILL_MODE_SOLID) 
				? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

			if (pDesc->Graphics.RasterizerState.CullMode == CULL_MODE_BACK)
				desc.CullMode = D3D11_CULL_BACK;
			else if (pDesc->Graphics.RasterizerState.CullMode == CULL_MODE_FRONT)
				desc.CullMode = D3D11_CULL_FRONT;
			else if (pDesc->Graphics.RasterizerState.CullMode == CULL_MODE_NONE)
				desc.CullMode = D3D11_CULL_NONE;

			desc.FrontCounterClockwise = pDesc->Graphics.RasterizerState.FrontCounterClockwise;

			desc.DepthClipEnable = pDesc->Graphics.RasterizerState.DepthClipEnable;
			desc.DepthBias = pDesc->Graphics.RasterizerState.DepthBias;
			desc.DepthBiasClamp = pDesc->Graphics.RasterizerState.DepthBiasClamp;
			desc.SlopeScaledDepthBias = pDesc->Graphics.RasterizerState.SlopeScaleDepthBias;
			
			desc.ScissorEnable = pDesc->Graphics.RasterizerState.ScissorEnable;
			desc.MultisampleEnable = pDesc->Graphics.RasterizerState.MultisampleEnable;
			desc.AntialiasedLineEnable = pDesc->Graphics.RasterizerState.AntialiasedLineEnable;

			ID3D11Device* pD3D11Device = pDevice->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateRasterizerState(&desc, &m_RasterizerState);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create RasterizerState." + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateDepthStencilState(DX11Device* pDevice, const PipelineStateDesc* pDesc)
		{
			D3D11_DEPTH_STENCIL_DESC desc = {};
			desc.DepthEnable = pDesc->Graphics.DepthStencilState.DepthEnable;
			desc.DepthFunc = ReToDX11ComparisonFunc(pDesc->Graphics.DepthStencilState.DepthFunc);
			desc.DepthWriteMask = (pDesc->Graphics.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL) 
				? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			desc.StencilEnable = pDesc->Graphics.DepthStencilState.StencilEnable;
			desc.StencilReadMask = pDesc->Graphics.DepthStencilState.StencilReadMask;
			desc.StencilWriteMask = pDesc->Graphics.DepthStencilState.StencilWriteMask;

			desc.BackFace = ReToDX11StencilOpDesc(pDesc->Graphics.DepthStencilState.BackFace);
			desc.FrontFace = ReToDX11StencilOpDesc(pDesc->Graphics.DepthStencilState.FrontFace);


			ID3D11Device* pD3D11Device = pDevice->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateDepthStencilState(&desc, &m_DepthStencilState);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create DepthStencilState" + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateBlendState(DX11Device* pDevice, const PipelineStateDesc* pDesc)
		{
			D3D11_BLEND_DESC desc = {};
			desc.AlphaToCoverageEnable = pDesc->Graphics.BlendState.AlphaToCoverageEnable;
			desc.IndependentBlendEnable = pDesc->Graphics.BlendState.IndependentBlendEnable;
			
			for (int32 i = 0; i < 8; i++)
			{
				desc.RenderTarget[i].BlendEnable = pDesc->Graphics.BlendState.RenderTargets[i].BlendEnable;
				
				desc.RenderTarget[i].SrcBlend = ReToDX11Blend(pDesc->Graphics.BlendState.RenderTargets[i].SrcBlend);
				desc.RenderTarget[i].DestBlend = ReToDX11Blend(pDesc->Graphics.BlendState.RenderTargets[i].DstBlend);
				desc.RenderTarget[i].BlendOp = ReToDX11BlendOp(pDesc->Graphics.BlendState.RenderTargets[i].BlendOperation);
				
				desc.RenderTarget[i].SrcBlendAlpha = ReToDX11Blend(pDesc->Graphics.BlendState.RenderTargets[i].SrcAlphaBlend);
				desc.RenderTarget[i].DestBlendAlpha = ReToDX11Blend(pDesc->Graphics.BlendState.RenderTargets[i].DstAlphaBlend);
				desc.RenderTarget[i].BlendOpAlpha = ReToDX11BlendOp(pDesc->Graphics.BlendState.RenderTargets[i].AlphaBlendOperation);

				uint8 renderTargetWriteMask = 0;
				if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask == COLOR_WRITE_ENABLE_ALL)
				{
					renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				}
				else
				{
					if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
					if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
					if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
					if (pDesc->Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
				}

				desc.RenderTarget[i].RenderTargetWriteMask = renderTargetWriteMask;
			}

			m_BlendFactor[0] = pDesc->Graphics.BlendState.BlendFactor[0];
			m_BlendFactor[1] = pDesc->Graphics.BlendState.BlendFactor[1];
			m_BlendFactor[2] = pDesc->Graphics.BlendState.BlendFactor[2];
			m_BlendFactor[3] = pDesc->Graphics.BlendState.BlendFactor[3];
			m_SampleMask = pDesc->Graphics.SampleMask;


			ID3D11Device* pD3D11Device = pDevice->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBlendState(&desc, &m_BlendState);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create BlendState" + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC* pD3D11Desc, const InputElementDesc* pDesc)
		{
			pD3D11Desc->AlignedByteOffset = static_cast<uint32>(pDesc->ElementOffset);
			pD3D11Desc->Format = ReToDXFormat(pDesc->Format);
			pD3D11Desc->InputSlot = static_cast<uint32>(pDesc->InputSlot);
			pD3D11Desc->InputSlotClass = (pDesc->StepType == ELEMENT_STEP_TYPE_VERTEX) ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
			pD3D11Desc->InstanceDataStepRate = static_cast<uint32>(pDesc->DataStepRate);
			pD3D11Desc->SemanticName = pDesc->pSemantic;
			pD3D11Desc->SemanticIndex = pDesc->SemanticIndex;
		}
	}
}

#endif