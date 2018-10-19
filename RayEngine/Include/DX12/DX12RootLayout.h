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
#include <vector>
#include "..\Graphics\IRootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;
		class DX12RootVariableSlot;



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12RootLayout : public IRootLayout
		{
			RE_IMPLEMENT_INTERFACE(DX12RootLayout);

		public:
			DX12RootLayout(IDevice* pDevice, const RootLayoutInfo& info);
			~DX12RootLayout();

			ID3D12RootSignature* GetD3D12RootSignature() const;
			
			DX12RootVariableSlot* GetDX12RootVariableSlot(int32 index) const;
			
			DX12RootVariableSlot* const * GetDX12RootVariableSlotArray(int32 index) const;
			
			int32 GetDX12RootVariableSlotCount() const;

			void SetName(const std::string& name) override final;

			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;

			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;


		private:
			void Create(const RootLayoutInfo& info);

			D3D12_ROOT_PARAMETER1 CreateVariable(const ShaderVariable& variable);

			D3D12_STATIC_SAMPLER_DESC CreateSampler(const StaticSampler& sampler);

			DX12RootVariableSlot* CreateRootVariableSlot(const ShaderVariable& variable, int32 rootSlot, bool placeDescriptorTable);


		private:
			DX12Device* m_Device;
			ID3D12RootSignature* m_RootSignature;

			std::vector<DX12RootVariableSlot*> m_VariableSlots;

			IObject::CounterType mReferences;
		};
	}
}

#endif