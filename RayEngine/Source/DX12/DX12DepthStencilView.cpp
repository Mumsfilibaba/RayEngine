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
			m_Device = reinterpret_cast<IDevice*>(pDevice);
			
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::~DX12DepthStencilView()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12DepthStencilView::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Create(IDevice* pDevice, const DepthStencilViewInfo& info)
		{
			const DX12DescriptorHeap* pHeap = reinterpret_cast<DX12Device*>(pDevice)->GetDX12DepthStencilViewHeap();
			m_View = pHeap->GetNext();
			
			//TODO: Use a Desc

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			pD3D12Device->CreateDepthStencilView(pD3D12Resource, nullptr, m_View);
		}
	}
}

#endif