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
		IReferenceCounter * DX12RenderTargetView::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12RenderTargetView::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12RenderTargetView::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12RenderTargetView::Create(const IDevice* pDevice, const RenderTargetViewInfo& info)
		{
			//TODO: Use a Desc

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.Resource)->GetD3D12Resource();
			pD3D12Device->CreateRenderTargetView(pD3D12Resource, nullptr, m_ViewHandle);
		}
	}
}