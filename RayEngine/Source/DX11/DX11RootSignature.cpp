#include "..\..\Include\DX11\DX11RootSignature.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11RootLayout::DX11RootLayout(IDevice* pDevice, const RootSignatureInfo& info)
			: m_Device(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11RootLayout::~DX11RootLayout()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11RootLayout::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void DX11RootLayout::Create(IDevice* pDevice, const RootSignatureInfo& info)
		{
		}
	}
}

#endif