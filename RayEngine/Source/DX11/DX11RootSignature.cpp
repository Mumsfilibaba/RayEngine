#include "..\..\Include\DX11\DX11RootSignature.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11RootSignature::DX11RootSignature(IDevice* pDevice, const RootSignatureInfo& info)
			: m_Device(nullptr),
			m_ReferenceCount(0)
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
		IReferenceCounter* DX11RootSignature::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11RootSignature::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11RootSignature::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11RootSignature::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount++;;
		}



		/////////////////////////////////////////////////////////////
		void DX11RootSignature::Create(IDevice* pDevice, const RootSignatureInfo& info)
		{
		}
	}
}

#endif