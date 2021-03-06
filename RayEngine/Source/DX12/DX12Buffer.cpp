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
#include <DX12/DX12Device.h>
#include <DX12/DX12Buffer.h>
#include <DX12/DX12DescriptorHeap.h>
#include <DX12/DX12DynamicUploadHeap.h>
#include <DX12/DX12DeviceContext.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferDesc* pDesc)
			: DX12Resource(),
			m_Device(nullptr),
			m_MappedSubresource(-1),
			m_Views(),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pInitalData, pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Buffer::~DX12Buffer()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void* DX12Buffer::Map(int32 subresource, RESOURCE_MAP_FLAG flag)
		{
			void* gpuPtr = nullptr;
			if (flag == RESOURCE_MAP_FLAG_READ)
			{
				m_Resource->Map(subresource, nullptr, &gpuPtr);
			}
			else if (flag == RESOURCE_MAP_FLAG_WRITE)
			{
				D3D12_RANGE range = { 0, 0 };
				m_Resource->Map(subresource, &range, &gpuPtr);
			}

			m_MappedSubresource = subresource;
			return gpuPtr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Buffer::Unmap()
		{
			m_Resource->Unmap(m_MappedSubresource, nullptr);
			m_MappedSubresource = -1;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Buffer::GetDesc(BufferDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Buffer::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Buffer::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Buffer::Create(const ResourceData* pInitalData, const BufferDesc* pDesc)
		{
			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			desc.Width = pDesc->ByteStride * pDesc->Count;
			desc.Height = 1;
			desc.DepthOrArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;

			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProp.CreationNodeMask = 1;
			heapProp.VisibleNodeMask = 1;

			if (pDesc->Usage == RESOURCE_USAGE_DEFAULT || pDesc->Usage == RESOURCE_USAGE_STATIC)
				heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			else if (pDesc->Usage == RESOURCE_USAGE_DYNAMIC)
				heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

			D3D12_RESOURCE_STATES startingState = D3D12_RESOURCE_STATE_COMMON;
			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			
			HRESULT hr = pD3D12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, startingState, nullptr, IID_PPV_ARGS(&m_Resource));
			if (FAILED(hr))
			{
				LOG_ERROR("D3D12: Could not create Buffer. " + DXErrorString(hr));
				return;
			}
			else
			{
				CreateView(pDesc);
				
				m_State = startingState;
				m_Desc = *pDesc;
			}

			if (pInitalData != nullptr)
			{
				//TODO: Should be changed later to take different CPU_ACCESS into account??

				DX12DynamicUploadHeap* uploadHeap = m_Device->GetDX12UploadHeap();
				uploadHeap->SetData(pInitalData->pData, pInitalData->ByteStride * pInitalData->WidthOrCount);

				DX12DeviceContext* pContext = nullptr;
				m_Device->GetImmediateContext(reinterpret_cast<IDeviceContext**>(&pContext));

				DX12Resource* resources[] = { this, uploadHeap };
				D3D12_RESOURCE_STATES states[] = { D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COPY_SOURCE };
				int32 subresoures[] = { 0, 0 };

				pContext->TransitionResourceGroup(resources, states, subresoures, 2);
				pContext->CopyResource(this, uploadHeap);

				ReRelease_S(pContext);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Buffer::CreateView(const BufferDesc* pDesc)
		{
			if (pDesc->Usage == BUFFER_USAGE_CONSTANT || pDesc->Usage == BUFFER_USAGE_SHADER_RESOURCE || pDesc->Usage == BUFFER_USAGE_UNORDERED_ACCESS)
			{
				m_Views.SRV_CBV_UAV.Descriptor = m_Device->CreateResourceDescriptorHandle();
				m_Views.SRV_CBV_UAV.GpuVirtualAdress = m_Resource->GetGPUVirtualAddress();
			}
			else if (pDesc->Usage == BUFFER_USAGE_VERTEX)
			{
				m_Views.Vertex.BufferLocation = GetD3D12Resource()->GetGPUVirtualAddress();
				m_Views.Vertex.StrideInBytes = pDesc->ByteStride;
				m_Views.Vertex.SizeInBytes = pDesc->ByteStride * pDesc->Count;
			}
			else if (pDesc->Usage == BUFFER_USAGE_INDEX)
			{
				m_Views.Index.BufferLocation = GetD3D12Resource()->GetGPUVirtualAddress();
				m_Views.Index.Format = DXGI_FORMAT_R32_FLOAT;
				m_Views.Index.SizeInBytes = pDesc->ByteStride * pDesc->Usage;
			}
		}
	}
}

#endif