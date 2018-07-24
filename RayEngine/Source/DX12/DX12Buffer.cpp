#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Buffer.h"


namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
			: m_Buffer(),
			m_UploadBuffer(),
			m_BufferType(BUFFER_USAGE_UNKNOWN)
		{
			Create(pDevice, pInitalData, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::~DX12Buffer()
		{
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12Buffer::GetD3D12CpuDescriptorHandle() const
		{
			return m_Views.Constant;
		}



		/////////////////////////////////////////////////////////////
		D3D12_VERTEX_BUFFER_VIEW DX12Buffer::GetD3D12VertexBufferView() const
		{
			return m_Views.Vertex;
		}



		/////////////////////////////////////////////////////////////
		D3D12_INDEX_BUFFER_VIEW DX12Buffer::GetD3D12IndexBufferView() const
		{
			return m_Views.Index;
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::Create(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
		{
			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			const DX12CommandQueue* pQueue = reinterpret_cast<const DX12Device*>(pDevice)->GetDX12CommandQueue();


			D3D12_RESOURCE_DESC desc = DX12Resource::CreateDescBuffer(info.Count, info.Stride, D3D12_RESOURCE_FLAG_NONE);
			m_Buffer = DX12Resource(pD3D12Device, info.Name, nullptr, desc, D3D12_RESOURCE_STATE_COMMON, info.Usage, info.CpuAccess);
			
			CreateView(pDevice, info);


			if (pInitalData != nullptr)
			{
				//TODO: Should be changed later to take different CPU_ACCESS into account??

				m_UploadBuffer = DX12Resource(pD3D12Device, (info.Name + ": UploadBuffer"), nullptr, desc,
					D3D12_RESOURCE_STATE_GENERIC_READ, RESOURCE_USAGE_DYNAMIC, CPU_ACCESS_FLAG_WRITE);


				void* gpuData = m_UploadBuffer.Map(0);
				memcpy(gpuData, pInitalData->pData, pInitalData->ByteStride * pInitalData->WidthOrCount);
				m_UploadBuffer.Unmap();


				pQueue->Reset();

				pQueue->TransitionResource(m_Buffer, D3D12_RESOURCE_STATE_COPY_DEST, 0);
				pQueue->CopyResource(m_Buffer, m_UploadBuffer);
				pQueue->TransitionResource(m_Buffer, D3D12_RESOURCE_STATE_GENERIC_READ, 0);

				pQueue->Close();
				pQueue->Execute();
				pQueue->Flush();
			}

			return;
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::CreateView(const IDevice* pDevice, const BufferInfo& info)
		{
			const DX12Device* pDX12Device = reinterpret_cast<const DX12Device*>(pDevice);

			if (info.Usage == BUFFER_USAGE_UNIFORM)
			{
				m_Views.Constant = pDX12Device->GetDX12ResourceHeap()->GetNext();
			}
			else if (info.Usage == BUFFER_USAGE_VERTEX)
			{
				m_Views.Vertex.BufferLocation = m_Buffer.GetD3D12Resource()->GetGPUVirtualAddress();
				m_Views.Vertex.StrideInBytes = info.Stride;
				m_Views.Vertex.SizeInBytes = info.Stride * info.Count;
			}
			else if (info.Usage == BUFFER_USAGE_INDEX)
			{
				m_Views.Index.BufferLocation = m_Buffer.GetD3D12Resource()->GetGPUVirtualAddress();
				m_Views.Index.Format = DXGI_FORMAT_R32_FLOAT;
				m_Views.Index.SizeInBytes = info.Stride * info.Usage;
			}
		}
	}
}