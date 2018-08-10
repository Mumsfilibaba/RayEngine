#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

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
			const DX12DescriptorHeap* pHeap = m_Device->GetDX12DepthStencilViewHeap();
			m_View = pHeap->GetNext();

			//TODO: More than texture2D
			D3D12_DEPTH_STENCIL_VIEW_DESC desc = {};
			desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			pD3D12Device->CreateDepthStencilView(pD3D12Resource, nullptr, m_View);
		}
	}
}

#endif