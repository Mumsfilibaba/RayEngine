#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Buffer.h"


namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(const IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
			: DX12Resource(),
			m_BufferType(BUFFER_USAGE_UNKNOWN)
		{
			Create(pDevice, pInitalData, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(DX12Buffer&& other)
			: DX12Resource(std::move(other))
		{
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::~DX12Buffer()
		{
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12Buffer::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Buffer::GetReferenceCount() const
		{
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::Release() const
		{
			m_ReferenceCounter--;
			if (m_ReferenceCounter < 1)
				delete this;
		}


		
		/////////////////////////////////////////////////////////////
		uint32 DX12Buffer::AddRef()
		{
			m_ReferenceCounter++;
			return m_ReferenceCounter;
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


			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			desc.Width = info.Stride * info.Count;
			desc.Height = 1;
			desc.DepthOrArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = D3D12_RESOURCE_FLAG_NONE;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;


			DX12Resource::Create(pD3D12Device, info.Name, nullptr, desc, D3D12_RESOURCE_STATE_COMMON, info.Usage, info.CpuAccess);
			CreateView(pDevice, info);


			if (pInitalData != nullptr)
			{
				//TODO: Should be changed later to take different CPU_ACCESS into account??

				DX12Resource uploadBuffer(pD3D12Device, (info.Name + ": UploadBuffer"), nullptr, desc,
					D3D12_RESOURCE_STATE_GENERIC_READ, RESOURCE_USAGE_DYNAMIC, CPU_ACCESS_FLAG_WRITE);


				void* gpuData = uploadBuffer.Map(0);
				memcpy(gpuData, pInitalData->pData, pInitalData->ByteStride * pInitalData->WidthOrCount);
				uploadBuffer.Unmap();


				pQueue->Reset();

				pQueue->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_DEST, 0);
				pQueue->CopyResource(this, &uploadBuffer);
				pQueue->TransitionResource(this, D3D12_RESOURCE_STATE_GENERIC_READ, 0);

				pQueue->Close();
				pQueue->Execute();
				pQueue->Flush();
			}
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
				m_Views.Vertex.BufferLocation = GetD3D12Resource()->GetGPUVirtualAddress();
				m_Views.Vertex.StrideInBytes = info.Stride;
				m_Views.Vertex.SizeInBytes = info.Stride * info.Count;
			}
			else if (info.Usage == BUFFER_USAGE_INDEX)
			{
				m_Views.Index.BufferLocation = GetD3D12Resource()->GetGPUVirtualAddress();
				m_Views.Index.Format = DXGI_FORMAT_R32_FLOAT;
				m_Views.Index.SizeInBytes = info.Stride * info.Usage;
			}
		}
	}
}