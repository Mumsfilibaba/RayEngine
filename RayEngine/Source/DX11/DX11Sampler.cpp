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

#include "RayEngine.h"
#include "DX11/DX11Sampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11/DX11Device.h"
#include "DX11/DX11Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Sampler::DX11Sampler(IDevice* pDevice, const SamplerDesc* pDesc)
			: m_Device(nullptr),
			m_SamplerState(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX11Device*>(pDevice);
			
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Sampler::~DX11Sampler()
		{
			D3DRelease_S(m_SamplerState);
		}
		

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Sampler::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX11Device>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Sampler::GetDesc(SamplerDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Sampler::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Sampler::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Sampler::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Sampler::Create(const SamplerDesc* pDesc)
		{
			D3D11_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX11TextureAdressMode(pDesc->AdressU);
			desc.AddressV = ReToDX11TextureAdressMode(pDesc->AdressV);
			desc.AddressW = ReToDX11TextureAdressMode(pDesc->AdressW);
			desc.ComparisonFunc = ReToDX11ComparisonFunc(pDesc->ComparisonFunc);
			desc.Filter = ReToDX11Filter(pDesc->FilterMode);
			desc.MaxAnisotropy = pDesc->MaxAnistropy;
			desc.MinLOD = pDesc->MinLOD;
			desc.MaxLOD = pDesc->MaxLOD;
			desc.MipLODBias = pDesc->MipLODBias;
			desc.BorderColor[0] = pDesc->BorderColor.R;
			desc.BorderColor[1] = pDesc->BorderColor.G;
			desc.BorderColor[2] = pDesc->BorderColor.B;
			desc.BorderColor[3] = pDesc->BorderColor.A;

			ID3D11Device* pD3D11Device = m_Device->GetD3D11Device();
			HRESULT hr = pD3D11Device->CreateSamplerState(&desc, &m_SamplerState);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create SamplerState. " + DXErrorString(hr));
			}
			else
			{
				m_Desc = *pDesc;
			}
		}
	}
}

#endif