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
#include <Graphics\IDepthStencilView.h>

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
		class DX11DepthStencilView final : public IDepthStencilView
		{
			RE_IMPLEMENT_INTERFACE(DX11DepthStencilView);

		public:
			DX11DepthStencilView(IDevice* pDevice, const DepthStencilViewDesc* pDesc);
			~DX11DepthStencilView();

			inline ID3D11DepthStencilView* GetD3D11DepthStencilView() const
			{
				return m_View;
			}

			void GetDesc(DepthStencilViewDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const DepthStencilViewDesc* pDesc);

		private:
			DX11Device* m_Device;
			ID3D11DepthStencilView* m_View;

			DepthStencilViewDesc m_Desc;

			CounterType m_References;
		};
	}
}

#endif