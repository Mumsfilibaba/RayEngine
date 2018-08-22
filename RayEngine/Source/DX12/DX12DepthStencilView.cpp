#include "..\..\Include\DX12\DX12DepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(IDevice* pDevice, const DepthStencilViewInfo& info)
			: DX12View(),
			m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);
			
			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::~DX12DepthStencilView()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Create(const DepthStencilViewInfo& info)
		{
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Resource*>(info.pResource)->GetD3D12Resource();

			const DX12DescriptorHeap* pHeap = m_Device->GetDX12DepthStencilViewHeap();
			m_View = pHeap->GetNext();
			m_View.GpuResourceAdress = pD3D12Resource->GetGPUVirtualAddress();

			//TODO: More than texture2D
			D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.Flags = D3D12_DSV_FLAG_NONE;
			if (info.Flags & DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_STENCIL)
				desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_STENCIL;
			if (info.Flags & DEPTH_STENCIL_VIEW_FLAGS_READ_ONLY_DEPTH)
				desc.Flags |= D3D12_DSV_FLAG_READ_ONLY_DEPTH;
			
			if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1D;
				desc.Texture1D.MipSlice = info.Texture1D.MipSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE1D_ARRAY)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE1DARRAY;
				desc.Texture1DArray.MipSlice = info.Texture1DArray.MipSlice;
				desc.Texture1DArray.ArraySize = info.Texture1DArray.ArraySize;
				desc.Texture1DArray.FirstArraySlice = info.Texture1DArray.FirstArraySlice;
			}
			if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MipSlice = info.Texture2D.MipSlice;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2D_ARRAY)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MipSlice = info.Texture2DArray.MipSlice;
				desc.Texture2DArray.ArraySize = info.Texture2DArray.ArraySize;
				desc.Texture2DArray.FirstArraySlice = info.Texture2DArray.FirstArraySlice;
			}
			if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMS;
			}
			else if (info.ViewDimension == VIEWDIMENSION_TEXTURE2DMS_ARRAY)
			{
				desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2DMSARRAY;
				desc.Texture2DMSArray.ArraySize = info.Texture2DMSArray.ArraySize;
				desc.Texture2DMSArray.FirstArraySlice = info.Texture2DMSArray.FirstArraySlice;
			}

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			pD3D12Device->CreateDepthStencilView(pD3D12Resource, nullptr, m_View.CpuDescriptor);
		}
	}
}

#endif