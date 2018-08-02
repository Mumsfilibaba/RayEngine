#include <vector>
#include "..\..\Include\DX12\DX12RootSignature.h"
#include "..\..\Include\DX12\DX12PipelineState.h"
#include "..\..\Include\DX12\DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(IDevice* pDevice, const PipelineStateInfo& info)
			: m_Device(nullptr),
			m_PipelineState(nullptr),
			m_Type(PIPELINE_TYPE_UNKNOWN),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState::~DX12PipelineState()
		{
			D3DRelease_S(m_PipelineState);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		PIPELINE_TYPE DX12PipelineState::GetPipelineType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12PipelineState::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		ID3D12PipelineState* DX12PipelineState::GetD3D12PipelineState() const
		{
			return m_PipelineState;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12PipelineState::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12PipelineState::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12PipelineState::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::Create(IDevice* pDevice, const PipelineStateInfo& info)
		{
			const DX12RootSignature* rootSignature = reinterpret_cast<const DX12RootSignature*>(info.pRootSignature);

			if (info.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState(pDevice, rootSignature->GetD3D12RootSignature(), info);
			else if (info.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState(pDevice, rootSignature->GetD3D12RootSignature(), info);

			if (m_PipelineState != nullptr)
				D3D12SetName(m_PipelineState, info.Name);
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateGraphicsState(IDevice* pDevice, ID3D12RootSignature* pRootSignature, const PipelineStateInfo& info)
		{
			std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(info.GraphicsPipeline.InputLayout.ElementCount);

			for (int32 i = 0; i < inputLayout.size(); i++)
				SetInputElementDesc(inputLayout[i], info.GraphicsPipeline.InputLayout.pElements[i]);

			D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc;
			memset(&pDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
			
			//TODO: Fix rendertarget count etc
			pDesc.NumRenderTargets = 1;
			pDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			pDesc.DSVFormat = DXGI_FORMAT_D16_UNORM;
			pDesc.SampleDesc.Count = 1;

			pDesc.SampleMask = UINT_MAX;
			pDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			
			pDesc.pRootSignature = pRootSignature;

			pDesc.InputLayout.pInputElementDescs = inputLayout.data();
			pDesc.InputLayout.NumElements = static_cast<uint32>(inputLayout.size());

			//TODO: Make sure shader is of correct type
			SetShaderByteCode(pDesc.VS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pVertexShader));
			SetShaderByteCode(pDesc.HS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pHullShader));
			SetShaderByteCode(pDesc.DS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pDomainShader));
			SetShaderByteCode(pDesc.GS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pGeometryShader));
			SetShaderByteCode(pDesc.PS, reinterpret_cast<DX12Shader*>(info.GraphicsPipeline.pPixelShader));
			
			SetRasterizerDesc(pDesc.RasterizerState, info.GraphicsPipeline.RasterizerState);

			SetDepthStencilDesc(pDesc.DepthStencilState, info.GraphicsPipeline.DepthStencilState);

			SetBlendDesc(pDesc.BlendState, info.GraphicsPipeline.BlendState);
	

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			if (FAILED(pD3D12Device->CreateGraphicsPipelineState(&pDesc, IID_PPV_ARGS(&m_PipelineState))))
				return;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateComputeState(IDevice* pDevice, ID3D12RootSignature* pRootSignature, const PipelineStateInfo& info)
		{
			//TODO: Compute states
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetShaderByteCode(D3D12_SHADER_BYTECODE& byteCode, const DX12Shader* shader)
		{
			if (shader == nullptr)
				return;

			byteCode.BytecodeLength = shader->GetD3D12ByteCode()->BytecodeLength;
			byteCode.pShaderBytecode = shader->GetD3D12ByteCode()->pShaderBytecode;
			return;
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
				desc.RenderTarget[i].BlendEnable = info.RenderTargets[i].BlendEnable;
				desc.RenderTarget[i].LogicOpEnable = info.RenderTargets[i].LogicOpEnable;
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