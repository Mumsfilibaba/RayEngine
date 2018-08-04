#include "..\..\Include\DX11\DX11CommandQueue.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11RenderTargetView.h"
#include "..\..\Include\DX11\DX11DepthStencilView.h"
#include "..\..\Include\DX11\DX11Buffer.h"
#include "..\..\Include\DX11\DX11PipelineState.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11CommandQueue::DX11CommandQueue(IDevice* pDevice, const CommandQueueInfo& info)
			: m_Device(nullptr),
			m_DefferedContext(nullptr),
			m_List(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11CommandQueue::~DX11CommandQueue()
		{
			D3DRelease_S(m_DefferedContext);
			D3DRelease_S(m_List);
			
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		ID3D11CommandList* DX11CommandQueue::GetD3D11CommandList() const
		{
			return m_List;
		}



		/////////////////////////////////////////////////////////////
		ID3D11DeviceContext* DX11CommandQueue::GetD3D11DefferedContext() const
		{
			return m_DefferedContext;
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			ID3D11RenderTargetView* pD3D11View = reinterpret_cast<DX11RenderTargetView*>(pView)->GetD3D11RenderTargetView();
			m_DefferedContext->ClearRenderTargetView(pD3D11View, pColor);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			ID3D11DepthStencilView* pD3D11View = reinterpret_cast<DX11DepthStencilView*>(pView)->GetD3D11DepthStencilView();
			m_DefferedContext->ClearDepthStencilView(pD3D11View, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetPipelineState(IPipelineState* pPipelineState) const
		{
			DX11PipelineState* pDX11PipelineState = reinterpret_cast<DX11PipelineState*>(pPipelineState);

			m_DefferedContext->IASetInputLayout(pDX11PipelineState->GetD3D11InputLayout());
			m_DefferedContext->OMSetDepthStencilState(pDX11PipelineState->GetD3D11DepthStencilState(), 0);
			m_DefferedContext->OMSetBlendState(pDX11PipelineState->GetD3D11BlendState(),
				pDX11PipelineState->GetBlendFactor(), pDX11PipelineState->GetSampleMask());
			m_DefferedContext->RSSetState(pDX11PipelineState->GetD3D11RasterizerState());

			m_DefferedContext->VSSetShader(pDX11PipelineState->GetD3D11VertexShader(), nullptr, 0);
			m_DefferedContext->HSSetShader(pDX11PipelineState->GetD3D11HullShader(), nullptr, 0);
			m_DefferedContext->DSSetShader(pDX11PipelineState->GetD3D11DomainShader(), nullptr, 0);
			m_DefferedContext->GSSetShader(pDX11PipelineState->GetD3D11GeometryShader(), nullptr, 0);
			m_DefferedContext->PSSetShader(pDX11PipelineState->GetD3D11PixelShader(), nullptr, 0);
			m_DefferedContext->CSSetShader(pDX11PipelineState->GetD3D11ComputeShader(), nullptr, 0);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetRootSignature(IRootSignature* pRootSignature) const
		{
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
			ID3D11Buffer* pD3D11Buffer = reinterpret_cast<DX11Buffer*>(pBuffer)->GetD3D11Buffer();
			uint32 stride = reinterpret_cast<DX11Buffer*>(pBuffer)->GetByteStride();
			uint32 offset = 0;

			m_DefferedContext->IASetVertexBuffers(0, 1, &pD3D11Buffer, &stride, &offset);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
			ID3D11RenderTargetView* pD3D11RenderTarget = reinterpret_cast<DX11RenderTargetView*>(pRenderTarget)->GetD3D11RenderTargetView();
			ID3D11DepthStencilView* pD3D11DepthStencil = reinterpret_cast<DX11DepthStencilView*>(pDepthStencil)->GetD3D11DepthStencilView();

			m_DefferedContext->OMSetRenderTargets(1, &pD3D11RenderTarget, pD3D11DepthStencil);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetViewports(const Viewport& viewport) const
		{
			D3D11_VIEWPORT port = {};
			port.Width = viewport.Width;
			port.Height = viewport.Height;
			port.TopLeftX = viewport.TopLeftX;
			port.TopLeftY = viewport.TopLeftY;
			port.MaxDepth = viewport.MaxDepth;
			port.MinDepth = viewport.MinDepth;

			m_DefferedContext->RSSetViewports(1, &port);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetScissorRects(const Math::Rectangle& rect) const
		{
			D3D11_RECT dxRect = {};
			dxRect.top = static_cast<LONG>(rect.TopLeft.y);
			dxRect.left = static_cast<LONG>(rect.TopLeft.x);
			dxRect.bottom = static_cast<LONG>(rect.BottomRight.y);
			dxRect.right = static_cast<LONG>(rect.BottomRight.x);

			m_DefferedContext->RSSetScissorRects(1, &dxRect);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
			D3D_PRIMITIVE_TOPOLOGY dxTopology;

			switch (topology)
			{
			case PRIMITIVE_TOPOLOGY_TRIANGLELIST: dxTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST; break;
			case PRIMITIVE_TOPOLOGY_TRIANGLESTRIP: dxTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP; break;
			case PRIMITIVE_TOPOLOGY_LINELIST: dxTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST; break;
			case PRIMITIVE_TOPOLOGY_LINESTRIP: dxTopology = D3D_PRIMITIVE_TOPOLOGY_LINESTRIP; break;
			case PRIMITIVE_TOPOLOGY_POINTS: dxTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST; break;
			default: dxTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED; break;
			}

			m_DefferedContext->IASetPrimitiveTopology(dxTopology);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::TransitionResource(ITexture* pResource, RESOURCE_STATE to, int32 subresource) const
		{
			//NOT RELEVANT
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::Draw(int32 startVertex, int32 vertexCount) const
		{
			m_DefferedContext->Draw(vertexCount, startVertex);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
			m_DefferedContext->DrawIndexed(indexCount, startIndex, startVertex);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
			m_DefferedContext->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
			m_DefferedContext->DrawIndexedInstanced(indexCount, instanceCount, startIndex, startVertex, startInstance);
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::Flush() const
		{
			//NOT RELEVANT
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::Execute() const
		{
			ID3D11DeviceContext* pImmediateContext = reinterpret_cast<DX11Device*>(m_Device)->GetD3D11DeviceContext();
			pImmediateContext->ExecuteCommandList(m_List, TRUE);
		}



		/////////////////////////////////////////////////////////////
		bool DX11CommandQueue::Reset() const
		{
			D3DRelease_S(m_List);
			return true;
		}



		/////////////////////////////////////////////////////////////
		bool DX11CommandQueue::Close() const
		{
			HRESULT hr = m_DefferedContext->FinishCommandList(FALSE, &m_List);
			return SUCCEEDED(hr);
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11CommandQueue::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void DX11CommandQueue::Create(IDevice* pDevice, const CommandQueueInfo& info)
		{
			using namespace System;

			ID3D11Device* pD3D11Device = reinterpret_cast<DX11Device*>(pDevice)->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateDeferredContext(0, &m_DefferedContext);
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create Deffered Context");
				return;
			}

			hr = m_DefferedContext->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			return;
		}
	}
}

#endif