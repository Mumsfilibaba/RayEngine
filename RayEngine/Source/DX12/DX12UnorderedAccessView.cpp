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

#include "..\..\Include\DX12\DX12UnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12UnorderedAccessView::DX12UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewInfo& info)
			: m_Device(nullptr),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(info);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12UnorderedAccessView::~DX12UnorderedAccessView()
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::SetName(const std::string& name)
		{
			//Not relevant
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::QueryDevice(IDevice ** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12UnorderedAccessView::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12UnorderedAccessView::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12UnorderedAccessView::Release()
		{
			mReferences--;
			IObject::CounterType counter = mReferences;

			if (counter < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::Create(const UnorderedAccessViewInfo& info)
		{			
			ID3D12Resource* pD3D12CounterResource = nullptr;
			if (info.pCounterResource != nullptr)
				pD3D12CounterResource = reinterpret_cast<const DX12Resource*>(info.pCounterResource)->GetD3D12Resource();

			m_Resource = info.pResource->QueryReference<DX12Resource>();
			ID3D12Resource* pD3D12Resource = m_Resource->GetD3D12Resource();

			DX12DescriptorHeap* pDX12Heap = m_Device->GetDX12SamplerHeap();
			m_View = pDX12Heap->GetNext(m_Resource);

			
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);

			if (info.ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
				desc.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;

				if (info.Flags & UNORDERED_ACCESS_VIEW_FLAG_BUFFER_RAW)
					desc.Buffer.Flags |= D3D12_BUFFER_UAV_FLAG_RAW;

				desc.Buffer.FirstElement = info.Buffer.StartElement;
				desc.Buffer.NumElements = info.Buffer.ElementCount;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = info.Texture1D.MipSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = info.Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = info.Texture2D.MipSlice;
				desc.Texture2D.PlaneSlice = info.Texture2D.PlaneSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = info.Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;
				desc.Texture2DArray.PlaneSlice = info.Texture2DArray.FirstArraySlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = info.Texture3D.MipSlice;
				desc.Texture3D.FirstWSlice = info.Texture3D.FirstDepthSlice;
				desc.Texture3D.WSize = info.Texture3D.DepthSliceCount;
			}

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			pD3D12Device->CreateUnorderedAccessView(pD3D12Resource, pD3D12CounterResource, &desc, m_View.CpuDescriptor);
		}
	}
}

#endif