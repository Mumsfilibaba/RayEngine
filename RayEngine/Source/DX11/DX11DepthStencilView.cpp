#include "..\..\Include\DX11\DX11DepthStencilView.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::DX11DepthStencilView(IDevice * pDevice, const DepthStencilViewInfo & info)
		{
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::DX11DepthStencilView(DX11DepthStencilView && other)
		{
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView::~DX11DepthStencilView()
		{
		}



		/////////////////////////////////////////////////////////////
		ID3D11DepthStencilView* DX11DepthStencilView::GetD3D11DepthStencilView() const
		{
			return m_View;
		}



		/////////////////////////////////////////////////////////////
		DX11DepthStencilView & DX11DepthStencilView::operator=(DX11DepthStencilView && other)
		{
			// TODO: insert return statement here
		}



		/////////////////////////////////////////////////////////////
		IDevice * DX11DepthStencilView::GetDevice() const
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter * DX11DepthStencilView::QueryReference()
		{
			return nullptr;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11DepthStencilView::GetReferenceCount() const
		{
			return uint32();
		}



		/////////////////////////////////////////////////////////////
		void DX11DepthStencilView::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11DepthStencilView::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11DepthStencilView::Create(IDevice* pDevice, const DepthStencilViewInfo& info)
		{
		}
	}
}

#endif