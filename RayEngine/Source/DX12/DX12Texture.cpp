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
			: DX12Resource(),
			m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(pInitialData, info);
		}




		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(IDevice* pDevice, ID3D12Resource* pResource)
			: DX12Resource(), 
			m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			m_Resource = pResource;
			pResource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		RESOURCE_STATE DX12Texture::GetResourceState() const
		{
			return DXToREResourceState(GetD3D12State());
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::Create(const ResourceData* const pInitialData, const TextureInfo& info)
		{
			using namespace System;

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
			if (info.Flags & TEXTURE_FLAGS_RENDERTARGET)
			{
				flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
				pClearValue = &clearValue;
			}

			if (info.Flags & TEXTURE_FLAGS_DEPTH_STENCIL)
			{
				flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
				pClearValue = &clearValue;
			}

			if (info.Flags & TEXTURE_FLAGS_UNORDERED_ACCESS)
			{
				flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
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


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			D3D12_RESOURCE_STATES startingState = D3D12_RESOURCE_STATE_COMMON;

			HRESULT hr = pD3D12Device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &desc, startingState, pClearValue, IID_PPV_ARGS(&m_Resource));
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D12: Could not create Texture. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_State = startingState;
				D3D12SetName(m_Resource, info.Name);
			}


			if (pInitialData != nullptr)
			{
				const DX12CommandQueue* pQueue = m_Device->GetDX12CommandQueue();
				
				DX12DynamicUploadHeap* uploadHeap = m_Device->GetDX12UploadHeap();
				uploadHeap->SetData(pInitialData->pData, pInitialData->ByteStride * pInitialData->WidthOrCount);

				pQueue->Reset();

				ID3D12Resource* pSrc = uploadHeap->GetD3D12Resource();
				pQueue->TransitionResource(GetD3D12Resource(), GetD3D12State(), D3D12_RESOURCE_STATE_COPY_DEST, 0);
				SetD3D12State(D3D12_RESOURCE_STATE_COPY_DEST);

				pQueue->CopyTextureRegion(GetD3D12Resource(), pSrc, format, info.Width, info.Height, 1, pInitialData->ByteStride);
				
				pQueue->TransitionResource(GetD3D12Resource(), GetD3D12State(), D3D12_RESOURCE_STATE_GENERIC_READ, 0);
				SetD3D12State(D3D12_RESOURCE_STATE_GENERIC_READ);

				pQueue->Close();
				pQueue->Execute();
				pQueue->Flush();
			}
		}
	}
}

#endif