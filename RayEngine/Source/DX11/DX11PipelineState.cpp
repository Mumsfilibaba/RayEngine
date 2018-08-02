#include <vector>
#include "..\..\Include\DX11\DX11PipelineState.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

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
			m_VertexShader(nullptr),
			m_HullShader(nullptr),
			m_DomainShader(nullptr),
			m_GeometryShader(nullptr),
			m_PixelShader(nullptr),
			m_ComputeShader(nullptr),
			m_SampleMask(0),
			m_Type(PIPELINE_TYPE_UNKNOWN),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11PipelineState::DX11PipelineState(DX11PipelineState&& other)
			: m_Device(other.m_Device),
			m_InputLayout(other.m_InputLayout),
			m_BlendState(other.m_BlendState),
			m_RasterizerState(other.m_RasterizerState),
			m_DepthStencilState(other.m_DepthStencilState),
			m_VertexShader(other.m_VertexShader),
			m_HullShader(other.m_HullShader),
			m_DomainShader(other.m_DomainShader),
			m_GeometryShader(other.m_GeometryShader),
			m_PixelShader(other.m_PixelShader),
			m_ComputeShader(other.m_ComputeShader),
			m_SampleMask(other.m_SampleMask),
			m_Type(other.m_Type),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_InputLayout = nullptr;
			other.m_BlendState = nullptr;
			other.m_RasterizerState = nullptr;
			other.m_DepthStencilState = nullptr;
			other.m_VertexShader = nullptr;
			other.m_HullShader = nullptr;
			other.m_DomainShader = nullptr;
			other.m_GeometryShader = nullptr;
			other.m_PixelShader = nullptr;
			other.m_ComputeShader = nullptr;
			other.m_SampleMask = 0;
			other.m_Type = PIPELINE_TYPE_UNKNOWN;
			other.m_ReferenceCount = 0;
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
			return (m_VertexShader == nullptr) ? nullptr : m_VertexShader->GetD3D11VertexShader();
		}



		/////////////////////////////////////////////////////////////
		ID3D11HullShader* DX11PipelineState::GetD3D11HullShader() const
		{
			return (m_HullShader == nullptr) ? nullptr : m_HullShader->GetD3D11HullShader();
		}



		/////////////////////////////////////////////////////////////
		ID3D11DomainShader* DX11PipelineState::GetD3D11DomainShader() const
		{
			return (m_DomainShader == nullptr) ? nullptr : m_DomainShader->GetD3D11DomainShader();
		}



		/////////////////////////////////////////////////////////////
		ID3D11GeometryShader* DX11PipelineState::GetD3D11GeometryShader() const
		{
			return (m_GeometryShader == nullptr) ? nullptr : m_GeometryShader->GetD3D11GeometryShader();
		}



		/////////////////////////////////////////////////////////////
		ID3D11PixelShader* DX11PipelineState::GetD3D11PixelShader() const
		{
			return (m_PixelShader == nullptr) ? nullptr : m_PixelShader->GetD3D11PixelShader();
		}



		/////////////////////////////////////////////////////////////
		ID3D11ComputeShader* DX11PipelineState::GetD3D11ComputeShader() const
		{
			return (m_ComputeShader == nullptr) ? nullptr : m_ComputeShader->GetD3D11ComputeShader();
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11VertexShader() const
		{
			return m_VertexShader;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11HullShader() const
		{
			return m_HullShader;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11DomainShader() const
		{
			return m_DomainShader;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11GeometryShader() const
		{
			return m_GeometryShader;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11PixelShader() const
		{
			return m_PixelShader;
		}



		/////////////////////////////////////////////////////////////
		DX11Shader* DX11PipelineState::GetDX11ComputeShader() const
		{
			return m_ComputeShader;
		}



		/////////////////////////////////////////////////////////////
		PIPELINE_TYPE DX11PipelineState::GetPipelineType() const
		{
			return m_Type;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11PipelineState::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		DX11PipelineState& DX11PipelineState::operator=(DX11PipelineState&& other)
		{
			if (this != &other)
			{
				ReleaseInterfaces();


				m_Device = other.m_Device;
				m_InputLayout = other.m_InputLayout;
				m_BlendState = other.m_BlendState;
				
				memcpy(m_BlendFactor, other.m_BlendFactor, sizeof(float[4]));
				
				m_RasterizerState = other.m_RasterizerState;
				m_DepthStencilState = other.m_DepthStencilState;
				m_VertexShader = other.m_VertexShader;
				m_HullShader = other.m_HullShader;
				m_DomainShader = other.m_DomainShader;
				m_GeometryShader = other.m_GeometryShader;
				m_PixelShader = other.m_PixelShader;
				m_ComputeShader = other.m_ComputeShader;
				m_SampleMask = other.m_SampleMask;
				m_Type = other.m_Type;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_InputLayout = nullptr;
				other.m_BlendState = nullptr;
				other.m_RasterizerState = nullptr;
				other.m_DepthStencilState = nullptr;
				other.m_VertexShader = nullptr;
				other.m_HullShader = nullptr;
				other.m_DomainShader = nullptr;
				other.m_GeometryShader = nullptr;
				other.m_PixelShader = nullptr;
				other.m_ComputeShader = nullptr;
				other.m_SampleMask = 0;
				other.m_Type = PIPELINE_TYPE_UNKNOWN;
				other.m_ReferenceCount = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11PipelineState::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11PipelineState::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11PipelineState::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::ReleaseInterfaces()
		{
			D3DRelease_S(m_InputLayout);
			D3DRelease_S(m_BlendState);
			D3DRelease_S(m_DepthStencilState);
			D3DRelease_S(m_RasterizerState);

			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}

			if (m_VertexShader != nullptr)
			{
				m_VertexShader->Release();
				m_VertexShader = nullptr;
			}

			if (m_HullShader != nullptr)
			{
				m_HullShader->Release();
				m_HullShader = nullptr;
			}

			if (m_DomainShader != nullptr)
			{
				m_DomainShader->Release();
				m_DomainShader = nullptr;
			}

			if (m_GeometryShader != nullptr)
			{
				m_GeometryShader->Release();
				m_GeometryShader = nullptr;
			}

			if (m_PixelShader != nullptr)
			{
				m_PixelShader->Release();
				m_PixelShader = nullptr;
			}

			if (m_ComputeShader != nullptr)
			{
				m_ComputeShader->Release();
				m_ComputeShader = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::Create(IDevice* pDevice, const PipelineStateInfo& info)
		{
			m_Type = info.Type;
			if (info.Type == PIPELINE_TYPE_GRAPHICS)
				CreateGraphicsState(pDevice, info);
			else if (info.Type == PIPELINE_TYPE_COMPUTE)
				CreateComputeState(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateGraphicsState(IDevice* pDevice, const PipelineStateInfo& info)
		{
			if (info.GraphicsPipeline.pVertexShader != nullptr)
				m_VertexShader = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pVertexShader->QueryReference());
			if (info.GraphicsPipeline.pHullShader != nullptr)
				m_HullShader = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pHullShader->QueryReference());
			if (info.GraphicsPipeline.pDomainShader != nullptr)
				m_DomainShader = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pDomainShader->QueryReference());
			if (info.GraphicsPipeline.pGeometryShader != nullptr)
				m_GeometryShader = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pGeometryShader->QueryReference());
			if (info.GraphicsPipeline.pPixelShader != nullptr)
				m_PixelShader = reinterpret_cast<DX11Shader*>(info.GraphicsPipeline.pPixelShader->QueryReference());


			CreateInputLayout(pDevice, info);
			CreateBlendState(pDevice, info);
			CreateRasterizerState(pDevice, info);
			CreateDepthStencilState(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateComputeState(IDevice* pDevice, const PipelineStateInfo& info)
		{
			//TODO: Compute states
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateInputLayout(IDevice* pDevice, const PipelineStateInfo& info)
		{
			using namespace System;

			if (info.GraphicsPipeline.InputLayout.ElementCount < 1)
				return;


			if (m_VertexShader == nullptr)
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Cannot create a inputlayout without a VertexShader.");
				return;
			}


			std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout;
			inputLayout.resize(info.GraphicsPipeline.InputLayout.ElementCount);

			for (int32 i = 0; i < info.GraphicsPipeline.InputLayout.ElementCount; i++)
				SetInputElementDesc(inputLayout[i], info.GraphicsPipeline.InputLayout.pElements[i]);

			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateInputLayout(inputLayout.data(), static_cast<uint32>(inputLayout.size()), 
				m_VertexShader->GetByteCode().GetBytes(), static_cast<size_t>(m_VertexShader->GetByteCode().GetSize()), &m_InputLayout);
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create InputLayout" + DXErrorString(hr));
				return;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateRasterizerState(IDevice* pDevice, const PipelineStateInfo& info)
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

			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateRasterizerState(&desc, &m_RasterizerState);
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create RasterizerState" + DXErrorString(hr));
				return;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateDepthStencilState(IDevice* pDevice, const PipelineStateInfo& info)
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


			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateDepthStencilState(&desc, &m_DepthStencilState);
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DepthStencilState" + DXErrorString(hr));
				return;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::CreateBlendState(IDevice* pDevice, const PipelineStateInfo& info)
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
			m_SampleMask = info.GraphicsPipeline.BlendState.SampleMask;


			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateBlendState(&desc, &m_BlendState);
			
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create BlendState" + DXErrorString(hr));
				return;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX11PipelineState::SetInputElementDesc(D3D11_INPUT_ELEMENT_DESC& desc, const InputElementInfo& info)
		{
			desc.AlignedByteOffset = static_cast<uint32>(info.ElementOffset);
			desc.Format = ReToDXFormat(info.Format);
			desc.InputSlot = static_cast<uint32>(info.InputSlot);
			desc.InputSlotClass = (info.StepType == ELEMENT_STEP_TYPE_VERTEX) 
				? D3D11_INPUT_PER_VERTEX_DATA : D3D11_INPUT_PER_INSTANCE_DATA;
			desc.InstanceDataStepRate = static_cast<uint32>(info.DataStepRate);
			desc.SemanticName = info.Semantic.c_str();
			desc.SemanticIndex = info.SemanticIndex;
		}
	}
}

#endif