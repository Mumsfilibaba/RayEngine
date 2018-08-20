#include "..\..\Include\DX12\DX12DeviceContext.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12RootLayout.h"
#include "..\..\Include\DX12\DX12PipelineState.h"
#include "..\..\Include\DX12\DX12RendertargetView.h"
#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12ShaderResourceView.h"
#include "..\..\Include\DX12\DX12UnorderedAccessView.h"
#include "..\..\Include\DX12\DX12Buffer.h"
#include "..\..\Include\DX12\DX12Sampler.h"
#include "..\..\Include\DX12\DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DeviceContext::DX12DeviceContext(IDevice* pDevice, bool isDeffered)
			: m_Device(nullptr),
			m_CurrentRootLayout(nullptr),
			m_Allocator(nullptr),
			m_Queue(nullptr),
			m_CommandList(nullptr),
			m_CurrentFence(0),
			m_NumCommands(0),
			m_MaxCommands(15),
			m_IsDeffered(false)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(isDeffered);
		}



		/////////////////////////////////////////////////////////////
		DX12DeviceContext::~DX12DeviceContext()
		{
			D3DRelease_S(m_Allocator);
			D3DRelease_S(m_Queue);
			D3DRelease_S(m_CommandList);
			D3DRelease_S(m_Fence);

			ReRelease_S(m_Device);
			ReRelease_S(m_CurrentRootLayout);
		}



		/////////////////////////////////////////////////////////////
		ID3D12Fence* DX12DeviceContext::GetD3D12Fence() const
		{
			return m_Fence;
		}



		/////////////////////////////////////////////////////////////s
		ID3D12CommandList* DX12DeviceContext::GetD3D12CommandList() const
		{
			return m_CommandList;
		}



		/////////////////////////////////////////////////////////////
		ID3D12CommandQueue* DX12DeviceContext::GetD3D12CommandQueue() const
		{
			return m_Queue;
		}



		/////////////////////////////////////////////////////////////
		ID3D12CommandAllocator* DX12DeviceContext::GetD3D12CommandAllocator() const
		{
			return m_Allocator;
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::TransitionResource(DX12Resource* pResource, D3D12_RESOURCE_STATES to, int32 subresource) const
		{
			ID3D12Resource* pD3D12Resource = pResource->GetD3D12Resource();
			D3D12_RESOURCE_STATES from = pResource->GetD3D12State();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.Subresource = subresource;
			barrier.Transition.pResource = pD3D12Resource;
			barrier.Transition.StateBefore = from;
			barrier.Transition.StateAfter = to;

			m_CommandList->ResourceBarrier(1, &barrier);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			DX12RenderTargetView* pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			m_CommandList->ClearRenderTargetView(pDX12View->GetD3D12CpuDescriptorHandle(), pColor, 0, nullptr);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			DX12RenderTargetView* pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			m_CommandList->ClearDepthStencilView(pDX12View->GetD3D12CpuDescriptorHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
			DX12RenderTargetView* pDX12RenderTarget = reinterpret_cast<DX12RenderTargetView*>(pRenderTarget);
			DX12DepthStencilView* pDX12DepthStencil = reinterpret_cast<DX12DepthStencilView*>(pDepthStencil);

			D3D12_CPU_DESCRIPTOR_HANDLE rtv = pDX12RenderTarget->GetD3D12CpuDescriptorHandle();
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = pDX12DepthStencil->GetD3D12CpuDescriptorHandle();

			m_CommandList->OMSetRenderTargets(1, &rtv, false, &dsv);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
			DX12PipelineState* pDX12PipelineState = reinterpret_cast<DX12PipelineState*>(pPipelineState);
			m_CommandList->SetPipelineState(pDX12PipelineState->GetD3D12PipelineState());

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
			DX12RootLayout* pDX12RootLayout = reinterpret_cast<DX12RootLayout*>(pRootLayout);
			m_CommandList->SetGraphicsRootSignature(pDX12RootLayout->GetD3D12RootSignature());

			ReRelease_S(m_CurrentRootLayout);
			m_CurrentRootLayout = reinterpret_cast<DX12RootLayout*>(pRootLayout->GetReferenceCount());

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
			DX12Buffer* pDX12Buffer = reinterpret_cast<DX12Buffer*>(pBuffer);

			D3D12_VERTEX_BUFFER_VIEW view = pDX12Buffer->GetD3D12VertexBufferView();
			m_CommandList->IASetVertexBuffers(startSlot, 1, &view);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetViewports(const Viewport& viewport) const
		{
			D3D12_VIEWPORT view =
			{
				viewport.TopLeftX,
				viewport.TopLeftY,
				viewport.Width,
				viewport.Height,
				viewport.MinDepth,
				viewport.MaxDepth
			};

			m_CommandList->RSSetViewports(1, &view);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
			m_CommandList->IASetPrimitiveTopology(ReToDXTopology(topology));

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetScissorRects(const Math::Rectangle& rect) const
		{
			D3D12_RECT dxRect =
			{
				static_cast<LONG>(rect.TopLeft.x),
				static_cast<LONG>(rect.TopLeft.y),
				static_cast<LONG>(rect.BottomRight.x),
				static_cast<LONG>(rect.BottomRight.y)
			};

			m_CommandList->RSSetScissorRects(1, &dxRect);

			AddCommand();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
			if (!m_IsDeffered)
				Flush();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
			if (!m_IsDeffered)
				Flush();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
			if (!m_IsDeffered)
				Flush();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
			if (!m_IsDeffered)
				Flush();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::Flush() const
		{
			using namespace System;

			m_CurrentFence++;
			HRESULT hr = m_Queue->Signal(m_Fence, m_CurrentFence);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Signal fence failed. " + DXErrorString(hr));
				return;
			}



			if (m_Fence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE ev = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

				hr = m_Fence->SetEventOnCompletion(m_CurrentFence, ev);
				if (FAILED(m_Fence->SetEventOnCompletion(m_CurrentFence, ev)))
				{
					m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Failed to set event. " + DXErrorString(hr));
					return;
				}

				WaitForSingleObject(ev, INFINITE);
				CloseHandle(ev);
			}
		}



		/////////////////////////////////////////////////////////////
		bool DX12DeviceContext::Reset() const
		{
			if (FAILED(m_Allocator->Reset()))
				return false;

			if (FAILED(m_CommandList->Reset(m_Allocator, nullptr)))
				return false;

			return true;
		}



		/////////////////////////////////////////////////////////////
		bool DX12DeviceContext::Close() const
		{
			if (!m_IsDeffered)
				return;

			HRESULT hr = m_CommandList->Close();
			return SUCCEEDED(hr);
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
			if (m_IsDeffered)
				return;

			DX12DeviceContext* pDX12DeviceContext = reinterpret_cast<DX12DeviceContext*>(pDefferedContext);
			ID3D12CommandList* lists[] = { pDX12DeviceContext->GetD3D12CommandList() };
			m_Queue->ExecuteCommandLists(1, lists);

			pDX12DeviceContext->Flush();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::Create(bool isDeffered)
		{
			using namespace System;

			//TODO: different types of commandqueues

			D3D12_COMMAND_QUEUE_DESC qDesc = {};
			qDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			qDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			qDesc.NodeMask = 0;


			HRESULT hr = 0;
			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();

			if (isDeffered)
			{
				pD3D12Device->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_Queue));
				if (FAILED(hr))
				{
					m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "DX12: Could not create CommandQueue. " + DXErrorString(hr));
					return;
				}
				else
				{
					D3D12SetName(m_Queue, "DeviceContext");
				}
			}



			hr = pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_Allocator));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "DX12: Could not create CommandAllocator. " + DXErrorString(hr));
				return;
			}
			else
			{
				D3D12SetName(m_Allocator, "DeviceContext : Allocator");
			}



			hr = pD3D12Device->CreateCommandList(qDesc.NodeMask, D3D12_COMMAND_LIST_TYPE_DIRECT, m_Allocator, nullptr, IID_PPV_ARGS(&m_CommandList));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "DX12: Could not create CommandList. " + DXErrorString(hr));
				return;
			}
			else
			{
				D3D12SetName(m_CommandList, "DeviceContext : List");
			}



			hr = pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, DXErrorString(hr) + "DX12: Could not create Fence");
			}
			else
			{
				Close();
			}



			m_IsDeffered = isDeffered;
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::AddCommand() const
		{
			if (m_IsDeffered)
				return;

			m_NumCommands++;
			if (m_NumCommands >= m_MaxCommands)
				Flush();
		}



		/////////////////////////////////////////////////////////////
		void DX12DeviceContext::ExecuteCommandList()
		{
			ID3D12CommandList* lists[] = { m_CommandList };
			m_Queue->ExecuteCommandLists(1, lists);

			if (m_NumCommands >= m_MaxCommands)
				Flush();
		}
	}
}

#endif