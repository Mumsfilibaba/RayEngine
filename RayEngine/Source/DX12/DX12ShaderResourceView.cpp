#include "..\..\Include\DX12\DX12ShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12ShaderResourceView::DX12ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info)
			: DX12View(),
			m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12ShaderResourceView::~DX12ShaderResourceView()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12ShaderResourceView::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12ShaderResourceView::Create(const ShaderResourceViewInfo& info)
		{
			const DX12DescriptorHeap* pDX12Heap = m_Device->GetDX12ResourceHeap();
			m_View = pDX12Heap->GetNext();

			//TODO: More than texture2D
			D3D12_SHADER_RESOURCE_VIEW_DESC desc = {};
			desc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;
			desc.Texture2D.PlaneSlice = 0;
			desc.Texture2D.ResourceMinLODClamp = 0;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			pD3D12Device->CreateShaderResourceView(pD3D12Resource, nullptr, m_View.Cpu);
		}
	}
}

#endif