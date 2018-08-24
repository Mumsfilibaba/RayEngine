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

#include "..\..\Include\Graphics\Viewport.h"
#include "..\..\Include\DX11\DX11DeviceContext.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11RenderTargetView.h"
#include "..\..\Include\DX11\DX11DepthStencilView.h"
#include "..\..\Include\DX11\DX11ShaderResourceView.h"
#include "..\..\Include\DX11\DX11UnorderedAccessView.h"
#include "..\..\Include\DX11\DX11PipelineState.h"
#include "..\..\Include\DX11\DX11Buffer.h"
#include "..\..\Include\DX11\DX11Sampler.h"
#include "..\..\Include\DX11\DX11RootLayout.h"
#include "..\..\Include\DX11\DX11RootVariableSlot.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11DeviceContext::DX11DeviceContext(IDevice* pDevice, bool isDeffered)
			: m_Device(nullptr),
			m_CurrentRootLayout(nullptr),
			m_CurrentPipelineState(nullptr),
			m_Context(nullptr),
			m_CommandList(nullptr),
			m_IsDeffered(false)
		{
			AddRef();
			m_Device = QueryDX11Device(pDevice);

			Create(isDeffered);
		}



		/////////////////////////////////////////////////////////////
		DX11DeviceContext::~DX11DeviceContext()
		{
			D3DRelease_S(m_Context);
			D3DRelease_S(m_CommandList);
			
			ReRelease_S(m_CurrentRootLayout);
			ReRelease_S(m_CurrentPipelineState);
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D11DeviceContext* DX11DeviceContext::GetD3D11DeviceContext() const
		{
			return m_Context;
		}



		/////////////////////////////////////////////////////////////
		ID3D11CommandList* DX11DeviceContext::GetD3D11CommandList() const
		{
			return m_CommandList;
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			ID3D11RenderTargetView* pD3D11View = reinterpret_cast<DX11RenderTargetView*>(pView)->GetD3D11RenderTargetView();
			m_Context->ClearRenderTargetView(pD3D11View, pColor);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			ID3D11DepthStencilView* pD3D11View = reinterpret_cast<DX11DepthStencilView*>(pView)->GetD3D11DepthStencilView();
			m_Context->ClearDepthStencilView(pD3D11View, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
			ID3D11RenderTargetView* pD3D11RenderTarget = reinterpret_cast<DX11RenderTargetView*>(pRenderTarget)->GetD3D11RenderTargetView();
			ID3D11DepthStencilView* pD3D11DepthStencil = reinterpret_cast<DX11DepthStencilView*>(pDepthStencil)->GetD3D11DepthStencilView();

			m_Context->OMSetRenderTargets(1, &pD3D11RenderTarget, pD3D11DepthStencil);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
			ID3D11ShaderResourceView* pD3D11View = reinterpret_cast<DX11ShaderResourceView*>(pShaderResourceView)->GetD3D11ShaderResourceView();
			m_CurrentRootLayout->GetDX11VariableSlot(startRootIndex)->SetShaderResourceViews(m_Context, &pD3D11View, 1);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
			ID3D11UnorderedAccessView* pD3D11View = reinterpret_cast<DX11UnorderedAccessView*>(pUnorderedAccessView)->GetD3D11UnorderedAccessView();
			m_CurrentRootLayout->GetDX11VariableSlot(startRootIndex)->SetUnorderedAccessViews(m_Context, &pD3D11View, 1);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
			ID3D11Buffer* pD3D11Buffer = reinterpret_cast<DX11Buffer*>(pBuffer)->GetD3D11Buffer();
			m_CurrentRootLayout->GetDX11VariableSlot(startRootIndex)->SetConstantBuffers(m_Context, &pD3D11Buffer, 1);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
			ID3D11SamplerState* pD3D11Sampler = reinterpret_cast<DX11Sampler*>(pSampler)->GetD3D11SamplerState();
			m_CurrentRootLayout->GetDX11VariableSlot(startRootIndex)->SetSamplers(m_Context, &pD3D11Sampler, 1);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
			DX11PipelineState* pDX11PipelineState = reinterpret_cast<DX11PipelineState*>(pPipelineState);
			
			if (pDX11PipelineState->GetPipelineType() == PIPELINE_TYPE_GRAPHICS)
				SetGraphicsPipeline(pDX11PipelineState);
			else
				SetComputePipeline(pDX11PipelineState);

			ReRelease_S(m_CurrentPipelineState);
			m_CurrentPipelineState = pDX11PipelineState->QueryReference<DX11PipelineState>();
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
			ReRelease_S(m_CurrentRootLayout);
			m_CurrentRootLayout = reinterpret_cast<DX11RootLayout*>(pRootLayout->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
			ID3D11Buffer* pD3D11Buffer = reinterpret_cast<DX11Buffer*>(pBuffer)->GetD3D11Buffer();
			uint32 stride = reinterpret_cast<DX11Buffer*>(pBuffer)->GetByteStride();
			uint32 offset = 0;

			m_Context->IASetVertexBuffers(0, 1, &pD3D11Buffer, &stride, &offset);
		}
		


		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetViewports(const Viewport& viewport) const
		{
			D3D11_VIEWPORT port = {};
			port.Width = viewport.Width;
			port.Height = viewport.Height;
			port.TopLeftX = viewport.TopLeftX;
			port.TopLeftY = viewport.TopLeftY;
			port.MaxDepth = viewport.MaxDepth;
			port.MinDepth = viewport.MinDepth;

			m_Context->RSSetViewports(1, &port);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
			m_Context->IASetPrimitiveTopology(ReToDXTopology(topology));
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetScissorRects(const Math::Rectangle& rect) const
		{
			D3D11_RECT dxRect = {};
			dxRect.top = static_cast<LONG>(rect.TopLeft.y);
			dxRect.left = static_cast<LONG>(rect.TopLeft.x);
			dxRect.bottom = static_cast<LONG>(rect.BottomRight.y);
			dxRect.right = static_cast<LONG>(rect.BottomRight.x);

			m_Context->RSSetScissorRects(1, &dxRect);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
			m_Context->Draw(vertexCount, startVertex);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
			m_Context->DrawIndexed(indexCount, startIndex, startVertex);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
			m_Context->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
			m_Context->DrawIndexedInstanced(indexCount, instanceCount, startIndex, startVertex, startInstance);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const
		{
			m_Context->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Flush() const
		{
			//NOT RELEVANT?
		}



		/////////////////////////////////////////////////////////////
		bool DX11DeviceContext::Reset() const
		{
			D3DRelease_S(m_CommandList);
			return true;
		}



		/////////////////////////////////////////////////////////////
		bool DX11DeviceContext::Close() const
		{
			if (!m_IsDeffered)
				return false;

			HRESULT hr = m_Context->FinishCommandList(FALSE, &m_CommandList);
			return SUCCEEDED(hr);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
			if (m_IsDeffered)
				return;

			ID3D11CommandList* pD3D11CommandList = reinterpret_cast<DX11DeviceContext*>(pDefferedContext)->GetD3D11CommandList();
			m_Context->ExecuteCommandList(pD3D11CommandList, TRUE);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetName(const std::string& name)
		{
			m_Context->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX11Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::Create(bool isDeffered)
		{
			using namespace System;

			if (isDeffered)
			{
				ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
				HRESULT hr = pD3D11Device->CreateDeferredContext(0, &m_Context);
				if (FAILED(hr))
				{
					m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create Deffered Context. " + DXErrorString(hr));
				}
			}
			else
			{
				ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
				pD3D11Device->GetImmediateContext(&m_Context);
			}

			m_IsDeffered = isDeffered;
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetGraphicsPipeline(DX11PipelineState* pGraphicsPipeline) const
		{
			m_Context->IASetInputLayout(pGraphicsPipeline->GetD3D11InputLayout());
			m_Context->OMSetDepthStencilState(pGraphicsPipeline->GetD3D11DepthStencilState(), 0);
			m_Context->OMSetBlendState(pGraphicsPipeline->GetD3D11BlendState(),
				pGraphicsPipeline->GetBlendFactor(), pGraphicsPipeline->GetSampleMask());
			m_Context->RSSetState(pGraphicsPipeline->GetD3D11RasterizerState());

			m_Context->VSSetShader(pGraphicsPipeline->GetD3D11VertexShader(), nullptr, 0);
			m_Context->HSSetShader(pGraphicsPipeline->GetD3D11HullShader(), nullptr, 0);
			m_Context->DSSetShader(pGraphicsPipeline->GetD3D11DomainShader(), nullptr, 0);
			m_Context->GSSetShader(pGraphicsPipeline->GetD3D11GeometryShader(), nullptr, 0);
			m_Context->PSSetShader(pGraphicsPipeline->GetD3D11PixelShader(), nullptr, 0);
		}



		/////////////////////////////////////////////////////////////
		void DX11DeviceContext::SetComputePipeline(DX11PipelineState* pComputePipeline) const
		{
			m_Context->CSSetShader(pComputePipeline->GetD3D11ComputeShader(), nullptr, 0);
		}
	}
}

#endif