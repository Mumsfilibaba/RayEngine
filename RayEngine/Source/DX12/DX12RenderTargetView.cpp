#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(ID3D12Device* device, DX12DescriptorHeap& heap, const RenderTargetViewInfo& info)
			: m_View()
		{
			Create(device, heap, info);
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
		void DX12RenderTargetView::Create(ID3D12Device* device, DX12DescriptorHeap& heap,
			const RenderTargetViewInfo& info)
		{
			m_View = heap.GetNext();

			//TODO: Use a Desc

			ID3D12Resource* resource = reinterpret_cast<const DX12Texture*>(info.Resource)->GetResource();
			device->CreateRenderTargetView(resource, nullptr, m_View);
		}
	}
}