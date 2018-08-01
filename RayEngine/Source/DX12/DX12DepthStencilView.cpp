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
			: m_Device(nullptr),
			m_ReferenceCount(0),
			m_View()
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice);
			
			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::DX12DepthStencilView(DX12DepthStencilView&& other)
			: m_Device(other.m_Device),
			m_ReferenceCount(other.m_ReferenceCount),
			m_View(other.m_View)
		{
			other.m_Device = nullptr;
			other.m_ReferenceCount = 0;
			other.m_View.ptr = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView::~DX12DepthStencilView()
		{
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12DepthStencilView::GetD3D12CpuDescriptorHandle() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		DX12DepthStencilView& DX12DepthStencilView::operator=(DX12DepthStencilView&& other)
		{
			if (this != &other)
			{
				if (m_Device != nullptr)
				{
					m_Device->Release();
					m_Device = nullptr;
				}


				m_Device = other.m_Device;
				m_ReferenceCount = other.m_ReferenceCount;
				m_View = other.m_View;


				other.m_Device = nullptr;
				other.m_ReferenceCount = 0;
				other.m_View.ptr = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12DepthStencilView::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12DepthStencilView::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12DepthStencilView::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX12DepthStencilView::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX12DepthStencilView::Create(IDevice* pDevice, const DepthStencilViewInfo& info)
		{
			const DX12DescriptorHeap* pHeap = reinterpret_cast<DX12Device*>(pDevice)->GetDX12DepthStencilViewHeap();
			m_View= pHeap->GetNext();
			
			//TODO: Use a Desc

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			pD3D12Device->CreateDepthStencilView(pD3D12Resource, nullptr, m_View);
		}
	}
}

#endif