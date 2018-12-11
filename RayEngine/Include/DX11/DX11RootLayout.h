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
#include "RayEngine.h"
#include "Graphics\IRootLayout.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device;
		class IDX11RootVariableSlot;
		class DX11ShaderConstantBlock;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class RAYENGINE_API DX11RootLayout : public IRootLayout
		{
			RE_IMPLEMENT_INTERFACE(DX11RootLayout);

		public:
			DX11RootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc);
			~DX11RootLayout();

			inline IDX11RootVariableSlot* GetDX11VariableSlot(int32 slotIndex) const
			{
				return m_VariableSlots[slotIndex];
			}

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(RootLayoutDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const RootLayoutDesc* pDesc);
			
			ID3D11SamplerState* CreateStaticSampler(const StaticSamplerDesc* pDesc);
			
			IDX11RootVariableSlot* CreateVariable(const ShaderVariableDesc* pDesc);
			
			DX11ShaderConstantBlock* CreateConstantBlock(const ShaderVariableDesc* pDesc);

		private:
			DX11Device* m_Device;

			std::vector<DX11ShaderConstantBlock*> m_ConstantBlocks;
			std::vector<ID3D11SamplerState*> m_StaticSamplers;
			std::vector<IDX11RootVariableSlot*> m_VariableSlots;

			RootLayoutDesc m_Desc;

			IObject::CounterType m_References;
		};
	}
}

#endif