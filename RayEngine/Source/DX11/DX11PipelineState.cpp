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
		DX11PipelineState::DX11PipelineState(IDevice* pDevice, const PipelineStateDesc* pDesc)
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
			m_Desc(),
			m_SampleMask(0),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11PipelineState::~DX11PipelineState()
		{
			ReleaseInterfaces();

			if (m_Desc.Type == PIPELINE_TYPE_GRAPHICS)
			{
				if (m_Desc.Graphics.InputLayout.pElements != nullptr)
				{
					delete[] m_Desc.Graphics.InputLayout.pElements;
					m_Desc.Graphics.InputLayout.pElements = nullptr;
					
					m_Desc.Graphics.InputLayout.ElementCount = 0;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::GetDesc(PipelineStateDesc* pDesc) const
		{
			//We do not need to copy the inputelement array since we are the owner of the memory
			*pDesc = m_Desc;
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
			D3DRelease_S(m_InputLayout);
			D3DRelease_S(m_BlendState);
			D3DRelease_S(m_DepthStencilState);
			D3DRelease_S(m_RasterizerState);

			ReRelease_S(m_VS);
			ReRelease_S(m_HS);
			ReRelease_S(m_DS);
			ReRelease_S(m_GS);
			ReRelease_S(m_PS);

			ReRelease_S(m_CS);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::Create(const PipelineStateDesc* pDesc)
		{
			if (m_Desc.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState();
			else if (m_Desc.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateGraphicsState()
		{
			if (m_Desc.Graphics.pVertexShader != nullptr)
				m_VS = m_Desc.Graphics.pVertexShader->QueryReference<DX11Shader>();
			if (m_Desc.Graphics.pHullShader != nullptr)
				m_HS = m_Desc.Graphics.pHullShader->QueryReference<DX11Shader>();
			if (m_Desc.Graphics.pDomainShader != nullptr)
				m_DS = m_Desc.Graphics.pDomainShader->QueryReference<DX11Shader>();
			if (m_Desc.Graphics.pGeometryShader != nullptr)
				m_GS = m_Desc.Graphics.pGeometryShader->QueryReference<DX11Shader>();
			if (m_Desc.Graphics.pPixelShader != nullptr)
				m_PS = m_Desc.Graphics.pPixelShader->QueryReference<DX11Shader>();


			CreateInputLayout();
			CreateBlendState();
			CreateRasterizerState();
			CreateDepthStencilState();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateComputeState()
		{
			if (m_Desc.Compute.pComputeShader != nullptr)
				m_CS = m_Desc.Compute.pComputeShader->QueryReference<DX11Shader>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateInputLayout()
		{
			if (m_Desc.Graphics.InputLayout.ElementCount < 1)
				return;


			if (m_VS == nullptr)
			{
				LOG_ERROR("D3D11: Cannot create a inputlayout without a VertexShader.");
				return;
			}

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(m_Desc.Graphics.InputLayout.ElementCount);

			for (int32 i = 0; i < m_Desc.Graphics.InputLayout.ElementCount; i++)
				SetInputElementDesc(&inputLayout[i], &m_Desc.Graphics.InputLayout.pElements[i]);

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			ID3DBlob* pD3DBlob = m_VS->GetBlob();
			
			HRESULT hr = pD3D11Device->CreateInputLayout(inputLayout.data(), static_cast<uint32>(inputLayout.size()), 
				pD3DBlob->GetBufferPointer(), static_cast<size_t>(pD3DBlob->GetBufferSize()), &m_InputLayout);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create InputLayout. " + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateRasterizerState()
		{
			D3D11_RASTERIZER_DESC desc = {};
			desc.FillMode = (m_Desc.Graphics.RasterizerState.FillMode == FILL_MODE_SOLID) 
				? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

			if (m_Desc.Graphics.RasterizerState.CullMode == CULL_MODE_BACK)
				desc.CullMode = D3D11_CULL_BACK;
			else if (m_Desc.Graphics.RasterizerState.CullMode == CULL_MODE_FRONT)
				desc.CullMode = D3D11_CULL_FRONT;
			else if (m_Desc.Graphics.RasterizerState.CullMode == CULL_MODE_NONE)
				desc.CullMode = D3D11_CULL_NONE;

			desc.FrontCounterClockwise = m_Desc.Graphics.RasterizerState.FrontCounterClockwise;

			desc.DepthClipEnable = m_Desc.Graphics.RasterizerState.DepthClipEnable;
			desc.DepthBias = m_Desc.Graphics.RasterizerState.DepthBias;
			desc.DepthBiasClamp = m_Desc.Graphics.RasterizerState.DepthBiasClamp;
			desc.SlopeScaledDepthBias = m_Desc.Graphics.RasterizerState.SlopeScaleDepthBias;
			
			desc.ScissorEnable = m_Desc.Graphics.RasterizerState.ScissorEnable;
			desc.MultisampleEnable = m_Desc.Graphics.RasterizerState.MultisampleEnable;
			desc.AntialiasedLineEnable = m_Desc.Graphics.RasterizerState.AntialiasedLineEnable;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateRasterizerState(&desc, &m_RasterizerState);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create RasterizerState." + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateDepthStencilState()
		{
			D3D11_DEPTH_STENCIL_DESC desc = {};
			desc.DepthEnable = m_Desc.Graphics.DepthStencilState.DepthEnable;
			desc.DepthFunc = ReToDX11ComparisonFunc(m_Desc.Graphics.DepthStencilState.DepthFunc);
			desc.DepthWriteMask = (m_Desc.Graphics.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL) 
				? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			desc.StencilEnable = m_Desc.Graphics.DepthStencilState.StencilEnable;
			desc.StencilReadMask = m_Desc.Graphics.DepthStencilState.StencilReadMask;
			desc.StencilWriteMask = m_Desc.Graphics.DepthStencilState.StencilWriteMask;

			desc.BackFace = ReToDX11StencilOpDesc(m_Desc.Graphics.DepthStencilState.BackFace);
			desc.FrontFace = ReToDX11StencilOpDesc(m_Desc.Graphics.DepthStencilState.FrontFace);


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateDepthStencilState(&desc, &m_DepthStencilState);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create DepthStencilState" + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateBlendState()
		{
			D3D11_BLEND_DESC desc = {};
			desc.AlphaToCoverageEnable = m_Desc.Graphics.BlendState.AlphaToCoverageEnable;
			desc.IndependentBlendEnable = m_Desc.Graphics.BlendState.IndependentBlendEnable;
			
			for (int32 i = 0; i < 8; i++)
			{
				desc.RenderTarget[i].BlendEnable = m_Desc.Graphics.BlendState.RenderTargets[i].BlendEnable;
				
				desc.RenderTarget[i].SrcBlend = ReToDX11Blend(m_Desc.Graphics.BlendState.RenderTargets[i].SrcBlend);
				desc.RenderTarget[i].DestBlend = ReToDX11Blend(m_Desc.Graphics.BlendState.RenderTargets[i].DstBlend);
				desc.RenderTarget[i].BlendOp = ReToDX11BlendOp(m_Desc.Graphics.BlendState.RenderTargets[i].BlendOperation);
				
				desc.RenderTarget[i].SrcBlendAlpha = ReToDX11Blend(m_Desc.Graphics.BlendState.RenderTargets[i].SrcAlphaBlend);
				desc.RenderTarget[i].DestBlendAlpha = ReToDX11Blend(m_Desc.Graphics.BlendState.RenderTargets[i].DstAlphaBlend);
				desc.RenderTarget[i].BlendOpAlpha = ReToDX11BlendOp(m_Desc.Graphics.BlendState.RenderTargets[i].AlphaBlendOperation);

				uint8 renderTargetWriteMask = 0;
				if (m_Desc.Graphics.BlendState.RenderTargets[i].WriteMask == COLOR_WRITE_ENABLE_ALL)
				{
					renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				}
				else
				{
					if (m_Desc.Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
					if (m_Desc.Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
					if (m_Desc.Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
					if (m_Desc.Graphics.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
				}

				desc.RenderTarget[i].RenderTargetWriteMask = renderTargetWriteMask;
			}

			m_BlendFactor[0] = m_Desc.Graphics.BlendState.BlendFactor[0];
			m_BlendFactor[1] = m_Desc.Graphics.BlendState.BlendFactor[1];
			m_BlendFactor[2] = m_Desc.Graphics.BlendState.BlendFactor[2];
			m_BlendFactor[3] = m_Desc.Graphics.BlendState.BlendFactor[3];
			m_SampleMask = m_Desc.Graphics.SampleMask;


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
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