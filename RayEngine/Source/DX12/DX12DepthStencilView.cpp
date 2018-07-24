#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(const IDevice* pDevice, const DepthStencilViewInfo& info)
			: DX12ViewBase(reinterpret_cast<const DX12Device*>(pDevice)->GetDX12DepthStencilViewHeap())
		{
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(DX12DepthStencilView&& other)
			: DX12ViewBase(std::move(other))
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::~DX12DepthStencilView()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView& DX12DepthStencilView::operator=(DX12DepthStencilView&& other)
		{
			DX12ViewBase::operator=(std::move(other));
			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Create(const IDevice* pDevice, const DepthStencilViewInfo& info)
		{
			//TODO: Use a Desc

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.Resource)->GetResource().GetD3D12Resource();
			pD3D12Device->CreateDepthStencilView(pD3D12Resource, nullptr, m_ViewHandle);
		}
	}
}