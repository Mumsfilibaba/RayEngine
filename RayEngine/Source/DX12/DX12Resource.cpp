#include "..\..\Include\DX12\DX12Resource.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource()
			: m_Resource(nullptr),
			m_UploadResource(nullptr),
			m_State()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(ID3D12Device* device, D3D12_CLEAR_VALUE* clearValue,
			const D3D12_RESOURCE_DESC& desc, RESOURCE_USAGE usage)
			: m_Resource(nullptr),
			m_UploadResource(nullptr),
			m_State()
		{
			Create(device, clearValue, desc, usage);
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(ID3D12Resource* resource)
			: m_Resource(resource),
			m_UploadResource(nullptr),
			m_State()
		{
			resource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(const DX12Resource& other)
			: m_Resource(nullptr),
			m_UploadResource(nullptr)
		{
			other.m_Resource->AddRef();
			other.m_UploadResource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(DX12Resource&& other)
			: m_Resource(other.m_Resource),
			m_UploadResource(other.m_UploadResource)
		{
			other.m_Resource = nullptr;
			other.m_UploadResource = nullptr;
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::~DX12Resource()
		{
			D3DRelease_S(m_Resource);
			D3DRelease_S(m_UploadResource);
		}



		/////////////////////////////////////////////////////////////
		void DX12Resource::GetDesc(D3D12_RESOURCE_DESC& desc)
		{
			desc = m_Resource->GetDesc();
		}




		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12Resource::GetResource() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_STATES DX12Resource::GetState() const
		{
			return m_State;
		}



		/////////////////////////////////////////////////////////////
		DX12Resource& DX12Resource::operator=(const DX12Resource& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Resource);
				D3DRelease_S(m_UploadResource);

				other.m_Resource->AddRef();
				m_Resource = other.m_Resource;

				other.m_UploadResource->AddRef();
				m_UploadResource = other.m_UploadResource;

				m_State = other.m_State;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		DX12Resource& DX12Resource::operator=(DX12Resource&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Resource);
				D3DRelease_S(m_UploadResource);

				m_Resource = other.m_Resource;
				m_UploadResource = other.m_UploadResource;

				other.m_Resource = nullptr;
				other.m_UploadResource = nullptr;

				m_State = other.m_State;
			}

			return *this;
		}



		void DX12Resource::Create(ID3D12Device* device, D3D12_CLEAR_VALUE* clearValue, 
			const D3D12_RESOURCE_DESC& desc, RESOURCE_USAGE usage)
		{
			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
			heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProp.CreationNodeMask = 1;
			heapProp.VisibleNodeMask = 1;
			heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;


			if (usage == RESOURCE_USAGE_DEFAULT || usage == RESOURCE_USAGE_STATIC)
			{
				if (FAILED(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc,
					D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_UploadResource))))
				{
					return;
				}


				heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			}


			if (FAILED(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATE_COMMON, clearValue, IID_PPV_ARGS(&m_Resource))))
			{
				return;
			}


			m_State = D3D12_RESOURCE_STATE_COMMON;
		}



		/////////////////////////////////////////////////////////////
		DX12Resource DX12Resource::CreateTexture1D(ID3D12Device* device, uint64 width, uint32 arraySize, uint32 sampleCount,
			uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, DXGI_FORMAT format, RESOURCE_USAGE usage)
		{
			return Create(device, nullptr, D3D12_RESOURCE_DIMENSION_TEXTURE1D, width, 1, arraySize, 
				sampleCount, sampleQuality, D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT,
				flags, miplevels, format, usage);
		}



		/////////////////////////////////////////////////////////////
		DX12Resource DX12Resource::CreateTexture2D(ID3D12Device* device, uint64 width, uint32 height,
			uint32 arraySize, uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags, 
			uint16 miplevels, DXGI_FORMAT format, RESOURCE_USAGE usage)
		{
			return Create(device, nullptr, D3D12_RESOURCE_DIMENSION_TEXTURE2D, width, height, arraySize, sampleCount,
				sampleQuality, D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, flags, 
				miplevels, format, usage);
		}



		/////////////////////////////////////////////////////////////
		DX12Resource DX12Resource::CreateTexture3D(ID3D12Device* device, uint64 width, uint32 height,
			uint32 depth, uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags,
			uint16 miplevels, DXGI_FORMAT format, RESOURCE_USAGE usage)
		{
			return Create(device, nullptr, D3D12_RESOURCE_DIMENSION_TEXTURE2D, width, height, depth, sampleCount,
				sampleQuality, D3D12_TEXTURE_LAYOUT_UNKNOWN, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, flags,
				miplevels, format, usage);
		}



		/////////////////////////////////////////////////////////////
		DX12Resource DX12Resource::CreateBuffer(ID3D12Device* device, uint32 count, uint32 stride,
			D3D12_RESOURCE_FLAGS flags, RESOURCE_USAGE usage)
		{
			return Create(device, nullptr, D3D12_RESOURCE_DIMENSION_BUFFER, stride * count, 1, 1, 1, 0,
				D3D12_TEXTURE_LAYOUT_ROW_MAJOR, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, flags, 1,
				DXGI_FORMAT_UNKNOWN, usage);
		}



		/////////////////////////////////////////////////////////////
		DX12Resource DX12Resource::Create(ID3D12Device* device, D3D12_CLEAR_VALUE* clearValue, D3D12_RESOURCE_DIMENSION dimension, uint64 width,
			uint32 height, uint16 depthOrArraySize, uint32 sampleCount, uint32 sampleQuality, D3D12_TEXTURE_LAYOUT layout,
			uint32 alignment, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, DXGI_FORMAT format, RESOURCE_USAGE usage)
		{
			using namespace Microsoft::WRL;

			D3D12_RESOURCE_DESC desc = {};
			desc.Alignment = alignment;
			desc.DepthOrArraySize = depthOrArraySize;
			desc.Dimension = dimension;
			desc.Flags = flags;
			desc.Height = height;
			desc.Width = width;
			desc.Layout = layout;
			desc.MipLevels = miplevels;
			desc.SampleDesc.Count = sampleCount;
			desc.SampleDesc.Quality = sampleQuality;

			return DX12Resource(device, clearValue, desc, usage);
		}
	}
}