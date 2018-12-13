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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <Graphics/Viewport.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Buffer.h>
#include <DX12/DX12Sampler.h>
#include <DX12/DX12Resource.h>
#include <DX12/DX12Swapchain.h>
#include <DX12/DX12RootLayout.h>
#include <DX12/DX12CommandList.h>
#include <DX12/DX12PipelineState.h>
#include <DX12/DX12DeviceContext.h>
#include <DX12/DX12RootVariableSlot.h>
#include <DX12/DX12RendertargetView.h>
#include <DX12/DX12DepthStencilView.h>
#include <DX12/DX12ShaderResourceView.h>
#include <DX12/DX12UnorderedAccessView.h>

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
			m_CurrentSwapchain(nullptr),
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
			pResource->SetD3D12State(to);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::TransitionResourceGroup(DX12Resource* const * ppResource, D3D12_RESOURCE_STATES* pToStates, int32* pSubresources, int32 count) const
		{
			std::vector<D3D12_RESOURCE_BARRIER> barriers;
			barriers.reserve(count);
			
			for (int32 i = 0; i < count; i++)
			{
				D3D12_RESOURCE_STATES from = ppResource[i]->GetD3D12State();
				if (from == pToStates[i])
					continue;

				barriers.push_back(CreateTransitionBarrier(ppResource[i], from, pToStates[i], pSubresources[i]));
				ppResource[i]->SetD3D12State(pToStates[i]);
			}

			uint32 barrierCount = static_cast<uint32>(barriers.size());
			m_List->GetD3D12GraphicsCommandList()->ResourceBarrier(barrierCount, barriers.data());

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
			DX12RenderTargetView* pDX12View = nullptr;
			if (pView == nullptr)
				pDX12View = m_CurrentSwapchain->GetCurrentDX12RenderTargetView();
			else
				pDX12View = reinterpret_cast<DX12RenderTargetView*>(pView);
			
			//TODO: Subresource may not be zero

			TransitionResource(pDX12View->GetDX12Resource(), D3D12_RESOURCE_STATE_RENDER_TARGET, 0);
			m_List->GetD3D12GraphicsCommandList()->ClearRenderTargetView(pDX12View->GetD3D12CpuDescriptorHandle(), pColor, 0, nullptr);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ClearDepthStencilView(IDepthStencilView* pView, float depth, uint8 stencil) const
		{
			DX12DepthStencilView* pDX12View = nullptr;
			if (pView == nullptr)
				pDX12View = m_CurrentSwapchain->GetDX12DepthStencilView();
			else
				pDX12View = reinterpret_cast<DX12DepthStencilView*>(pView);

			//TODO: Subresource may not be zero

			TransitionResource(pDX12View->GetDX12Resource(), D3D12_RESOURCE_STATE_DEPTH_WRITE, 0);
			m_List->GetD3D12GraphicsCommandList()->ClearDepthStencilView(pDX12View->GetD3D12CpuDescriptorHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, depth, stencil, 0, nullptr);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetSwapChain(ISwapchain* pSwapChain) const
		{
			ReRelease_S(m_CurrentSwapchain);
			m_CurrentSwapchain = pSwapChain->QueryReference<DX12Swapchain>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetDX12Rendertargets(DX12RenderTargetView** ppRenderTargets, int32 count, DX12DepthStencilView* pDepthStencil) const 
		{
			int32 pSubResources[9];
			memset(pSubResources, 0, sizeof(pSubResources));

			DX12Resource* ppResources[9];
			D3D12_RESOURCE_STATES pToStates[9];

			int32 numResources = 0;
			D3D12_CPU_DESCRIPTOR_HANDLE pRTVs[8];
			for (int32 i = 0; i < count; i++)
			{
				if (ppRenderTargets[i] != nullptr)
				{
					pRTVs[i] = ppRenderTargets[i]->GetD3D12CpuDescriptorHandle();
					pToStates[numResources] = D3D12_RESOURCE_STATE_RENDER_TARGET;
					ppResources[numResources] = ppRenderTargets[i]->GetDX12Resource();
					numResources++;
				}
				else
				{
					pRTVs[i] = m_Device->GetEmptyRenderTargetView();
				}
			}

			D3D12_CPU_DESCRIPTOR_HANDLE dsv = { 0 };
			if (pDepthStencil != nullptr)
			{
				dsv = pDepthStencil->GetD3D12CpuDescriptorHandle();
				pToStates[numResources] = D3D12_RESOURCE_STATE_DEPTH_READ;
				ppResources[numResources] = pDepthStencil->GetDX12Resource();
				numResources++;
			}
			else
			{
				dsv = m_Device->GetEmptyDepthStencilView();
			}

			TransitionResourceGroup(ppResources, pToStates, pSubResources, numResources);
			m_List->GetD3D12GraphicsCommandList()->OMSetRenderTargets(count, pRTVs, false, &dsv);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetRendertargets(IRenderTargetView* pRenderTarget, IDepthStencilView* pDepthStencil) const
		{
			if (pRenderTarget == nullptr && pDepthStencil == nullptr)
			{
				SetDefaultFramebuffer();
			}
			else
			{
				DX12RenderTargetView* pDX12RenderTarget = reinterpret_cast<DX12RenderTargetView*>(pRenderTarget);
				DX12DepthStencilView* pDX12DepthStencil = reinterpret_cast<DX12DepthStencilView*>(pDepthStencil);
				SetDX12Rendertargets(&pDX12RenderTarget, 1, pDX12DepthStencil);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetShaderResourceViews(IShaderResourceView* pShaderResourceView, int32 startRootIndex) const
		{
			DX12ShaderResourceView* pDX12View = reinterpret_cast<DX12ShaderResourceView*>(pShaderResourceView);
			DX12DescriptorHandle srv = pDX12View->GetDX12DescriptorHandle();
			
			//TODO: Maybe the subreource is not 0

			DX12RootVariableSlot* pDX12Slot = m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex);
			TransitionResourceIndirect(pDX12View->GetDX12Resource(), pDX12Slot->GetNeededD3D12ResourceState(), 0);
			pDX12Slot->SetShaderResourceViews(m_List->GetD3D12GraphicsCommandList(), &srv, 1);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetUnorderedAccessViews(IUnorderedAccessView* pUnorderedAccessView, int32 startRootIndex) const
		{
			DX12ShaderResourceView* pDX12View = reinterpret_cast<DX12ShaderResourceView*>(pUnorderedAccessView);
			DX12DescriptorHandle uav = pDX12View->GetDX12DescriptorHandle();

			DX12RootVariableSlot* pDX12Slot = m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex);
			TransitionResourceIndirect(pDX12View->GetDX12Resource(), pDX12Slot->GetNeededD3D12ResourceState(), 0);
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
			DX12DescriptorHandle sampler;// = reinterpret_cast<DX12Sampler*>(pSampler)->Get();
			m_CurrentRootLayout->GetDX12RootVariableSlot(startRootIndex)->SetSamplers(m_List->GetD3D12GraphicsCommandList(), &sampler, 1);

			AddCommand();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetPipelineState(IPipelineState* pPipelineState) const
		{
			DX12PipelineState* pDX12PipelineState = reinterpret_cast<DX12PipelineState*>(pPipelineState);
			m_List->GetD3D12GraphicsCommandList()->SetGraphicsRootSignature(pDX12PipelineState->GetD3D12RootSignature());
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
			SetDefaultFramebuffer();
			m_List->GetD3D12GraphicsCommandList()->DrawInstanced(vertexCount, 1, startVertex, 0);
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
			ExecuteCommandLists();
			Synchronize();

			m_List->Close();
			m_List->ResetAllocator();
			ResetCommandList();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ExecuteDefferedContext(IDeviceContext* pDefferedContext) const
		{
			if (m_IsDeffered)
				return;

			DX12DeviceContext* pDX12DeviceContext = reinterpret_cast<DX12DeviceContext*>(pDefferedContext);
			
			ID3D12CommandList* lists[] = { pDX12DeviceContext->GetD3D12CommandList() };
			m_Queue->ExecuteCommandLists(1, lists);

			Flush();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DeviceContext::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DeviceContext::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::Create(bool isDeffered)
		{
			//TODO: different types of commandqueues

			D3D12_COMMAND_QUEUE_DESC qDesc = {};
			qDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
			qDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
			qDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
			qDesc.NodeMask = 0;

			HRESULT hr = 0;
			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			if (!isDeffered)
			{
				hr = pD3D12Device->CreateCommandQueue(&qDesc, IID_PPV_ARGS(&m_Queue));
				if (FAILED(hr))
				{
					LOG_ERROR("DX12: Could not create CommandQueue. " + DXErrorString(hr));
					return;
				}

				hr = pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence));
				if (FAILED(hr))
				{
					LOG_ERROR("DX12: Could not create Fence. " + DXErrorString(hr));
					return;
				}
			}
			
			m_List = new DX12CommandList(m_Device, nullptr, D3D12_COMMAND_LIST_TYPE_DIRECT, qDesc.NodeMask);
			m_List->Reset();
			m_ComputeList = new DX12CommandList(m_Device, nullptr, D3D12_COMMAND_LIST_TYPE_COMPUTE, qDesc.NodeMask);
			m_ComputeList->Reset();

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
		void DX12DeviceContext::Synchronize() const
		{
			if (m_IsDeffered)
				return;

			m_CurrentFence++;
			HRESULT hr = m_Queue->Signal(m_Fence.Get(), m_CurrentFence);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Signal fence failed. " + DXErrorString(hr));
				return;
			}

			if (m_Fence->GetCompletedValue() < m_CurrentFence)
			{
				HANDLE ev = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

				hr = m_Fence->SetEventOnCompletion(m_CurrentFence, ev);
				if (FAILED(m_Fence->SetEventOnCompletion(m_CurrentFence, ev)))
				{
					LOG_ERROR("D3D12: Failed to set event. " + DXErrorString(hr));
					return;
				}

				WaitForSingleObject(ev, INFINITE);
				CloseHandle(ev);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::ExecuteCommandLists() const
		{
			if (m_NumCommands < 1)
				return;

			if (m_List->Close())
			{
				ID3D12CommandList* list = m_List->GetD3D12CommandList();
				m_Queue->ExecuteCommandLists(1, &list);

				ResetCommandList();
				m_NumCommands = 0;
			}
			else
			{
				LOG_WARNING("D3D12: Failed to close commandlist. ");
			}
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


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DeviceContext::SetDefaultFramebuffer() const
		{
			DX12RenderTargetView* pDX12RenderTarget = m_CurrentSwapchain->GetCurrentDX12RenderTargetView();
			DX12DepthStencilView* pDX12DepthStencil = m_CurrentSwapchain->GetDX12DepthStencilView();
			SetDX12Rendertargets(&pDX12RenderTarget, 1, pDX12DepthStencil);
		}
	}
}

#endif