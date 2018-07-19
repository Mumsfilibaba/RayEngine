#include "..\..\Include\DX12\DX12RootSignature.h"
#include "..\..\Include\DX12\DX12PipelineState.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(ID3D12Device* device, const PipelineStateInfo& info)
			: m_PipelineState(nullptr)
		{
			Create(device, info);
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState::DX12PipelineState(DX12PipelineState&& other)
			: m_PipelineState(other.m_PipelineState)
		{
			other.m_PipelineState = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState::~DX12PipelineState()
		{
			D3DRelease_S(m_PipelineState);
		}



		/////////////////////////////////////////////////////////////
		DX12PipelineState& DX12PipelineState::operator=(DX12PipelineState&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_PipelineState);

				m_PipelineState = other.m_PipelineState;
				other.m_PipelineState = nullptr;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::Create(ID3D12Device* device, const PipelineStateInfo& info)
		{
			const DX12RootSignature* rootSignature = reinterpret_cast<const DX12RootSignature*>(info.RootSignature);

			if (info.Type == PIPELINETYPE_GRAPHICS)
				CreateGraphicsState(device, rootSignature->GetRootSignature(), info);
			else if (info.Type == PIPELINETYPE_COMPUTE)
				CreateComputeState(device, rootSignature->GetRootSignature(), info);
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateGraphicsState(ID3D12Device* device, ID3D12RootSignature* rootSignature, const PipelineStateInfo& info)
		{
			D3D12_RASTERIZER_DESC rasterizerState = {};
			rasterizerState.AntialiasedLineEnable = false;
			rasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
			rasterizerState.CullMode = D3D12_CULL_MODE_BACK;
			rasterizerState.DepthBias = 0;
			rasterizerState.DepthBiasClamp = 0.0f;
			rasterizerState.DepthClipEnable = true;
			rasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
			rasterizerState.ForcedSampleCount = 0;
			rasterizerState.FrontCounterClockwise = false;
			rasterizerState.MultisampleEnable = false;
			rasterizerState.SlopeScaledDepthBias = 0.0f;

			D3D12_BLEND_DESC blendState = {};
			blendState.AlphaToCoverageEnable = false;
			blendState.IndependentBlendEnable = false;
			blendState.RenderTarget[0].BlendEnable = false;
			blendState.RenderTarget[0].LogicOpEnable = false;
			blendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			blendState.RenderTarget[0].SrcBlend = D3D12_BLEND_ONE;
			blendState.RenderTarget[0].DestBlend = D3D12_BLEND_ZERO;
			blendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			blendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
			blendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			blendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
			blendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

			D3D12_DEPTH_STENCIL_DESC depthStencilState = {};
			depthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			depthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			depthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
			depthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
			depthStencilState.FrontFace = depthStencilState.BackFace;
			depthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
			depthStencilState.DepthEnable = true;
			depthStencilState.StencilEnable = false;
			depthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			depthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
			depthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_READ_MASK;

			D3D12_GRAPHICS_PIPELINE_STATE_DESC pDesc;
			memset(&pDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
			pDesc.pRootSignature = rootSignature;
			pDesc.VS = vs;
			pDesc.PS = ps;
			pDesc.InputLayout = { iDescs, _countof(iDescs) };
			pDesc.RasterizerState = rasterizerState;
			pDesc.BlendState = blendState;
			pDesc.DepthStencilState = depthStencilState;
			pDesc.SampleMask = UINT_MAX;
			pDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
			pDesc.NumRenderTargets = 1;
			pDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
			pDesc.DSVFormat = DXGI_FORMAT_D16_UNORM;
			pDesc.SampleDesc.Count = 1;

			if (FAILED(device->CreateGraphicsPipelineState(&pDesc, IID_PPV_ARGS(&m_PipelineState))))
				return;
		}



		/////////////////////////////////////////////////////////////
		void DX12PipelineState::CreateComputeState(ID3D12Device* device, ID3D12RootSignature* rootSignature, const PipelineStateInfo& info)
		{
		}
	}
}