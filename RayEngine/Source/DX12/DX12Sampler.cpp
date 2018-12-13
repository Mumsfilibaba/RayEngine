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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX12/DX12Device.h>
#include <DX12/DX12Sampler.h>
#include <DX12/DX12DescriptorHeap.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Sampler::DX12Sampler(IDevice* pDevice, const SamplerDesc* pDesc)
			: m_Device(nullptr),
			m_Descriptor(),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Sampler::~DX12Sampler()
		{
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Sampler::GetDesc(SamplerDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Sampler::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Sampler::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Sampler::Create(const SamplerDesc* pDesc)
		{
			D3D12_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX12TextureAdressMode(pDesc->AdressU);
			desc.AddressV = ReToDX12TextureAdressMode(pDesc->AdressV);
			desc.AddressW = ReToDX12TextureAdressMode(pDesc->AdressW);
			desc.ComparisonFunc = ReToDX12ComparisonFunc(pDesc->ComparisonFunc);
			desc.Filter = ReToDX12Filter(pDesc->FilterMode);
			desc.MaxAnisotropy = pDesc->MaxAnistropy;
			desc.MipLODBias = pDesc->MipLODBias;
			desc.MinLOD = pDesc->MinLOD;
			desc.MaxLOD = pDesc->MaxLOD;
			desc.BorderColor[0] = pDesc->BorderColor.R;
			desc.BorderColor[1] = pDesc->BorderColor.G;
			desc.BorderColor[2] = pDesc->BorderColor.B;
			desc.BorderColor[3] = pDesc->BorderColor.A;

			m_Device->CreateSamplerDescriptorHandle(desc, m_Descriptor);

			m_Desc = *pDesc;
		}
	}
}

#endif