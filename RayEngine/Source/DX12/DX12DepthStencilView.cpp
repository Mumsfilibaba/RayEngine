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
#include <DX12/DX12DepthStencilView.h>
#include <DX12/DX12Device.h>
#include <DX12/DX12Texture.h>
#include <DX12/DX12DescriptorHeap.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(IDevice* pDevice, const DepthStencilViewDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);
			
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12DepthStencilView::~DX12DepthStencilView()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DepthStencilView::GetDesc(DepthStencilViewDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}

		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DepthStencilView::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12DepthStencilView::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Create(const DepthStencilViewDesc* pDesc)
		{
			ID3D12Resource* pD3D12Resource = nullptr;
			if (pDesc->pResource != nullptr)
			{
				m_Resource = dynamic_cast<DX12Resource*>(pDesc->pResource);
				pD3D12Resource = m_Resource->GetD3D12Resource();
			}

			D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(pDesc->Format);

			desc.Flags = D3D12_DSV_FLAG_NONE;
			if (pDesc->Flags & DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_STENCIL)
				desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_STENCIL;
			if (pDesc->Flags & DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_DEPTH)
				desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_DEPTH;
			
			if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = pDesc->Texture1D.MipSlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = pDesc->Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = pDesc->Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = pDesc->Texture1DArray.FirstArraySlice;
			}
			if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = pDesc->Texture2D.MipSlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = pDesc->Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = pDesc->Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = pDesc->Texture2DArray.FirstArraySlice;
			}
			if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = pDesc->Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = pDesc->Texture2DMSArray.FirstArraySlice;
			}

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			m_Descriptor = m_Device->CreateDepthStencilDescriptorHandle();

			pD3D12Device->CreateDepthStencilView(pD3D12Resource, &desc, m_Descriptor.CpuDescriptor);
			if (pD3D12Resource == nullptr)
			{
				LOG_WARNING("D3D12: Created an empty DepthStencilView-Descriptor");
			}

			m_Desc = *pDesc;
		}
	}
}

#endif