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
			m_SampleMask(0),
			m_Type(PIPELINE_TYPE_UNKNOWN),
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
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PIPELINE_TYPE DX11PipelineState::GetPipelineType() const
		{
			return m_Type;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::SetName(const std::string& name)
		{
			std::string tmpName = name + " : InputLayout";
			m_InputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(tmpName.size()), tmpName.c_str());

			tmpName = name + " : BlendState";
			m_BlendState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(tmpName.size()), tmpName.c_str());

			tmpName = name + " : RasterizerState";
			m_RasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(tmpName.size()), tmpName.c_str());

			tmpName = name + " : DepthStencilState";
			m_DepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(tmpName.size()), tmpName.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11PipelineState::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11PipelineState::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11PipelineState::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

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
			m_Type = pDesc->Type;
			if (pDesc->Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState(pDesc);
			else if (pDesc->Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState(pDesc);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateGraphicsState(const PipelineStateDesc* pDesc)
		{
			if (pDesc->GraphicsPipeline.pVertexShader != nullptr)
				m_VS = pDesc->GraphicsPipeline.pVertexShader->QueryReference<DX11Shader>();
			if (pDesc->GraphicsPipeline.pHullShader != nullptr)
				m_HS = pDesc->GraphicsPipeline.pHullShader->QueryReference<DX11Shader>();
			if (pDesc->GraphicsPipeline.pDomainShader != nullptr)
				m_DS = pDesc->GraphicsPipeline.pDomainShader->QueryReference<DX11Shader>();
			if (pDesc->GraphicsPipeline.pGeometryShader != nullptr)
				m_GS = pDesc->GraphicsPipeline.pGeometryShader->QueryReference<DX11Shader>();
			if (pDesc->GraphicsPipeline.pPixelShader != nullptr)
				m_PS = pDesc->GraphicsPipeline.pPixelShader->QueryReference<DX11Shader>();


			CreateInputLayout(pDesc);
			CreateBlendState(pDesc);
			CreateRasterizerState(pDesc);
			CreateDepthStencilState(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateComputeState(const PipelineStateDesc* pDesc)
		{
			if (pDesc->ComputePipeline.pComputeShader != nullptr)
				m_CS = pDesc->ComputePipeline.pComputeShader->QueryReference<DX11Shader>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateInputLayout(const PipelineStateDesc* pDesc)
		{
			using namespace System;

			if (pDesc->GraphicsPipeline.InputLayout.ElementCount < 1)
				return;


			if (m_VS == nullptr)
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Cannot create a inputlayout without a VertexShader.");
				return;
			}

			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(pDesc->GraphicsPipeline.InputLayout.ElementCount);

			for (int32 i = 0; i < pDesc->GraphicsPipeline.InputLayout.ElementCount; i++)
				SetInputElementDesc(&inputLayout[i], &pDesc->GraphicsPipeline.InputLayout.pElements[i]);

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
				std::string name = pDesc->Name + ": InputLayout";
				m_InputLayout->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateRasterizerState(const PipelineStateDesc* pDesc)
		{
			using namespace System;

			D3D11_RASTERIZER_DESC desc = {};
			desc.FillMode = (pDesc->GraphicsPipeline.RasterizerState.FillMode == FILL_MODE_SOLID) 
				? D3D11_FILL_SOLID : D3D11_FILL_WIREFRAME;

			if (pDesc->GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_BACK)
				desc.CullMode = D3D11_CULL_BACK;
			else if (pDesc->GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_FRONT)
				desc.CullMode = D3D11_CULL_FRONT;
			else if (pDesc->GraphicsPipeline.RasterizerState.CullMode == CULL_MODE_NONE)
				desc.CullMode = D3D11_CULL_NONE;

			desc.FrontCounterClockwise = pDesc->GraphicsPipeline.RasterizerState.FrontCounterClockwise;

			desc.DepthClipEnable = pDesc->GraphicsPipeline.RasterizerState.DepthClipEnable;
			desc.DepthBias = pDesc->GraphicsPipeline.RasterizerState.DepthBias;
			desc.DepthBiasClamp = pDesc->GraphicsPipeline.RasterizerState.DepthBiasClamp;
			desc.SlopeScaledDepthBias = pDesc->GraphicsPipeline.RasterizerState.SlopeScaleDepthBias;
			
			desc.ScissorEnable = pDesc->GraphicsPipeline.RasterizerState.ScissorEnable;
			desc.MultisampleEnable = pDesc->GraphicsPipeline.RasterizerState.MultisampleEnable;
			desc.AntialiasedLineEnable = pDesc->GraphicsPipeline.RasterizerState.AntialiasedLineEnable;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateRasterizerState(&desc, &m_RasterizerState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create RasterizerState." + DXErrorString(hr));
			}
			else
			{
				std::string name = pDesc->Name + ": RasterizerState";
				m_RasterizerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateDepthStencilState(const PipelineStateDesc* pDesc)
		{
			using namespace System;

			D3D11_DEPTH_STENCIL_DESC desc = {};
			desc.DepthEnable = pDesc->GraphicsPipeline.DepthStencilState.DepthEnable;
			desc.DepthFunc = ReToDX11ComparisonFunc(pDesc->GraphicsPipeline.DepthStencilState.DepthFunc);
			desc.DepthWriteMask = (pDesc->GraphicsPipeline.DepthStencilState.DepthWriteMask == DEPTH_WRITE_MASK_ALL) 
				? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;

			desc.StencilEnable = pDesc->GraphicsPipeline.DepthStencilState.StencilEnable;
			desc.StencilReadMask = pDesc->GraphicsPipeline.DepthStencilState.StencilReadMask;
			desc.StencilWriteMask = pDesc->GraphicsPipeline.DepthStencilState.StencilWriteMask;

			desc.BackFace = ReToDX11StencilOpDesc(pDesc->GraphicsPipeline.DepthStencilState.BackFace);
			desc.FrontFace = ReToDX11StencilOpDesc(pDesc->GraphicsPipeline.DepthStencilState.FrontFace);


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateDepthStencilState(&desc, &m_DepthStencilState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DepthStencilState" + DXErrorString(hr));
			}
			else
			{
				std::string name = pDesc->Name + ": DepthStencilState";
				m_DepthStencilState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateBlendState(const PipelineStateDesc* pDesc)
		{
			using namespace System;

			D3D11_BLEND_DESC desc = {};
			desc.AlphaToCoverageEnable = pDesc->GraphicsPipeline.BlendState.AlphaToCoverageEnable;
			desc.IndependentBlendEnable = pDesc->GraphicsPipeline.BlendState.IndependentBlendEnable;
			
			for (int32 i = 0; i < 8; i++)
			{
				desc.RenderTarget[i].BlendEnable = pDesc->GraphicsPipeline.BlendState.RenderTargets[i].BlendEnable;
				
				desc.RenderTarget[i].SrcBlend = ReToDX11Blend(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].SrcBlend);
				desc.RenderTarget[i].DestBlend = ReToDX11Blend(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].DstBlend);
				desc.RenderTarget[i].BlendOp = ReToDX11BlendOp(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].BlendOperation);
				
				desc.RenderTarget[i].SrcBlendAlpha = ReToDX11Blend(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].SrcAlphaBlend);
				desc.RenderTarget[i].DestBlendAlpha = ReToDX11Blend(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].DstAlphaBlend);
				desc.RenderTarget[i].BlendOpAlpha = ReToDX11BlendOp(pDesc->GraphicsPipeline.BlendState.RenderTargets[i].AlphaBlendOperation);

				uint8 renderTargetWriteMask = 0;
				if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask == COLOR_WRITE_ENABLE_ALL)
				{
					renderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				}
				else
				{
					if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_RED;
					if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_GREEN;
					if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_BLUE;
					if (pDesc->GraphicsPipeline.BlendState.RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
						renderTargetWriteMask |= D3D11_COLOR_WRITE_ENABLE_ALPHA;
				}

				desc.RenderTarget[i].RenderTargetWriteMask = renderTargetWriteMask;
			}

			m_BlendFactor[0] = pDesc->GraphicsPipeline.BlendState.BlendFactor[0];
			m_BlendFactor[1] = pDesc->GraphicsPipeline.BlendState.BlendFactor[1];
			m_BlendFactor[2] = pDesc->GraphicsPipeline.BlendState.BlendFactor[2];
			m_BlendFactor[3] = pDesc->GraphicsPipeline.BlendState.BlendFactor[3];
			m_SampleMask = pDesc->GraphicsPipeline.SampleMask;


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBlendState(&desc, &m_BlendState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create BlendState" + DXErrorString(hr));
			}
			else
			{
				std::string name = pDesc->Name + ": BlendState";
				m_BlendState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11PipelineState::SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC* pD3D11desc, const InputElementDesc* pDesc)
		{
			pD3D11desc->AlignedByteOffset = static_cast<uint32>(pDesc->ElementOffset);
			pD3D11desc->Format = ReToDXFormat(pDesc->Format);
			pD3D11desc->InputSlot = static_cast<uint32>(pDesc->InputSlot);
			pD3D11desc->InputSlotClass = (pDesc->StepType == ELEMENT_STEP_TYPE_VERTEX) ? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
			pD3D11desc->InstanceDataStepRate = static_cast<uint32>(pDesc->DataStepRate);
			pD3D11desc->SemanticName = pDesc->Semantic.c_str();
			pD3D11desc->SemanticIndex = pDesc->SemanticIndex;
		}
	}
}

#endif