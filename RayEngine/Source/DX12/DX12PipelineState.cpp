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
#include "..\..\Include\DX12\DX12PipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Shader.h"
#include "..\..\Include\DX12\DX12RootLayout.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(IDevice* pDevice, const PipelineStateInfo& info)
			: m_Device(nullptr),
			m_PipelineState(nullptr),
			m_Type(PIPELINE_TYPE_UNKNOWN)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState::~DX12PipelineState()
		{
			D3DRelease_S(m_PipelineState);
			
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		PIPELINE_TYPE DX12PipelineState::GetPipelineType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetName(const std::string& name)
		{
			D3D12SetName(m_PipelineState, name);
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::QueryDevice(IDevice ** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D12PipelineState* DX12PipelineState::GetD3D12PipelineState() const
		{
			return m_PipelineState;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::Create(const PipelineStateInfo& info)
		{
			if (info.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState(info);
			else if (info.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState(info);


			if (m_PipelineState != nullptr)
				D3D12SetName(m_PipelineState, info.Name);
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateGraphicsState(const PipelineStateInfo& info)
		{
			using namespace System;

			std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(info.GraphicsPipeline.InputLayout.ElementCount);

			for (int32 i = 0; i < inputLayout.size(); i++)
				SetInputElementDesc(inputLayout[i], info.GraphicsPipeline.InputLayout.pElements[i]);


			ID3D12RootSignature* pD3D12RootSignature = reinterpret_cast<DX12RootLayout*>(info.pRootLayout)->GetD3D12RootSignature();
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
			desc.NumRenderTargets = info.GraphicsPipeline.RenderTargetCount;
			for (uint32 i = 0; i < desc.NumRenderTargets; i++)
				desc.RTVFormats[i] = ReToDXFormat(info.GraphicsPipeline.RenderTargetFormats[i]);
			

			desc.DSVFormat = ReToDXFormat(info.GraphicsPipeline.DepthStencilFormat);
			desc.SampleDesc.Count = info.GraphicsPipeline.SampleCount;


			D3D_PRIMITIVE_TOPOLOGY topology = ReToDXTopology(info.GraphicsPipeline.Topology);
			if (topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST || topology == D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			else if (topology == D3D_PRIMITIVE_TOPOLOGY_LINELIST || topology == D3D_PRIMITIVE_TOPOLOGY_LINESTRIP)
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
			else if (topology == D3D_PRIMITIVE_TOPOLOGY_POINTLIST)
				desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;


			if (info.GraphicsPipeline.StripCutEnable)
				desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF;
			else
				desc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
			

			desc.CachedPSO.CachedBlobSizeInBytes = 0;
			desc.CachedPSO.pCachedBlob = 0;


			desc.InputLayout.pInputElementDescs = inputLayout.data();
			desc.InputLayout.NumElements = static_cast<uint32>(inputLayout.size());


			//TODO: Make sure shader is of correct type
			SetShaderByteCode(desc.VS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pVertexShader));
			SetShaderByteCode(desc.HS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pHullShader));
			SetShaderByteCode(desc.DS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pDomainShader));
			SetShaderByteCode(desc.GS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pGeometryShader));
			SetShaderByteCode(desc.PS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pPixelShader));
		

			SetRasterizerDesc(desc.RasterizerState, info.GraphicsPipeline.RasterizerState);

			SetDepthStencilDesc(desc.DepthStencilState, info.GraphicsPipeline.DepthStencilState);

			SetBlendDesc(desc.BlendState, info.GraphicsPipeline.BlendState);
			desc.SampleMask = info.GraphicsPipeline.SampleMask;
	

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&m_PipelineState));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create PipelineState. " + DXErrorString(hr));
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateComputeState(const PipelineStateInfo& info)
		{
			using namespace System;

			ID3D12RootSignature* pD3D12RootSignature = reinterpret_cast<DX12RootLayout*>(info.pRootLayout)->GetD3D12RootSignature();
			D3D12_COMPUTE_PIPELINE_STATE_DESC desc = { };
			desc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
			desc.pRootSignature = pD3D12RootSignature;
			desc.NodeMask = 0;

			SetShaderByteCode(desc.CS, reinterpret_cast<DX12Shader*>(info.ComputePipeline.pComputeShader));

			desc.CachedPSO.CachedBlobSizeInBytes = 0;
			desc.CachedPSO.pCachedBlob = nullptr;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			HRESULT hr = pD3D12Device->CreateComputePipelineState(&desc, IID_PPV_ARGS(&m_PipelineState));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create PipelineState. " + DXErrorString(hr));
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetShaderByteCode(D3D12_SHADER_BYTECODE& byteCode, const DX12Shader* shader)
		{
			if (shader == nullptr)
				return;

			byteCode.BytecodeLength = shader->GetD3D12ByteCode().BytecodeLength;
			byteCode.pShaderBytecode = shader->GetD3D12ByteCode().pShaderBytecode;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetInputElementDesc(D3D12_INPUT_ELEMENT_DESC& desc, const InputElementInfo& element)
		{
			desc.SemanticName = element.Semantic.c_str();
			desc.SemanticIndex = element.SemanticIndex;

			desc.Format = ReToDXFormat(element.Format);

			desc.InputSlot = element.InputSlot;

			desc.AlignedByteOffset = element.ElementOffset;
			
			if (element.StepType == ELEMENT_STEP_TYPE_VERTEX)
				desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
			else if (element.StepType == ELEMENT_STEP_TYPE_INSTANCE)
				desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;

			desc.InstanceDataStepRate = element.DataStepRate;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetRasterizerDesc(D3D12_RASTERIZER_DESC& desc, const RasterizerStateInfo& info)
		{
			desc.ConservativeRaster = info.ConservativeRasterizerEnable ? 
				D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON : D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			desc.AntialiasedLineEnable = info.AntialiasedLineEnable;
			desc.FrontCounterClockwise = false;
			desc.MultisampleEnable = false;
			desc.ForcedSampleCount = 0;
		
			if (info.CullMode == CULL_MODE_BACK)
				desc.CullMode = D3D12_CULL_MODE_BACK;
			else if (info.CullMode == CULL_MODE_FRONT)
				desc.CullMode = D3D12_CULL_MODE_FRONT;
			else if (info.CullMode == CULL_MODE_NONE)
				desc.CullMode = D3D12_CULL_MODE_NONE;


			desc.DepthBias = info.DepthBias;
			desc.DepthBiasClamp = info.DepthBiasClamp;
			desc.DepthClipEnable = info.DepthClipEnable;

			desc.FillMode = (info.FillMode == FILL_MODE_SOLID) ? D3D12_FILL_MODE_SOLID : D3D12_FILL_MODE_WIREFRAME;
			desc.SlopeScaledDepthBias = info.SlopeScaleDepthBias;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC& desc, const DepthStencilStateInfo& info)
		{
			desc.DepthFunc = ReToDX12ComparisonFunc(info.DepthFunc);
			desc.DepthEnable = info.DepthEnable;
			desc.StencilEnable = info.StencilEnable;

			if (info.DepthWriteMask == DEPTH_WRITE_MASK_ALL)
				desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			else if (info.DepthWriteMask == DEPTH_WRITE_MASK_ZERO)
				desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;

			desc.StencilReadMask = info.StencilReadMask;
			desc.StencilWriteMask = info.StencilWriteMask;

			desc.BackFace = ReToDX12StencilOpDesc(info.BackFace);
			desc.FrontFace = ReToDX12StencilOpDesc(info.BackFace);
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetBlendDesc(D3D12_BLEND_DESC& desc, const BlendStateInfo& info)
		{
			desc.AlphaToCoverageEnable = info.AlphaToCoverageEnable;
			desc.IndependentBlendEnable = info.IndependentBlendEnable;

			for (int32 i = 0; i < 8; i++)
			{
				desc.RenderTarget[i].LogicOpEnable = info.LogicOpEnable;
				desc.RenderTarget[i].BlendEnable = info.RenderTargets[i].BlendEnable;
				desc.RenderTarget[i].SrcBlend = ReToDX12Blend(info.RenderTargets[i].SrcBlend);
				desc.RenderTarget[i].DestBlend = ReToDX12Blend(info.RenderTargets[i].DstBlend);
				desc.RenderTarget[i].BlendOp = ReToDX12BlendOp(info.RenderTargets[i].BlendOperation);
				desc.RenderTarget[i].SrcBlendAlpha = ReToDX12Blend(info.RenderTargets[i].SrcAlphaBlend);
				desc.RenderTarget[i].DestBlendAlpha = ReToDX12Blend(info.RenderTargets[i].DstAlphaBlend);
				desc.RenderTarget[i].BlendOpAlpha = ReToDX12BlendOp(info.RenderTargets[i].AlphaBlendOperation);
				desc.RenderTarget[i].LogicOp = D3D12_LOGIC_OP_NOOP;

				if (info.RenderTargets[i].RenderTargetWriteMask == COLOR_WRITE_ENABLE_ALL)
				{
					desc.RenderTarget[i].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
				}
				else
				{
					if (info.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_RED)
						desc.RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_RED;
					if (info.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_GREEN)
						desc.RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_GREEN;
					if (info.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_BLUE)
						desc.RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_BLUE;
					if (info.RenderTargets[i].RenderTargetWriteMask & COLOR_WRITE_ENABLE_ALPHA)
						desc.RenderTarget[i].RenderTargetWriteMask |= D3D12_COLOR_WRITE_ENABLE_ALPHA;
				}
			}
		}
	}
}

#endif