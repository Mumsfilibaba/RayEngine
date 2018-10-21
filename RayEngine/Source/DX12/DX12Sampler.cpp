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


#include "..\..\Include\DX12\DX12Sampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Sampler::DX12Sampler(IDevice* pDevice, const SamplerDesc& info)
			: m_Device(nullptr),
			mReferences(0)
		{
			AddRef();
			m_Device = reinterpret_cast<DX12Device*>(pDevice);

			Create(info);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Sampler::~DX12Sampler()
		{
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12Sampler::GetD3D12CpuDescriptorHandle() const
		{
			return m_SamplerState.CpuDescriptor;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		D3D12_GPU_DESCRIPTOR_HANDLE DX12Sampler::GetD3D12GpuDescriptorHandle() const
		{
			return m_SamplerState.GpuDescriptor;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Sampler::SetName(const std::string& name)
		{
			//Not relevant
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Sampler::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = m_Device->QueryReference<DX12Device>();
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Sampler::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Sampler::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Sampler::Release()
		{
			mReferences--;
			IObject::CounterType counter = mReferences;

			if (counter < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Sampler::Create(const SamplerDesc& info)
		{
			const DX12DescriptorHeap* pDX12Heap = m_Device->GetDX12SamplerHeap();
			m_SamplerState = pDX12Heap->GetNext(nullptr);


			D3D12_SAMPLER_DESC desc = {};
			desc.AddressU = ReToDX12TextureAdressMode(info.AdressU);
			desc.AddressV = ReToDX12TextureAdressMode(info.AdressV);
			desc.AddressW = ReToDX12TextureAdressMode(info.AdressW);
			desc.ComparisonFunc = ReToDX12ComparisonFunc(info.ComparisonFunc);
			desc.Filter = ReToDX12Filter(info.FilterMode);
			desc.MaxAnisotropy = info.MaxAnistropy;
			desc.MipLODBias = info.MipLODBias;
			desc.MinLOD = info.MinLOD;
			desc.MaxLOD = info.MaxLOD;
			desc.BorderColor[0] = info.BorderColor.R;
			desc.BorderColor[1] = info.BorderColor.G;
			desc.BorderColor[2] = info.BorderColor.B;
			desc.BorderColor[3] = info.BorderColor.A;

			ID3D12Device* pD3D12Device = m_Device->GetD3D12Device();
			pD3D12Device->CreateSampler(&desc, m_SamplerState.CpuDescriptor);
		}
	}
}

#endif