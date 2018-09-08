/*////////////////////////////////////////////////////////////

Copyright 2018 Alexander Dahlin

Licensed under the Apache License, Version 2.0 (the
"License"); you may not use this file except in
compliance with the License. You may obtain a copy of
the License at

http ://www.apache.org/licenses/LICENSE-2.0

THIS SOFTWARE IS PROVIDED "AS IS". MEANING NO WARRANTY
OR SUPPORT IS PROVIDED OF ANY KIND.

In event of any damages, direct or indirect that can
be traced back to the use of this software, shall no
contributor be held liable. This includes computer
failure and or malfunction of any kind.

////////////////////////////////////////////////////////////*/

#include "..\..\Include\DX11\DX11Sampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Sampler::DX11Sampler(IDevice* pDevice, const SamplerInfo& info)
			: m_Device(nullptr),
			m_SamplerState(nullptr),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);
			
			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX11Sampler::~DX11Sampler()
		{
			D3DRelease_S(m_SamplerState);
		}



		/////////////////////////////////////////////////////////////
		ID3D11SamplerState* DX11Sampler::GetD3D11SamplerState() const
		{
			return m_SamplerState;
		}



		/////////////////////////////////////////////////////////////
		void DX11Sampler::SetName(const std::string& name)
		{
			m_SamplerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
		}



		/////////////////////////////////////////////////////////////
		void DX11Sampler::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11Sampler::GetReferenceCount() const
		{
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11Sampler::AddRef()
		{
			m_References++;
			return m_References;
		}



		/////////////////////////////////////////////////////////////
		IObject::CounterType DX11Sampler::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}



		/////////////////////////////////////////////////////////////
		void DX11Sampler::Create(const SamplerInfo& info)
		{
			using namespace System;

			D3D11_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX11TextureAdressMode(info.AdressU);
			desc.AddressV = ReToDX11TextureAdressMode(info.AdressV);
			desc.AddressW = ReToDX11TextureAdressMode(info.AdressW);
			desc.ComparisonFunc = ReToDX11ComparisonFunc(info.ComparisonFunc);
			desc.Filter = ReToDX11Filter(info.FilterMode);
			desc.MaxAnisotropy = info.MaxAnistropy;
			desc.MinLOD = info.MinLOD;
			desc.MaxLOD = info.MaxLOD;
			desc.MipLODBias = info.MipLODBias;
			desc.BorderColor[0] = info.BorderColor.R;
			desc.BorderColor[1] = info.BorderColor.G;
			desc.BorderColor[2] = info.BorderColor.B;
			desc.BorderColor[3] = info.BorderColor.A;


			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateSamplerState(&desc, &m_SamplerState);
			if (FAILED(hr))
			{
				m_Device->GetDeviceLog()->Write(LOG_SEVERITY_ERROR, "D3D11: Could not create SamplerState. " + DXErrorString(hr));
			}
			else
			{
				m_SamplerState->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			}
		}
	}
}

#endif