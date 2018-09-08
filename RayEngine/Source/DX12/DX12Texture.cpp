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

#include <utility>
#include "..\..\Include\DX12\DX12Texture.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12DeviceContext.h"
#include "..\..\Include\DX12\DX12DynamicUploadHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureInfo& info)
			: DX12Resource(),
			m_Device(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pInitialData, info);
		}




		/////////////////////////////////////////////////////////////
		DX12Texture::DX12Texture(IDevice* pDevice, ID3D12Resource* pResource)
			: DX12Resource(), 
			m_Device(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			m_Resource = pResource;
			pResource->AddRef();
		}



		/////////////////////////////////////////////////////////////
		DX12Texture::~DX12Texture()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::SetName(const std::string& name)
		{
			D3D12SetName(m_Resource, name);
		}



		/////////////////////////////////////////////////////////////
		void DX12Texture::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12Texture::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12Texture::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12Texture::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
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
				DX12DynamicUploadHeap* uploadHeap = m_Device->GetDX12UploadHeap();
				uploadHeap->SetData(pInitialData->pData, pInitialData->ByteStride * pInitialData->WidthOrCount);

				DX12DeviceContext* pContext = nullptr;
				m_Device->GetImmediateContext(reinterpret_cast<IDeviceContext**>(&pContext));

				DX12Resource* resources[] = { this, uploadHeap };
				D3D12_RESOURCE_STATES states[] = { D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COPY_SOURCE };
				int32 subresoures[] = { 0, 0 };
			
				pContext->TransitionResourceGroup(resources, states, subresoures, 2);
				pContext->CopyTexture(this, uploadHeap, format, info.Width, info.Height, info.DepthOrArraySize, pInitialData->ByteStride);

				ReRelease_S(pContext);
			}
		}
	}
}

#endif