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
			const DX12DescriptorHeap* pHeap = m_Device->GetDX12RenderTargetViewHeap();
			m_View = pHeap->GetNext();

			//TODO: More than texture2D
			D3D12_RENDER_TARGET_VIEW_DESC desc = {};
			desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Texture2D.PlaneSlice = 0;


			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			pD3D12Device->CreateRenderTargetView(pD3D12Resource, &desc, m_View);
		}
	}
}

#endif