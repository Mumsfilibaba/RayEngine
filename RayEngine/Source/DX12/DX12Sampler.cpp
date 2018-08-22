#include "..\..\Include\DX12\DX12Sampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12DescriptorHeap.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Sampler::DX12Sampler(IDevice* pDevice, const SamplerInfo& info)
			: m_Device(nullptr)
		{
			AddRef();
			m_Device = QueryDX12Device(pDevice);

			Create(info);
		}



		/////////////////////////////////////////////////////////////
		DX12Sampler::~DX12Sampler()
		{
			ReRelease_S(m_Device);
		}



		/////////////////////////////////////////////////////////////
		D3D12_CPU_DESCRIPTOR_HANDLE DX12Sampler::GetD3D12CpuDescriptorHandle() const
		{
			return m_SamplerState.CpuDescriptor;
		}



		/////////////////////////////////////////////////////////////
		D3D12_GPU_DESCRIPTOR_HANDLE DX12Sampler::GetD3D12GpuDescriptorHandle() const
		{
			return m_SamplerState.GpuDescriptor;
		}



		/////////////////////////////////////////////////////////////
		void DX12Sampler::QueryDevice(IDevice** ppDevice) const
		{
			(*ppDevice) = QueryDX12Device(m_Device);
		}



		/////////////////////////////////////////////////////////////
		void DX12Sampler::Create(const SamplerInfo& info)
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