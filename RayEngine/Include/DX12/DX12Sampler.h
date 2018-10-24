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

#pragma once
#include "..\Graphics\ISampler.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12DescriptorHandle.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Sampler final : public ISampler
		{
			RE_IMPLEMENT_INTERFACE(DX12Sampler);

		public:
			DX12Sampler(IDevice* pDevice, const SamplerDesc* pDesc);
			~DX12Sampler();

			inline D3D12_CPU_DESCRIPTOR_HANDLE  GetD3D12CpuDescriptorHandle() const
			{
				return m_SamplerState.CpuDescriptor;
			}
			
			inline D3D12_GPU_DESCRIPTOR_HANDLE  GetD3D12GpuDescriptorHandle() const
			{
				return m_SamplerState.GpuDescriptor;
			}

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(SamplerDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
	
			IObject::CounterType AddRef() override final;

		private:
			void Create(const SamplerDesc* pDesc);

		private:
			DX12Device* m_Device;

			DX12DescriptorHandle m_SamplerState;

			SamplerDesc m_Desc;

			IObject::CounterType m_References;
		};
	}
}

#endif