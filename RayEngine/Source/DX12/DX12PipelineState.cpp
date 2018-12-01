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
#include "../../Include/Debug/Debug.h"
#include "../../Include/DX12/DX12PipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX12/DX12Device.h"
#include "../../Include/DX12/DX12Shader.h"
#include "../../Include/DX12/DX12RootLayout.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(IDevice* pDevice, const PipelineStateDesc* pDesc)
			: m_Device(nullptr),
			m_PipelineState(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pDesc);
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12PipelineState::~DX12PipelineState()
		{
			D3DRelease_S(m_PipelineState);

			if (m_Desc.Type == PIPELINE_TYPE_GRAPHICS)
			{
				if (m_Desc.GraphicsPipeline.InputLayout.pElements != nullptr)
				{
					delete[] m_Desc.GraphicsPipeline.InputLayout.pElements;
					m_Desc.GraphicsPipeline.InputLayout.pElements = nullptr;

					m_Desc.GraphicsPipeline.InputLayout.ElementCount = 0;
				}
			}
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::SetName(const std::string& name)
		{
			D3D12SetName(m_PipelineState, name);
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::QueryDevice(IDevice ** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::GetDesc(PipelineStateDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12PipelineState::GetReferenceCount() const
		{
			return m_References;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12PipelineState::AddRef()
		{
			m_References++;
			return m_References;
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12PipelineState::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::Create(const PipelineStateDesc* pDesc)
		{
			CopyPipelineStateDesc(&m_Desc, pDesc);

			if (m_Desc.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState();
			else if (m_Desc.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState();


			if (m_PipelineState != nullptr)
				D3D12SetName(m_PipelineState, m_Desc.Name);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateGraphicsState()
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(m_Desc.GraphicsPipeline.InputLayout.ElementCount);

			for (int32 i = 0; i < static_cast<int32>(inputLayout.size()); i++)
			{
				SetInputElementDesc(&inputLayout[i], &m_Desc.GraphicsPipeline.InputLayout.pElements[i]);
			}


			ID3D12RootSignature* pD3D12RootSignature = reinterpret_cast<DX12RootLayout*>(m_Desc.pRootLayout)->GetD3D12RootSignature();
			D3D12_GRAPHICS_PIPELINE_STATE_DESC desc = {};
			desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
			desc.pRootSignature = pD3D12RootSignature;
			desc.NodeMask = 0;

			//TODO: Fill in streamoutput
			//desc.StreamOutput.NumEntries;
			//desc.StreamOutput.NumStrides;
			//desc.StreamOutput.pBufferStrides;
			//desc.StreamOutput.pSODeclaration;
			//desc.StreamOutput.RasterizedStream;

			//TODO: Fix rendertarget count etc
			desc.NumRenderTargets = m_Desc.GraphicsPipeline.RenderTargetCount;
			for (uint32 i = 0; i < desc.NumRenderTargets; i++)
				desc.RTVFormats[i] = ReToDXFormat(m_Desc.GraphicsPipeline.RenderTargetFormats[i]);
			

			desc.DSVFormat = ReToDXFormat(m_Desc.GraphicsPipeline.DepthStencilFormat);
			desc.SampleDesc.Count = m_Desc.GraphicsPipeline.SampleCount;


			D3D_PRIMITIVE_TOPOLOGY topology = ReToDXTopology(m_Desc.GraphicsPipeline.Topology);
			if (topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST || topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			else if (topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST || topology == D3D_PRIMITIVE_TOPOLOGY_LINESTRIP)
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			else if (topology == D3D_PRIMITIVE_TOPOLOGY_POINTLIST)
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

			desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

			desc.CachedPSO.CachedBlobSizeInBytes = 0;
			desc.CachedPSO.pCachedBlob = 0;


			desc.InputLayout.pInputElementDescs = inputLayout.data();
			desc.InputLayout.NumElements = static_cast<uint32>(inputLayout.size());


			//TODO: Make sure shader is of correct type
			SetShaderByteCode(&desc.VS, reinterpret_cast<DX12Shader*>(m_Desc.GraphicsPipeline.pVertexShader));
			SetShaderByteCode(&desc.HS, reinterpret_cast<DX12Shader*>(m_Desc.GraphicsPipeline.pHullShader));
			SetShaderByteCode(&desc.DS, reinterpret_cast<DX12Shader*>(m_Desc.GraphicsPipeline.pDomainShader));
			SetShaderByteCode(&desc.GS, reinterpret_cast<DX12Shader*>(m_Desc.GraphicsPipeline.pGeometryShader));
			SetShaderByteCode(&desc.PS, reinterpret_cast<DX12Shader*>(m_Desc.GraphicsPipeline.pPixelShader));
		

			SetRasterizerDesc(&desc.RasterizerState, &m_Desc.GraphicsPipeline.RasterizerState);

			SetDepthStencilDesc(&desc.DepthStencilState, &m_Desc.GraphicsPipeline.DepthStencilState);

			SetBlendDesc(&desc.BlendState, &m_Desc.GraphicsPipeline.BlendState);
			desc.SampleMask = m_Desc.GraphicsPipeline.SampleMask;
	

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_PipelineState));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create PipelineState. " + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateComputeState()
		{
			ID3D12RootSignature* pD3D12RootSignature = reinterpret_cast<DX12RootLayout*>(m_Desc.pRootLayout)->GetD3D12RootSignature();
			D3D12_COMPUTE_PIPELINE_STATE_DESC desc = { };
			desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
			desc.pRootSignature = pD3D12RootSignature;
			desc.NodeMask = 0;

			SetShaderByteCode(&desc.CS, reinterpret_cast<DX12Shader*>(m_Desc.ComputePipeline.pComputeShader));

			desc.CachedPSO.CachedBlobSizeInBytes = 0;
			desc.CachedPSO.pCachedBlob = nullptr;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&m_PipelineState));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create PipelineState. " + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::SetShaderByteCode(D3D12_SHADER_BYTECODE* pByteCode, const DX12Shader* pShader)
		{
			if (pShader == nullptr)
				return;

			pByteCode->BytecodeLength = pShader->GetD3D12ByteCode().BytecodeLength;
			pByteCode->pShaderBytecode = pShader->GetD3D12ByteCode().pShaderBytecode;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::SetInputElementDesc(D3D12_INPUT_ELEMENT_DESC* pD3D12Desc, const InputElementDesc* pDesc)
		{
			pD3D12Desc->SemanticName = pDesc->Semantic.c_str();
			pD3D12Desc->SemanticIndex = pDesc->SemanticIndex;

			pD3D12Desc->Format = ReToDXFormat(pDesc->Format);

			pD3D12Desc->InputSlot = pDesc->InputSlot;

			pD3D12Desc->AlignedByteOffset = pDesc->ElementOffset;
			
			if (pDesc->StepType == ELEMENT_STEP_TYPE_VERTEX)
				pD3D12Desc->InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			else if (pDesc->StepType == ELEMENT_STEP_TYPE_INSTANCE)
				pD3D12Desc->InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;

			pD3D12Desc->InstanceDataStepRate = pDesc->DataStepRate;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::SetRasterizerDesc(D3D12_RASTERIZER_DESC* pD3D12Desc, const RasterizerStateDesc* pDesc)
		{
			pD3D12Desc->ConservativeRaster = pDesc->ConservativeRasterizerEnable ? 
				D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			pD3D12Desc->AntialiasedLineEnable = pDesc->AntialiasedLineEnable;
			pD3D12Desc->FrontCounterClockwise = false;
			pD3D12Desc->MultisampleEnable = false;
			pD3D12Desc->ForcedSampleCount = 0;
		
			if (pDesc->CullMode == CULL_MODE_BACK)
				pD3D12Desc->CullMode = D3D12_CULL_MODE_BACK;
			else if (pDesc->CullMode == CULL_MODE_FRONT)
				pD3D12Desc->CullMode = D3D12_CULL_MODE_FRONT;
			else if (pDesc->CullMode == CULL_MODE_NONE)
				pD3D12Desc->CullMode = D3D12_CULL_MODE_NONE;


			pD3D12Desc->DepthBias = pDesc->DepthBias;
			pD3D12Desc->DepthBiasClamp = pDesc->DepthBiasClamp;
			pD3D12Desc->DepthClipEnable = pDesc->DepthClipEnable;

			pD3D12Desc->FillMode = (pDesc->FillMode == FILL_MODE_SOLID) ? D3D12_FILL_MODE_SOLID : D3D12_FILL_MODE_WIREFRAME;
			pD3D12Desc->SlopeScaledDepthBias = pDesc->SlopeScaleDepthBias;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC* pD3D12Desc, const DepthStencilStateDesc* pDesc)
		{
			pD3D12Desc->DepthFunc = ReToDX12ComparisonFunc(pDesc->DepthFunc);
			pD3D12Desc->DepthEnable = pDesc->DepthEnable;
			pD3D12Desc->StencilEnable = pDesc->StencilEnable;

			if (pDesc->DepthWriteMask == DEPTH_WRITE_MASK_ALL)
				pD3D12Desc->DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			else if (pDesc->DepthWriteMask == DEPTH_WRITE_MASK_ZERO)
				pD3D12Desc->DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

			pD3D12Desc->StencilReadMask = pDesc->StencilReadMask;
			pD3D12Desc->StencilWriteMask = pDesc->StencilWriteMask;

			pD3D12Desc->BackFace = ReToDX12StencilOpDesc(pDesc->BackFace);
			pD3D12Desc->FrontFace = ReToDX12StencilOpDesc(pDesc->BackFace);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12PipelineState::SetBlendDesc(D3D12_BLEND_DESC* pD3D12Desc, const BlendStateDesc* pDesc)
		{
			pD3D12Desc->AlphaToCoverageEnable = pDesc->AlphaToCoverageEnable;
			pD3D12Desc->IndependentBlendEnable = pDesc->IndependentBlendEnable;

			for (int32 i = 0; i < 8; i++)
			{
				pD3D12Desc->RenderTarget[i].LogicOpEnable = pDesc->LogicOpEnable;
				pD3D12Desc->RenderTarget[i].BlendEnable = pDesc->RenderTargets[i].BlendEnable;
				pD3D12Desc->RenderTarget[i].SrcBlend = ReToDX12Blend(pDesc->RenderTargets[i].SrcBlend);
				pD3D12Desc->RenderTarget[i].DestBlend = ReToDX12Blend(pDesc->RenderTargets[i].DstBlend);
				pD3D12Desc->RenderTarget[i].BlendOp = ReToDX12BlendOp(pDesc->RenderTargets[i].BlendOperation);
				pD3D12Desc->RenderTarget[i].SrcBlendAlpha = ReToDX12Blend(pDesc->RenderTargets[i].SrcAlphaBlend);
				pD3D12Desc->RenderTarget[i].DestBlendAlpha = ReToDX12Blend(pDesc->RenderTargets[i].DstAlphaBlend);
				pD3D12Desc->RenderTarget[i].BlendOpAlpha = ReToDX12BlendOp(pDesc->RenderTargets[i].AlphaBlendOperation);
				pD3D12Desc->RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;

				if (pDesc->RenderTargets[i].WriteMask == COLOR_WRITE_ENABLE_ALL)
				{
					pD3D12Desc->RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				}
				else
				{
					if (pDesc->RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_RED)
						pD3D12Desc->RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_RED;
					if (pDesc->RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_GREEN)
						pD3D12Desc->RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_GREEN;
					if (pDesc->RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_BLUE)
						pD3D12Desc->RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_BLUE;
					if (pDesc->RenderTargets[i].WriteMask & COLOR_WRITE_ENABLE_ALPHA)
						pD3D12Desc->RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
				}
			}
		}
	}
}

#endif