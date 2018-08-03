#include "..\..\Include\DX11\DX11RootSignature.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11RootSignature::DX11RootSignature(IDevice* pDevice, const RootSignatureInfo& info)
			: m_Device(nullptr)
		{
			AddRef();
			m_Device = reinterpret_cast<IDevice*>(pDevice->QueryReference());

			Create(pDevice, info);
		}



		/////////////////////////////////////////////////////////////
		DX11RootSignature::~DX11RootSignature()
		{
			if (m_Device != nullptr)
			{
				m_Device->Release();
				m_Device = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		IDevice* DX11RootSignature::GetDevice() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		void DX11RootSignature::Create(IDevice* pDevice, const RootSignatureInfo& info)
		{
		}
	}
}

#endif