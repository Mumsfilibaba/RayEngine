#pragma once

#include "..\Graphics\ISampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Device;



		/////////////////////////////////////////////////////////////
		class DX12Sampler final : public ISampler
		{
		public:
			DX12Sampler(const DX12Sampler& other) = delete;
			DX12Sampler& operator=(const DX12Sampler& other) = delete;
			DX12Sampler(DX12Sampler&& other) = delete;
			DX12Sampler& operator=(DX12Sampler&& other) = delete;

		public:
			DX12Sampler(IDevice* pDevice, const SamplerInfo& info);
			~DX12Sampler();

			D3D12_CPU_DESCRIPTOR_HANDLE  GetD3D12CpuDescriptorHandle() const;
			D3D12_GPU_DESCRIPTOR_HANDLE  GetD3D12GpuDescriptorHandle() const;

			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const SamplerInfo& info);

		private:
			DX12Device* m_Device;
			DX12DescriptorHandle m_SamplerState;
		};
	}
}

#endif