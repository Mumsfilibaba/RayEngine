#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12PipelineState.h"
#include "..\..\Include\DX12\DX12RootSignature.h"
#include "..\..\Include\DX12\DX12Buffer.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue()
			: m_Device(nullptr),
			m_Queue(nullptr),
			m_Allocator(nullptr),
			m_List(nullptr),
			m_Fence(nullptr),
			m_CurrentFence(0),
			m_ReferenceCounter(0)
		{
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(IDevice* pDevice, const CommandQueueInfo& info)
			: m_Device(nullptr),
			m_Queue(nullptr),
			m_Allocator(nullptr),
			m_List(nullptr),
			m_Fence(nullptr),
			m_CurrentFence(0),
			m_ReferenceCounter(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::DX12CommandQueue(DX12CommandQueue&& other)
			: m_Device(other.m_Device),
			m_Queue(other.m_Queue),
			m_Allocator(other.m_Allocator),
			m_List(other.m_List),
			m_Fence(other.m_Fence),
			m_CurrentFence(other.m_CurrentFence),
			m_ReferenceCounter(other.m_ReferenceCounter)
		{
			other.m_Device = nullptr;
			other.m_Queue = nullptr;
			other.m_Allocator = nullptr;
			other.m_List = nullptr;
			other.m_Fence = nullptr;
			other.m_CurrentFence = 0;
			other.m_ReferenceCounter = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue::~DX12CommandQueue()
		{
			D3DRelease_S(m_Queue);
			D3DRelease_S(m_Allocator);
			D3DRelease_S(m_List);
			D3DRelease_S(m_Fence);

			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			DX12RenderTargetView* pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			m_List->ClearRenderTargetView(pDX12View->GetD3D12CpuDescriptorHandle(), pColor, 0, nullptr);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			DX12RenderTargetView* pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			m_List->ClearDepthStencilView(pDX12View->GetD3D12CpuDescriptorHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetPipelineState(IPipelineState* pPipelineState) const
		{
			DX12PipelineState* pDX12PipelineState = reinterpret_cast<DX12PipelineState*>(pPipelineState);

			m_List->SetPipelineState(pDX12PipelineState->GetD3D12PipelineState());
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetRootSignature(IRootSignature* pRootSignature) const
		{
			DX12RootSignature* pDX12RootSignature = reinterpret_cast<DX12RootSignature*>(pRootSignature);

			m_List->SetGraphicsRootSignature(pDX12RootSignature->GetD3D12RootSignature());
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
			DX12Buffer* pDX12Buffer = reinterpret_cast<DX12Buffer*>(pBuffer);

			D3D12_VERTEX_BUFFER_VIEW view = pDX12Buffer->GetD3D12VertexBufferView();
			m_List->IASetVertexBuffers(startSlot, 1, &view);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
			DX12RenderTargetView* pDX12RenderTarget = reinterpret_cast<DX12RenderTargetView*>(pRenderTarget);
			DX12DepthStencilView* pDX12DepthStencil = reinterpret_cast<DX12DepthStencilView*>(pDepthStencil);

			D3D12_CPU_DESCRIPTOR_HANDLE rtv = pDX12RenderTarget->GetD3D12CpuDescriptorHandle();
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = pDX12DepthStencil->GetD3D12CpuDescriptorHandle();

			m_List->OMSetRenderTargets(1, &rtv, false, &dsv);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetViewports(const Viewport& viewport) const
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

			m_List->RSSetViewports(1, &view);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetScissorRects(const Math::Rectangle& rect) const
		{
			D3D12_RECT dxRect = 
			{
				static_cast<LONG>(rect.TopLeft.x),
				static_cast<LONG>(rect.TopLeft.y),
				static_cast<LONG>(rect.BottomRight.x),
				static_cast<LONG>(rect.BottomRight.y)
			};

			m_List->RSSetScissorRects(1, &dxRect);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
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

			m_List->IASetPrimitiveTopology(dxTopology);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::CopyResource(DX12Resource* dst, const DX12Resource* src) const
		{
			m_List->CopyResource(dst->GetD3D12Resource(), src->GetD3D12Resource());
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::CopyTextureRegion(DX12Resource* dst, const DX12Resource* src, DXGI_FORMAT format, 
			int32 width, int32 height, int32 depth, int32 stride) const
		{
			D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
			srcLoc.pResource = src->GetD3D12Resource();
			srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			srcLoc.PlacedFootprint.Offset = 0;
			srcLoc.PlacedFootprint.Footprint.Width = width;
			srcLoc.PlacedFootprint.Footprint.Height = height;
			srcLoc.PlacedFootprint.Footprint.Depth = depth;
			srcLoc.PlacedFootprint.Footprint.Format = format;
			srcLoc.PlacedFootprint.Footprint.RowPitch = width * stride * depth;


			D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
			dstLoc.pResource = dst->GetD3D12Resource();
			dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dstLoc.SubresourceIndex = 0;

			m_List->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::TransitionResource(DX12Resource* resource, D3D12_RESOURCE_STATES to, int32 subresource) const
		{
			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

			barrier.Transition.Subresource = subresource;

			barrier.Transition.pResource = resource->GetD3D12Resource();

			barrier.Transition.StateBefore = resource->GetD3D12State();
			barrier.Transition.StateAfter = to;

			m_List->ResourceBarrier(1, &barrier);

			resource->SetD3D12State(to);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::TransitionResource(ITexture* pResource, RESOURCE_STATE to, int32 subresource) const
		{
			DX12Texture* pDX12resource = reinterpret_cast<DX12Texture*>(pResource);
			TransitionResource(pDX12resource, ReToDXResourceState(to), subresource);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Draw(int32 startVertex, int32 vertexCount) const
		{
			m_List->DrawInstanced(vertexCount, 1, startVertex, 0);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
			m_List->DrawIndexedInstanced(indexCount, 1, startIndex, startVertex, 0);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
			m_List->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
			m_List->DrawIndexedInstanced(indexCount, instanceCount, startIndex, startVertex, startInstance);
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Flush() const
		{
			m_CurrentFence++;
			if (FAILED(m_Queue->Signal(m_Fence, m_CurrentFence)))
				return;

			if (m_Fence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE ev = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

				if (FAILED(m_Fence->SetEventOnCompletion(m_CurrentFence, ev)))
					return;

				WaitForSingleObject(ev, INFINITE);
				CloseHandle(ev);
			}

			return;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Execute() const
		{
			ID3D12CommandList* pList = m_List;
			m_Queue->ExecuteCommandLists(1, &pList);
		}



		/////////////////////////////////////////////////////////////
		bool DX12CommandQueue::Reset() const
		{
			if (FAILED(m_Allocator->Reset()))
				return false;

			if (FAILED(m_List->Reset(m_Allocator, nullptr)))
				return false;

			return true;
		}



		/////////////////////////////////////////////////////////////
		bool DX12CommandQueue::Close() const
		{
			if (FAILED(m_List->Close()))
				return false;

			return true;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12CommandQueue::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue& DX12CommandQueue::operator=(DX12CommandQueue&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Queue);
				D3DRelease_S(m_Allocator);
				D3DRelease_S(m_List);
				D3DRelease_S(m_Fence);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_Queue = other.m_Queue;
				m_Allocator = other.m_Allocator;
				m_List = other.m_List;
				m_Fence = other.m_Fence;
				m_CurrentFence = other.m_CurrentFence;
				m_ReferenceCounter = other.m_ReferenceCounter;


				other.m_Device = nullptr;
				other.m_Queue = nullptr;
				other.m_Allocator = nullptr;
				other.m_List = nullptr;
				other.m_Fence = nullptr;
				other.m_CurrentFence = 0;
				other.m_ReferenceCounter = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D12CommandQueue* DX12CommandQueue::GetD3D12CommandQueue() const
		{
			return m_Queue;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12CommandQueue::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12CommandQueue::GetReferenceCount() const
		{
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Release() const
		{
			m_ReferenceCounter--;
			if (m_ReferenceCounter < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12CommandQueue::AddRef()
		{
			m_ReferenceCounter++;
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX12CommandQueue::Create(IDevice* pDevice, const CommandQueueInfo& info)
		{
			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();

			//TODO: different types of commandqueues

			D3D12_COMMAND_QUEUE_DESC qDesc = {};
			qDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			qDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			qDesc.NodeMask = 0;


			HRESULT hr = pD3D12Device->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_Queue));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, DXErrorString(hr) + "DX12: Could not create CommandQueue");
				return;
			}
			else
			{ 
				D3D12SetName(m_Queue, info.Name);
			}


			hr = pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_Allocator));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, DXErrorString(hr) + "DX12: Could not create CommandAllocator");
				return;
			}
			else
			{
				D3D12SetName(m_Allocator, info.Name + ": Allocator");
			}


			hr = pD3D12Device->CreateCommandList(qDesc.NodeMask, D3D12_COMMAND_LIST_TYPE_DIRECT, m_Allocator, nullptr, IID_PPV_ARGS(&m_List));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, DXErrorString(hr) + "DX12: Could not create CommandList");
				return;
			}
			else
			{
				D3D12SetName(m_List, info.Name + ": List");
			}


			hr = pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
			if (FAILED(hr))
			{
				pDevice->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, DXErrorString(hr) + "DX12: Could not create Fence");
				return;
			}


			Close();
			return;
		}
	}
}

#endif