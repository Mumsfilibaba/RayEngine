#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(const IDevice* pDevice, const RenderTargetViewInfo& info)
			: DX12ViewBase(reinterpret_cast<const DX12Device*>(pDevice)->GetDX12RenderTargetViewHeap())
		{
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(DX12RenderTargetView&& other)
			: DX12ViewBase(std::move(other))
		{
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::~DX12RenderTargetView()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView& DX12RenderTargetView::operator=(DX12RenderTargetView&& other)
		{
			DX12ViewBase::operator=(std::move(other));
			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::Create(const IDevice* pDevice, const RenderTargetViewInfo& info)
		{
			//TODO: Use a Desc

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.Resource)->GetResource().GetD3D12Resource();
			pD3D12Device->CreateRenderTargetView(pD3D12Resource, nullptr, m_ViewHandle);
		}
	}
}