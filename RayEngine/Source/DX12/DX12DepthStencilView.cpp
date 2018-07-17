#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(ID3D12Device* device, DX12DescriptorHeap& heap,
			const DepthStencilViewInfo& info)
			: m_View()
		{
			Create(device, heap, info);
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(DX12DepthStencilView&& other)
			: m_View(other.m_View)
		{
			memset(&other.m_View, 0, sizeof(other.m_View));
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::~DX12DepthStencilView()
		{
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView& DX12DepthStencilView::operator=(DX12DepthStencilView&& other)
		{
			if (this != &other)
			{
				m_View = other.m_View;
				memset(&other.m_View, 0, sizeof(other.m_View));
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Create(ID3D12Device* device, DX12DescriptorHeap& heap,
			const DepthStencilViewInfo& info)
		{
			m_View = heap.GetNext();

			//TODO: Use a Desc

			ID3D12Resource* resource = reinterpret_cast<const DX12Texture*>(info.Resource)->GetResource();
			device->CreateDepthStencilView(resource, nullptr, m_View);
		}
	}
}