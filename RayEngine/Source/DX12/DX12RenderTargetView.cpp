#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(IDevice* pDevice, const RenderTargetViewInfo& info)
			: m_Device(nullptr),
			m_View(),
			m_ReferenceCount(0)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice);

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::DX12RenderTargetView(DX12RenderTargetView&& other)
			: m_Device(other.m_Device),
			m_View(other.m_View),
			m_ReferenceCount(other.m_ReferenceCount)
		{
			other.m_Device = nullptr;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView::~DX12RenderTargetView()
		{
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		DX12RenderTargetView& DX12RenderTargetView::operator=(DX12RenderTargetView&& other)
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
		D3D12_CPU_DESCRIPTOR_HANDLE DX12RenderTargetView::GetD3D12CpuDescriptorHandle() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX12RenderTargetView::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX12RenderTargetView::QueryReference()
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
		void DX12RenderTargetView::Create(IDevice* pDevice, const RenderTargetViewInfo& info)
		{
			const DX12DescriptorHeap* pHeap = reinterpret_cast<DX12Device*>(pDevice)->GetDX12RenderTargetViewHeap();
			m_View = pHeap->GetNext();

			//TODO: Use a Desc

			ID3D12Device* pD3D12Device = reinterpret_cast<const DX12Device*>(pDevice)->GetD3D12Device();
			ID3D12Resource* pD3D12Resource = reinterpret_cast<const DX12Texture*>(info.pResource)->GetD3D12Resource();
			pD3D12Device->CreateRenderTargetView(pD3D12Resource, nullptr, m_View);
		}
	}
}

#endif