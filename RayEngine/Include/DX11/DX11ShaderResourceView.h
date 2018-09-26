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
#include "..\Graphics\IShaderResourceView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;



		/////////////////////////////////////////////////////////////
		class DX11ShaderResourceView final : public IShaderResourceView
		{
			RE_IMPLEMENT_INTERFACE(DX11ShaderResourceView);

		public:
			DX11ShaderResourceView(IDevice* pDevice, const ShaderResourceViewInfo& info);
			~DX11ShaderResourceView();

			ID3D11ShaderResourceView* GetD3D11ShaderResourceView() const;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const ShaderResourceViewInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11ShaderResourceView* m_View;
			
			IObject::CounterType m_References;
		};
	}
}

#endif