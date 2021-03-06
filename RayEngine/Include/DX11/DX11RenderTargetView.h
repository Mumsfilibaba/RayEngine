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
#include <Graphics\IRenderTargetView.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class DX11Device;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11RenderTargetView final : public IRenderTargetView
		{
			RE_IMPLEMENT_INTERFACE(DX11RenderTargetView);

		public:
			DX11RenderTargetView(IDevice* pDevice, const RenderTargetViewDesc* pDesc);
			~DX11RenderTargetView();

			inline ID3D11RenderTargetView* GetD3D11RenderTargetView() const
			{
				return m_View;
			}

			void GetDesc(RenderTargetViewDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const RenderTargetViewDesc* pDesc);

		private:
			DX11Device* m_Device;
			ID3D11RenderTargetView* m_View;

			RenderTargetViewDesc m_Desc;

			CounterType m_References;
		};
	}
}

#endif