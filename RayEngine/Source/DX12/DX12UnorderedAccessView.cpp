#include "..\..\Include\DX12\DX12UnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12UnorderedAccessView::DX12UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewInfo& info)
			: m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12UnorderedAccessView::~DX12UnorderedAccessView()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::QueryDevice(IDevice ** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12UnorderedAccessView::Create(const UnorderedAccessViewInfo& info)
		{
			DX12DescriptorHeap* pDX12Heap = m_Device->GetDX12SamplerHeap();
			m_View = pDX12Heap->GetNext();

			//TODO: More than just Texture2D
			D3D12_UNORDERED_ACCESS_VIEW_DESC desc = {};
			desc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;
			desc.Texture2D.PlaneSlice = 0;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			//TODO: Look up pCounterResource
			pD3D12Device->CreateUnorderedAccessView(pD3D12Resource, nullptr, &desc, m_View.Cpu);
		}
	}
}

#endif