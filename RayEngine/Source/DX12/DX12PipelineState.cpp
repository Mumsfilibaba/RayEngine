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
		DX12PipelineState::DX12PipelineState(DX12PipelineState&& other)
			: m_Device(other.m_Device),
			m_PipelineState(other.m_PipelineState),
			m_Type(other.m_Type),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_PipelineState = nullptr;
			other.m_Type = PIPELINE_TYPE_UNKNOWN;
			other.m_ReferenceCount = 0;
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
		DX12PipelineState& DX12PipelineState::operator=(DX12PipelineState&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_PipelineState);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_PipelineState = other.m_PipelineState;
				m_Type = other.m_Type;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_PipelineState = nullptr;
				other.m_Type = PIPELINE_TYPE_UNKNOWN;
				other.m_ReferenceCount = 0;
			}

			return *this;
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
			//TODO: Actually set the rasterizerdesc
			desc.AntialiasedLineEnable = false;
			desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			desc.CullMode = D3D12_CULL_MODE_BACK;
			desc.DepthBias = 0;
			desc.DepthBiasClamp = 0.0f;
			desc.DepthClipEnable = true;
			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.ForcedSampleCount = 0;
			desc.FrontCounterClockwise = false;
			desc.MultisampleEnable = false;
			desc.SlopeScaledDepthBias = 0.0f;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetDepthStencilDesc(D3D12_DEPTH_STENCIL_DESC& desc, const DepthStencilStateInfo& info)
		{
			//TODO: Actually set the rasterizerdesc
			desc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			desc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			desc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
			desc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
			desc.FrontFace = desc.BackFace;
			desc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			desc.DepthEnable = true;
			desc.StencilEnable = false;
			desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			desc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D12_DEFAULT_STENCIL_READ_MASK;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::SetBlendDesc(D3D12_BLEND_DESC& desc, const BlendStateInfo& info)
		{
			//TODO: Actually set the blenddesc
			desc.AlphaToCoverageEnable = false;
			desc.IndependentBlendEnable = false;

			desc.RenderTarget[0].BlendEnable = false;
			desc.RenderTarget[0].LogicOpEnable = false;
			desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			desc.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			desc.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}
	}
}

#endif