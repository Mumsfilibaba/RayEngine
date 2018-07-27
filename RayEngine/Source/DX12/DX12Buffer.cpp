#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Buffer.h"


namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
			: m_Device(nullptr),
			m_Resource(nullptr),
			m_ReferenceCounter(0),
			m_BufferType(BUFFER_USAGE_UNKNOWN),
			m_State(D3D12_RESOURCE_STATE_COMMON)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());
			Create(pDevice, pInitalData, info);
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::DX12Buffer(DX12Buffer&& other)
			: m_Device(other.m_Device),
			m_Resource(other.m_Resource),
			m_ReferenceCounter(other.m_ReferenceCounter),
			m_BufferType(other.m_BufferType),
			m_State(other.m_State)
		{
			other.m_Device = nullptr;
			other.m_Resource = nullptr;
			other.m_ReferenceCounter = 0;
			other.m_BufferType = BUFFER_USAGE_UNKNOWN;
			other.m_State = D3D12_RESOURCE_STATE_COMMON;
		}



		/////////////////////////////////////////////////////////////
		DX12Buffer::~DX12Buffer()
		{
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		void* DX12Buffer::Map(int32 subresource)
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::Unmap()
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
		IDevice* DX12Buffer::GetDevice() const
		{
			return m_Device;
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
		DX12Buffer& DX12Buffer::operator=(DX12Buffer&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Resource);
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_Resource = other.m_Resource;
				m_ReferenceCounter = other.m_ReferenceCounter;
				m_BufferType = other.m_BufferType;
				m_State = other.m_State;


				other.m_Device = nullptr;
				other.m_Resource = nullptr;
				other.m_ReferenceCounter = 0;
				other.m_BufferType = BUFFER_USAGE_UNKNOWN;
				other.m_State = D3D12_RESOURCE_STATE_COMMON;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12Buffer::GetD3D12Resource() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_STATES DX12Buffer::GetD3D12State() const
		{
			return m_State;
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::SetD3D12State(D3D12_RESOURCE_STATES state)
		{
			m_State = state;
		}



		/////////////////////////////////////////////////////////////
		void DX12Buffer::Create(IDevice* pDevice, const ResourceData* pInitalData, const BufferInfo& info)
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

			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProp.CreationNodeMask = 1;
			heapProp.VisibleNodeMask = 1;

			if (info.Usage == RESOURCE_USAGE_DEFAULT || info.Usage == RESOURCE_USAGE_STATIC)
				heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			else if (info.Usage == RESOURCE_USAGE_DYNAMIC)
				heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


			if (FAILED(pD3D12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&m_Resource))))
			{
				return;
			}
			else
			{
				D3D12SetName(m_Resource, info.Name);
			}


			m_State = D3D12_RESOURCE_STATE_COMMON;
			CreateView(pDevice, info);


			if (pInitalData != nullptr)
			{
				//TODO: Should be changed later to take different CPU_ACCESS into account??

				DX12Resource uploadBuffer(pDevice, (info.Name + ": UploadBuffer"), nullptr, desc,
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
		void DX12Buffer::CreateView(IDevice* pDevice, const BufferInfo& info)
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