#include "..\..\Include\DX12\DX12Resource.h"
#include <sstream>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource()
			: m_Resource(nullptr),
			m_State()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(ID3D12Device* device, const std::string& name, D3D12_CLEAR_VALUE* clearValue,
			const D3D12_RESOURCE_DESC& desc, D3D12_RESOURCE_STATES initalState, RESOURCE_USAGE usage, CPU_ACCESS_FLAG cpuAccess)
			: m_Resource(nullptr),
			m_State()
		{
			Create(device, name, clearValue, desc, initalState, usage, cpuAccess);
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(ID3D12Resource* resource)
			: m_Resource(resource),
			m_State()
		{
			resource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(const DX12Resource& other)
			: m_Resource(other.m_Resource),
			m_State(other.m_State)
		{
			other.m_Resource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::DX12Resource(DX12Resource&& other)
			: m_Resource(other.m_Resource),
			m_State(other.m_State)
		{
			other.m_Resource = nullptr;
			other.m_State = D3D12_RESOURCE_STATE_COMMON;
		}



		/////////////////////////////////////////////////////////////
		DX12Resource::~DX12Resource()
		{
			D3DRelease_S(m_Resource);
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

				other.m_Resource->AddRef();
				m_Resource = other.m_Resource;

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

				m_Resource = other.m_Resource;
				m_State = other.m_State;
				
				other.m_Resource = nullptr;
				other.m_State = D3D12_RESOURCE_STATE_COMMON;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12Resource::Create(ID3D12Device* device, const std::string& name, D3D12_CLEAR_VALUE* clearValue, const D3D12_RESOURCE_DESC& desc,
			D3D12_RESOURCE_STATES initalState, RESOURCE_USAGE usage, CPU_ACCESS_FLAG cpuAccess)
		{
			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProp.CreationNodeMask = 1;
			heapProp.VisibleNodeMask = 1;

			if (usage == RESOURCE_USAGE_DEFAULT || usage == RESOURCE_USAGE_STATIC)
				heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			else if (usage == RESOURCE_USAGE_DYNAMIC)
				heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


			if (FAILED(device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc,
				initalState, clearValue, IID_PPV_ARGS(&m_Resource))))
			{
				return;
			}
			else
			{
				std::wstringstream wName;
				wName << name.c_str();
				
				m_Resource->SetName(wName.str().c_str());
			}


			m_State = initalState;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_DESC DX12Resource::CreateDescTexture1D(uint64 width, uint32 arraySize, uint32 sampleCount, 
			uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, DXGI_FORMAT format)
		{
			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
			desc.Width = width;
			desc.Height = 1;
			desc.DepthOrArraySize = arraySize;
			desc.SampleDesc.Count = sampleCount;
			desc.SampleDesc.Quality = sampleQuality;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = flags;
			desc.MipLevels = miplevels;
			desc.Format = format;

			return desc;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_DESC DX12Resource::CreateDescTexture2D(uint64 width, uint32 height, uint32 arraySize, 
			uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, DXGI_FORMAT format)
		{
			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			desc.Width = width;
			desc.Height = height;
			desc.DepthOrArraySize = arraySize;
			desc.SampleDesc.Count = sampleCount;
			desc.SampleDesc.Quality = sampleQuality;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = flags;
			desc.MipLevels = miplevels;
			desc.Format = format;

			return desc;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_DESC DX12Resource::CreateDescTexture3D(uint64 width, uint32 height, uint32 depth, 
			uint32 sampleCount, uint32 sampleQuality, D3D12_RESOURCE_FLAGS flags, uint16 miplevels, DXGI_FORMAT format)
		{
			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			desc.Width = width;
			desc.Height = height;
			desc.DepthOrArraySize = depth;
			desc.SampleDesc.Count = sampleCount;
			desc.SampleDesc.Quality = sampleQuality;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = flags;
			desc.MipLevels = miplevels;
			desc.Format = format;

			return desc;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_DESC DX12Resource::CreateDescBuffer(uint32 count, uint32 stride, D3D12_RESOURCE_FLAGS flags)
		{
			D3D12_RESOURCE_DESC desc = {};
			desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			desc.Width = stride * count;
			desc.Height = 1;
			desc.DepthOrArraySize = 1;
			desc.SampleDesc.Count = 1;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = flags;
			desc.MipLevels = 1;
			desc.Format = DXGI_FORMAT_UNKNOWN;

			return desc;
		}
	}
}