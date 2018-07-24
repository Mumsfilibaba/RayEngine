#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(const IDevice* pDevice, const RenderTargetViewInfo& info)
			: m_View()
		{
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(DX12RenderTargetView&& other)
			: m_View(other.m_View)
		{
			memset(&other.m_View, 0, sizeof(other.m_View));
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::~DX12RenderTargetView()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView& DX12RenderTargetView::operator=(DX12RenderTargetView&& other)
		{
			if (this != &other)
			{
				m_View = other.m_View;
				memset(&other.m_View, 0, sizeof(other.m_View));
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::Create(const IDevice* pDevice, const RenderTargetViewInfo& info)
		{
			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			const DX12DescriptorHeap* pHeap = reinterpret_cast<const DX12Device*>(pDevice)->GetDX12RenderTargetViewHeap();

			m_View = pHeap->GetNext();

			//TODO: Use a Desc

			ID3D12Resource* resource = reinterpret_cast<const DX12Texture*>(info.Resource)->GetResource().GetD3D12Resource();
			pD3D12Device->CreateRenderTargetView(resource, nullptr, m_View);
		}
	}
}