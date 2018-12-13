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
#include <Graphics\ISampler.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Sampler final : public ISampler
		{
			RE_IMPLEMENT_INTERFACE(DX11Sampler);

		public:
			DX11Sampler(IDevice* pDevice, const SamplerDesc* pDesc);
			~DX11Sampler();

			inline ID3D11SamplerState* GetD3D11SamplerState() const
			{
				return m_SamplerState;
			}

			void GetDesc(SamplerDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const SamplerDesc* pDesc);

		private:
			DX11Device* m_Device;
			ID3D11SamplerState* m_SamplerState;

			SamplerDesc m_Desc;

			CounterType m_References;
		};
	}
}

#endif