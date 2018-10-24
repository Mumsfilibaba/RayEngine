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
#include "..\..\Include\DX12\DX12RootVariableSlot.h"
#include "..\..\Include\DX12\DX12CommandList.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DeviceContext::DX12DeviceContext(IDevice* pDevice, bool isDeffered)
			: m_Device(nullptr),
			m_Queue(nullptr),
			m_List(nullptr),
			m_ComputeList(nullptr),
			m_Fence(nullptr),
			m_CurrentRootLayout(nullptr),
			m_CurrentFence(0),
			m_NumCommands(0),
			m_MaxCommands(RE_DX12_MAX_COMMANDS),
			m_IsDeffered(false),
			m_DefferedBarriers(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(isDeffered);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DeviceContext::~DX12DeviceContext()
		{
			D3DRelease_S(m_Queue);
			D3DRelease_S(m_Fence);

			ReRelease_S(m_CurrentRootLayout);
			ReRelease_S(m_List);
			ReRelease_S(m_ComputeList);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::CopyResource(DX12Resource* pDst, DX12Resource* pSrc) const
		{
			ID3D12Resource* pD3D12Dst = pDst->GetD3D12Resource();
			ID3D12Resource* pD3D12Src = pSrc->GetD3D12Resource();

			CommitDefferedBarriers();
			m_List->GetD3D12GraphicsCommandList()->CopyResource(pD3D12Dst, pD3D12Src);


			if (!m_IsDeffered)
				ExecuteCommandLists();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::CopyTexture(DX12Resource* pDst, DX12Resource* pSrc, DXGI_FORMAT format, int32 width, int32 height, int32 depth, int32 stride) const
		{
			ID3D12Resource* pD3D12Dst = pDst->GetD3D12Resource();
			ID3D12Resource* pD3D12Src = pSrc->GetD3D12Resource();


			D3D12_TEXTURE_COPY_LOCATION srcLoc = {};
			srcLoc.pResource = pD3D12Src;
			srcLoc.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
			srcLoc.PlacedFootprint.Offset = 0;
			srcLoc.PlacedFootprint.Footprint.Width = width;
			srcLoc.PlacedFootprint.Footprint.Height = height;
			srcLoc.PlacedFootprint.Footprint.Depth = depth;
			srcLoc.PlacedFootprint.Footprint.Format = format;
			srcLoc.PlacedFootprint.Footprint.RowPitch = width * stride * depth;


			D3D12_TEXTURE_COPY_LOCATION dstLoc = {};
			dstLoc.pResource = pD3D12Dst;
			dstLoc.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
			dstLoc.SubresourceIndex = 0;


			CommitDefferedBarriers();
			m_List->GetD3D12GraphicsCommandList()->CopyTextureRegion(&dstLoc, 0, 0, 0, &srcLoc, nullptr);


			if (!m_IsDeffered)
				ExecuteCommandLists();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::TransitionResource(DX12Resource* pResource, D3D12_RESOURCE_STATES to, int32 subresource) const
		{
			D3D12_RESOURCE_STATES from = pResource->GetD3D12State();

			if (from == to)
				return;

			D3D12_RESOURCE_BARRIER barrier = CreateTransitionBarrier(pResource, from, to, subresource);
			m_List->GetD3D12GraphicsCommandList()->ResourceBarrier(1, &barrier);

			AddCommand();

			pResource->SetD3D12State(to);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::TransitionResourceGroup(DX12Resource* const * ppResource, D3D12_RESOURCE_STATES* pToStates, int32* pSbresources, int32 count) const
		{
			std::vector<D3D12_RESOURCE_BARRIER> barriers;
			barriers.reserve(count);
			
			for (int32 i = 0; i < count; i++)
			{
				D3D12_RESOURCE_STATES from = ppResource[i]->GetD3D12State();

				if (from == pToStates[i])
					continue;

				barriers.push_back(CreateTransitionBarrier(ppResource[i], from, pToStates[i], pSbresources[i]));

				ppResource[i]->SetD3D12State(pToStates[i]);
			}

			m_List->GetD3D12GraphicsCommandList()->ResourceBarrier(static_cast<uint32>(barriers.size()), barriers.data());

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::TransitionResourceIndirect(DX12Resource* pResource, D3D12_RESOURCE_STATES to, int32 subresource) const
		{
			D3D12_RESOURCE_STATES from = pResource->GetD3D12State();

			if (from == to)
				return;

			m_DefferedBarriers.push_back(CreateTransitionBarrier(pResource, from, to, subresource));

			pResource->SetD3D12State(to);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::TransitionResourceGroupIndirect(DX12Resource* const * ppResource, D3D12_RESOURCE_STATES* pToStates, int32* pSbresources, int32 count) const
		{
			for (int32 i = 0; i < count; i++)
			{
				D3D12_RESOURCE_STATES from = ppResource[i]->GetD3D12State();

				if (from == pToStates[i])
					continue;

				m_DefferedBarriers.push_back(CreateTransitionBarrier(ppResource[i], from, pToStates[i], pSbresources[i]));

				ppResource[i]->SetD3D12State(pToStates[i]);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ClearRendertargetView(IRenderTargetView* pView, float pColor[4]) const
		{
			DX12RenderTargetView* pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			
			//TODO: Subresource may not be zero

			TransitionResource(pDX12View->GetD3D12Resource(), D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
			m_List->GetD3D12GraphicsCommandList()->ClearRenderTargetView(pDX12View->GetD3D12CpuDescriptorHandle(), pColor, 0, nullptr);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			DX12RenderTargetView* pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			
			//TODO: Subresource may not be zero

			TransitionResource(pDX12View->GetD3D12Resource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, 0);
			m_List->GetD3D12GraphicsCommandList()->ClearDepthStencilView(pDX12View->GetD3D12CpuDescriptorHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetSwapChain(ISwapchain* pSwapChain) const
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
			//TODO: We need a nulldescriptor

			DX12RenderTargetView* pDX12RenderTarget = reinterpret_cast<DX12RenderTargetView*>(pRenderTarget);
			DX12DepthStencilView* pDX12DepthStencil = reinterpret_cast<DX12DepthStencilView*>(pDepthStencil);

			D3D12_CPU_DESCRIPTOR_HANDLE rtv = pDX12RenderTarget->GetD3D12CpuDescriptorHandle();
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = pDX12DepthStencil->GetD3D12CpuDescriptorHandle();

			DX12Resource* ppResources[] = { pDX12RenderTarget->GetD3D12Resource(), pDX12DepthStencil->GetD3D12Resource() };
			D3D12_RESOURCE_STATES pToStates[] = { D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_DEPTH_WRITE };
			int32 pSubresources[] = { 0, 0 };

			TransitionResourceGroupIndirect(ppResources, pToStates, pSubresources, 2);
			m_List->GetD3D12GraphicsCommandList()->OMSetRenderTargets(1, &rtv, false, &dsv);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
			DX12ShaderResourceView* pDX12View = reinterpret_cast<DX12ShaderResourceView*>(pShaderResourceView);
			DX12DescriptorHandle srv = pDX12View->GetDX12DescriptorHandleSRVCBVUAV();
			
			//TODO: Maybe the subreource is not 0

			DX12RootVariableSlot* pDX12Slot = m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex);
			TransitionResourceIndirect(pDX12View->GetD3D12Resource(), pDX12Slot->GetNeededD3D12ResourceState(), 0);
			pDX12Slot->SetShaderResourceViews(m_List->GetD3D12GraphicsCommandList(), &srv, 1);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
			DX12ShaderResourceView* pDX12View = reinterpret_cast<DX12ShaderResourceView*>(pUnorderedAccessView);
			DX12DescriptorHandle uav = pDX12View->GetDX12DescriptorHandleSRVCBVUAV();

			DX12RootVariableSlot* pDX12Slot = m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex);
			TransitionResourceIndirect(pDX12View->GetD3D12Resource(), pDX12Slot->GetNeededD3D12ResourceState(), 0);
			pDX12Slot->SetUnorderedAccessViews(m_List->GetD3D12GraphicsCommandList(), &uav, 1);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetConstantBuffers(IBuffer* pBuffer, int32 startRootIndex) const
		{
			DX12Buffer* pDX12Buffer = reinterpret_cast<DX12Buffer*>(pBuffer);
			DX12DescriptorHandle cbv = pDX12Buffer->GetDX12DescriptorHandleSRVCBVUAV();

			DX12RootVariableSlot* pDX12Slot = m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex);
			TransitionResourceIndirect(pDX12Buffer, pDX12Slot->GetNeededD3D12ResourceState(), 0);
			pDX12Slot->SetUnorderedAccessViews(m_List->GetD3D12GraphicsCommandList(), &cbv, 1);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetSamplers(ISampler* pSampler, int32 startRootIndex) const
		{
			DX12DescriptorHandle sampler = reinterpret_cast<DX12Buffer*>(pSampler)->GetDX12DescriptorHandleSRVCBVUAV();
			m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex)->SetSamplers(m_List->GetD3D12GraphicsCommandList(), &sampler, 1);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
			DX12PipelineState* pDX12PipelineState = reinterpret_cast<DX12PipelineState*>(pPipelineState);
			m_List->GetD3D12GraphicsCommandList()->SetPipelineState(pDX12PipelineState->GetD3D12PipelineState());

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetRootLayout(IRootLayout* pRootLayout) const
		{
			DX12RootLayout* pDX12RootLayout = reinterpret_cast<DX12RootLayout*>(pRootLayout);
			m_List->GetD3D12GraphicsCommandList()->SetGraphicsRootSignature(pDX12RootLayout->GetD3D12RootSignature());

			ReRelease_S(m_CurrentRootLayout);
			m_CurrentRootLayout = pRootLayout->QueryReference<DX12RootLayout>();

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetVertexBuffers(IBuffer* pBuffer, int32 startSlot) const
		{
			DX12Buffer* pDX12Buffer = reinterpret_cast<DX12Buffer*>(pBuffer);

			//TODO: Maybe subresource ain't 0

			D3D12_VERTEX_BUFFER_VIEW view = pDX12Buffer->GetD3D12VertexBufferView();
			TransitionResourceIndirect(pDX12Buffer, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, 0);
			m_List->GetD3D12GraphicsCommandList()->IASetVertexBuffers(startSlot, 1, &view);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetViewports(const Viewport* pViewport) const
		{
			D3D12_VIEWPORT view =
			{
				pViewport->TopLeftX,
				pViewport->TopLeftY,
				pViewport->Width,
				pViewport->Height,
				pViewport->MinDepth,
				pViewport->MaxDepth
			};

			m_List->GetD3D12GraphicsCommandList()->RSSetViewports(1, &view);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetPrimitiveTopology(PRIMITIVE_TOPOLOGY topology) const
		{
			m_List->GetD3D12GraphicsCommandList()->IASetPrimitiveTopology(ReToDXTopology(topology));

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetScissorRects(const Rect* pRect) const
		{
			D3D12_RECT dxRect =
			{
				pRect->Left,
				pRect->Top,
				pRect->Right,
				pRect->Bottom
			};

			m_List->GetD3D12GraphicsCommandList()->RSSetScissorRects(1, &dxRect);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::Draw(int32 startVertex, int32 vertexCount) const
		{
			CommitDefferedBarriers();
			m_List->GetD3D12GraphicsCommandList()->DrawInstanced(vertexCount, 1, startVertex, 0);

			if (!m_IsDeffered)
			{
				ExecuteCommandLists();
				Flush();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::DrawIndexed(int32 startVertex, int32 startIndex, int32 indexCount) const
		{
			CommitDefferedBarriers();
			m_List->GetD3D12GraphicsCommandList()->DrawIndexedInstanced(indexCount, 1, startIndex, startVertex, 0);

			if (!m_IsDeffered)
			{
				ExecuteCommandLists();
				Flush();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::DrawInstanced(int32 startVertex, int32 vertexCount, int32 startInstance, int32 instanceCount) const
		{
			CommitDefferedBarriers();
			m_List->GetD3D12GraphicsCommandList()->DrawInstanced(vertexCount, instanceCount, startVertex, startInstance);

			if (!m_IsDeffered)
			{
				ExecuteCommandLists();
				Flush();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::DrawIndexInstanced(int32 startVertex, int32 startIndex, int32 indexCount, int32 startInstance, int32 instanceCount) const
		{
			CommitDefferedBarriers();
			m_List->GetD3D12GraphicsCommandList()->DrawIndexedInstanced(indexCount, instanceCount, startIndex, startVertex, startInstance);

			if (!m_IsDeffered)
			{
				ExecuteCommandLists();
				Flush();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::Dispatch(int32 threadGroupCountX, int32 threadGroupCountY, int32 threadGroupCountZ) const
		{
			CommitDefferedBarriers();
			m_ComputeList->GetD3D12GraphicsCommandList()->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);

			if (!m_IsDeffered)
			{
				ExecuteCommandLists();
				Flush();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::Flush() const
		{
			using namespace System;

			if (m_IsDeffered)
				return;

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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12DeviceContext::Reset() const
		{
			return m_List->Reset();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12DeviceContext::Close() const
		{
			if (!m_IsDeffered)
				return false;

			HRESULT hr = m_List->GetD3D12GraphicsCommandList()->Close();
			return SUCCEEDED(hr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
			if (m_IsDeffered)
				return;

			DX12DeviceContext* pDX12DeviceContext = reinterpret_cast<DX12DeviceContext*>(pDefferedContext);
			
			ID3D12CommandList* lists[] = { pDX12DeviceContext->GetDX12List()->GetD3D12CommandList() };
			m_Queue->ExecuteCommandLists(1, lists);

			Flush();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetName(const std::string& name)
		{
			D3D12SetName(m_Queue, name);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12DeviceContext::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12DeviceContext::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12DeviceContext::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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



				hr = pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
				if (FAILED(hr))
				{
					m_Device->GetDeviceLog()->Write(System::LOG_SEVERITY_ERROR, DXErrorString(hr) + "DX12: Could not create Fence");
					return;
				}
				else
				{
					Close();
				}
			}
			
			m_List = new DX12CommandList(m_Device, nullptr, D3D12_COMMAND_LIST_TYPE_DIRECT, qDesc.NodeMask);
			m_ComputeList = new DX12CommandList(m_Device, nullptr, D3D12_COMMAND_LIST_TYPE_COMPUTE, qDesc.NodeMask);

			m_IsDeffered = isDeffered;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::AddCommand() const
		{
			if (m_IsDeffered)
				return;

			m_NumCommands++;
			if (m_NumCommands >= m_MaxCommands)
			{
				ExecuteCommandLists();
				Flush();
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ExecuteCommandLists() const
		{
			ID3D12CommandList* lists[] = { m_ComputeList->GetD3D12GraphicsCommandList(), m_List->GetD3D12GraphicsCommandList() };
			m_Queue->ExecuteCommandLists(1, lists);

			m_NumCommands = 0;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::CommitDefferedBarriers() const
		{
			m_List->GetD3D12GraphicsCommandList()->ResourceBarrier(static_cast<uint32>(m_DefferedBarriers.size()), m_DefferedBarriers.data());
			m_DefferedBarriers.clear();

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_RESOURCE_BARRIER DX12DeviceContext::CreateTransitionBarrier(DX12Resource* pResource, D3D12_RESOURCE_STATES from, D3D12_RESOURCE_STATES to, int32 subresource) const
		{
			ID3D12Resource* pD3D12Resource = pResource->GetD3D12Resource();

			D3D12_RESOURCE_BARRIER barrier = {};
			barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
			barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
			barrier.Transition.Subresource = subresource;
			barrier.Transition.pResource = pD3D12Resource;
			barrier.Transition.StateBefore = from;
			barrier.Transition.StateAfter = to;

			return barrier;
		}
	}
}

#endif