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

#include "RayEngine.h"
#include "../../Include/DX12/DX12UnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX12/DX12Device.h"
#include "../../Include/DX12/DX12Texture.h"
#include "../../Include/DX12/DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12UnorderedAccessView::DX12UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewDesc* pDesc)
			: m_Device(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12UnorderedAccessView::~DX12UnorderedAccessView()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::GetDesc(UnorderedAccessViewDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12UnorderedAccessView::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12UnorderedAccessView::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12UnorderedAccessView::Release()
		{
			IObject::CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::Create(const UnorderedAccessViewDesc* pDesc)
		{			
			ID3D12Resource* pD3D12CounterResource = nullptr;
			if (pDesc->pCounterResource != nullptr)
			{
				pD3D12CounterResource = dynamic_cast<DX12Resource*>(pDesc->pCounterResource)->GetD3D12Resource();
			}

			ID3D12Resource* pD3D12Resource = nullptr;
			if (pDesc->pResource != nullptr)
			{
				m_Resource = dynamic_cast<DX12Resource*>(pDesc->pResource);
				pD3D12Resource = m_Resource->GetD3D12Resource();

				m_GpuVirtualAddress = pD3D12Resource->GetGPUVirtualAddress();
			}

			D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(pDesc->Format);
			if (pDesc->ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
				desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

				if (pDesc->Flags & UNORDERED_ACCESS_VIEW_FLAG_BUFFER_RAW)
					desc.Buffer.Flags |= D3D12_BUFFER_UAV_FLAG_RAW;

				desc.Buffer.FirstElement = pDesc->Buffer.StartElement;
				desc.Buffer.NumElements = pDesc->Buffer.ElementCount;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = pDesc->Texture1D.MipSlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = pDesc->Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = pDesc->Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = pDesc->Texture1DArray.FirstArraySlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = pDesc->Texture2D.MipSlice;
				desc.Texture2D.PlaneSlice = pDesc->Texture2D.PlaneSlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = pDesc->Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = pDesc->Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = pDesc->Texture2DArray.FirstArraySlice;
				desc.Texture2DArray.PlaneSlice = pDesc->Texture2DArray.FirstArraySlice;
			}
			else if (pDesc->ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = pDesc->Texture3D.MipSlice;
				desc.Texture3D.FirstWSlice = pDesc->Texture3D.FirstDepthSlice;
				desc.Texture3D.WSize = pDesc->Texture3D.DepthSliceCount;
			}

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			m_Descriptor = m_Device->CreateResourceDescriptorHandle();

			pD3D12Device->CreateUnorderedAccessView(pD3D12Resource, pD3D12CounterResource, &desc, m_Descriptor.CpuDescriptor);
			if (pD3D12Resource == nullptr)
			{
				LOG_WARNING("D3D12: Created an empty UnorderedAccessView-Descriptor");
			}

			m_Desc = *pDesc;
		}
	}
}

#endif