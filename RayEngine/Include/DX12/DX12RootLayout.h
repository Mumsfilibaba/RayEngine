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
#include <Graphics\IRootLayout.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class DX12Device;
		class DX12RootVariableSlot;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12RootLayout : public IRootLayout
		{
			RE_IMPLEMENT_INTERFACE(DX12RootLayout);

		public:
			DX12RootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc);
			~DX12RootLayout();

			inline ID3D12RootSignature* GetD3D12RootSignature() const
			{
				return m_RootSignature.Get();
			}
			
			inline DX12RootVariableSlot* GetDX12RootVariableSlot(int32 index) const
			{
				return m_VariableSlots[index];
			}

			inline DX12RootVariableSlot* const * GetDX12RootVariableSlotArray(int32 index) const
			{
				return m_VariableSlots.data();
			}
			
			inline int32 GetDX12RootVariableSlotCount() const
			{
				return static_cast<int32>(m_VariableSlots.size());
			}

			void GetDesc(RootLayoutDesc* pDesc) const override final;

			CounterType Release() override final;

			CounterType AddRef() override final;


		private:
			void Create(const RootLayoutDesc* pDesc);

			D3D12_ROOT_PARAMETER1 CreateVariable(const ShaderVariableDesc* pVariable);

			D3D12_STATIC_SAMPLER_DESC CreateSampler(const StaticSamplerDesc* pSampler);

			DX12RootVariableSlot* CreateRootVariableSlot(const ShaderVariableDesc* pVariable, int32 rootSlot, bool placeDescriptorTable);

		private:
			DX12Device* m_Device;
			Microsoft::WRL::ComPtr<ID3D12RootSignature> m_RootSignature;

			RootLayoutDesc m_Desc;

			std::vector<DX12RootVariableSlot*> m_VariableSlots;

			CounterType m_References;
		};
	}
}

#endif