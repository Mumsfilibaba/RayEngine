#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"
#include <utility>

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
			: m_Device(nullptr),
			m_Resource(nullptr),
			m_State(D3D12_RESOURCE_STATE_COMMON)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());
			Create(pDevice, pInitialData, info);
		}




		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(ID3D12Resource* pResource)
			: m_Device(nullptr),
			m_Resource(nullptr),
			m_State(D3D12_RESOURCE_STATE_COMMON)
		{
			m_Resource = pResource;
			pResource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(DX12Texture&& other)
			: m_Device(other.m_Device),
			m_Resource(other.m_Resource),
			m_State(other.m_State),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_Resource = nullptr;
			other.m_State = D3D12_RESOURCE_STATE_COMMON;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
			D3DRelease_S(m_Resource);
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		RESOURCE_STATE DX12Texture::GetResourceState() const
		{
			return DXToREResourceState(GetD3D12State());
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12Texture::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		DX12Texture& DX12Texture::operator=(DX12Texture&& other)
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
				m_State = other.m_State;
				m_ReferenceCount = other.m_ReferenceCount;


				other.m_Device = nullptr;
				other.m_Resource = nullptr;
				other.m_State = D3D12_RESOURCE_STATE_COMMON;
				other.m_ReferenceCount = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D12Resource* DX12Texture::GetD3D12Resource() const
		{
			return m_Resource;
		}



		/////////////////////////////////////////////////////////////
		D3D12_RESOURCE_STATES DX12Texture::GetD3D12State() const
		{
			return m_State;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::SetD3D12State(D3D12_RESOURCE_STATES state) const
		{
			m_State = state;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12Texture::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Texture::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12Texture::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			DXGI_FORMAT format = ReToDXFormat(info.Format);

			D3D12_CLEAR_VALUE* pClearValue = nullptr;
			D3D12_CLEAR_VALUE clearValue = {};
			clearValue.Format = format;
			clearValue.Color[0] = info.OptimizedColor[0];
			clearValue.Color[1] = info.OptimizedColor[1];
			clearValue.Color[2] = info.OptimizedColor[2];
			clearValue.Color[3] = info.OptimizedColor[3];

			clearValue.DepthStencil.Depth = info.DepthStencil.OptimizedDepth;
			clearValue.DepthStencil.Stencil = info.DepthStencil.OptimizedStencil;

			D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;
			if (info.Flags == TEXTURE_FLAGS_RENDERTARGET)
			{
				flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
				pClearValue = &clearValue;
			}
			if (info.Flags == TEXTURE_FLAGS_DEPTHBUFFER)
			{
				flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
				pClearValue = &clearValue;
			}


			D3D12_RESOURCE_DESC desc = {};
			desc.Width = info.Width;
			desc.Height = info.Height;
			desc.DepthOrArraySize = info.DepthOrArraySize;
			desc.SampleDesc.Count = info.SampleCount;
			desc.SampleDesc.Quality = 0;
			desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
			desc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
			desc.Flags = flags;
			desc.MipLevels = info.MipLevels;
			desc.Format = format;

			if (info.Type == TEXTURE_TYPE_1D)
			{
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE1D;
				desc.Height = 1;
			}
			else if (info.Type == TEXTURE_TYPE_2D)
			{
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			}
			else if (info.Type == TEXTURE_TYPE_3D)
			{
				desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE3D;
			}


			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
			heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
			heapProp.CreationNodeMask = 1;
			heapProp.VisibleNodeMask = 1;

			if (info.Usage == RESOURCE_USAGE_DEFAULT || info.Usage == RESOURCE_USAGE_STATIC)
				heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
			else if (info.Usage == RESOURCE_USAGE_DYNAMIC)
				heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;


			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			const DX12CommandQueue* pQueue = reinterpret_cast<const DX12Device*>(pDevice)->GetDX12CommandQueue();
			if (FAILED(pD3D12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc,
				D3D12_RESOURCE_STATE_COMMON, pClearValue, IID_PPV_ARGS(&m_Resource))))
			{
				return;
			}
			else
			{
				D3D12SetName(m_Resource, info.Name);
			}

			m_State = D3D12_RESOURCE_STATE_COMMON;


			if (pInitialData != nullptr)
			{
				D3D12_RESOURCE_DESC uploadDesc = {};
				uploadDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
				uploadDesc.Width = pInitialData->ByteStride * pInitialData->WidthOrCount * pInitialData->Height;
				uploadDesc.Height = 1;
				uploadDesc.DepthOrArraySize = 1;
				uploadDesc.SampleDesc.Count = 1;
				uploadDesc.SampleDesc.Quality = 0;
				uploadDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
				uploadDesc.Alignment = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
				uploadDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
				uploadDesc.MipLevels = 1;
				uploadDesc.Format = DXGI_FORMAT_UNKNOWN;

				DX12Resource uploadBuffer(pDevice, info.Name + " : UploadBuffer", nullptr, uploadDesc,
					D3D12_RESOURCE_STATE_GENERIC_READ, RESOURCE_USAGE_DYNAMIC, CPU_ACCESS_FLAG_WRITE);

				void* gpuPtr = uploadBuffer.Map(0);
				memcpy(gpuPtr, pInitialData->pData, pInitialData->ByteStride * pInitialData->WidthOrCount * pInitialData->Height);
				uploadBuffer.Unmap();

				pQueue->Reset();

				pQueue->TransitionResource(this, D3D12_RESOURCE_STATE_COPY_DEST, 0);
				pQueue->CopyTextureRegion(this, &uploadBuffer, format, info.Width, info.Height, 1, pInitialData->ByteStride);
				pQueue->TransitionResource(this, D3D12_RESOURCE_STATE_GENERIC_READ, 0);

				pQueue->Close();
				pQueue->Execute();
				pQueue->Flush();
			}
		}
	}
}

#endif