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
			m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(info);
		}

		

		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::~DX12RenderTargetView()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::QueryDevice(IDevice ** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::Create(const RenderTargetViewInfo& info)
		{
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Resource*>(info.pResource)->GetD3D12Resource();
			
			const DX12DescriptorHeap* pHeap = m_Device->GetDX12RenderTargetViewHeap();
			m_View = pHeap->GetNext();
			m_View.GpuResourceAdress = pD3D12Resource->GetGPUVirtualAddress();


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