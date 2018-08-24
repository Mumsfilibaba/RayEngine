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
#include "..\Graphics\IUnorderedAccessView.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX11Device;



		/////////////////////////////////////////////////////////////
		class DX11UnorderedAccessView final : public IUnorderedAccessView
		{
		public:
			DX11UnorderedAccessView(const DX11UnorderedAccessView& other) = delete;
			DX11UnorderedAccessView& operator=(const DX11UnorderedAccessView& other) = delete;
			DX11UnorderedAccessView(DX11UnorderedAccessView&& other) = delete;
			DX11UnorderedAccessView& operator=(DX11UnorderedAccessView&& other) = delete;

		public:
			DX11UnorderedAccessView(IDevice* pDevice, const UnorderedAccessViewInfo& info);
			~DX11UnorderedAccessView();

			ID3D11UnorderedAccessView* GetD3D11UnorderedAccessView() const;
			
			void SetName(const std::string& name) override final;
			void QueryDevice(IDevice** ppDevice) const override final;

		private:
			void Create(const UnorderedAccessViewInfo& info);

		private:
			DX11Device* m_Device;
			ID3D11UnorderedAccessView* m_View;
		};
	}
}

#endif