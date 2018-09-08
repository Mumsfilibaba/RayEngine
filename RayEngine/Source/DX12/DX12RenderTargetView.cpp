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

#include "..\..\Include\DX12\DX12RenderTargetView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info)
			: DX12View(),
			m_Device(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(info);
		}

		

		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::~DX12RenderTargetView()
		{
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::SetName(const std::string& name)
		{
			//Not relevant for now
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::QueryDevice(IDevice ** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12RenderTargetView::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12RenderTargetView::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX12RenderTargetView::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::Create(const RenderTargetViewInfo& info)
		{
			m_Resource = info.pResource->QueryReference<DX12Resource>();
			ID3D12Resource* pD3D12Resource = m_Resource->GetD3D12Resource();
			
			const DX12DescriptorHeap* pHeap = m_Device->GetDX12RenderTargetViewHeap();
			m_View = pHeap->GetNext(m_Resource);


			D3D12_RENDER_TARGET_VIEW_DESC desc = {};
			desc.Format = ReToDXFormat(info.Format);

			if (info.ViewDimension == VIEWDIMENSION_BUFFER)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement = info.Buffer.StartElement;
				desc.Buffer.NumElements = info.Buffer.ElementCount;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = info.Texture1D.MipSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = info.Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = info.Texture2D.MipSlice;
				desc.Texture2D.PlaneSlice = info.Texture2D.PlaneSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMS;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = info.Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;
				desc.Texture2DArray.PlaneSlice = info.Texture2DArray.PlaneSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = info.Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = info.Texture2DMSArray.FirstArraySlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE3D)
			{
				desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE3D;
				desc.Texture3D.MipSlice = info.Texture3D.MipSlice;
				desc.Texture3D.FirstWSlice = info.Texture3D.FirstDepthSlice;
				desc.Texture3D.WSize = info.Texture3D.DepthSliceCount;
			}

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			pD3D12Device->CreateRenderTargetView(pD3D12Resource, &desc, m_View.CpuDescriptor);
		}
	}
}

#endif