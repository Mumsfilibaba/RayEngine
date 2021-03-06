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
#include "VulkImageView.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IDevice;

		class VulkRenderTargetView final : public IRenderTargetView, public VulkImageView
		{
			RE_IMPLEMENT_INTERFACE(VulkRenderTargetView);

		public:
			VulkRenderTargetView(IDevice* pDevice, const RenderTargetViewDesc* pDesc);
			~VulkRenderTargetView();

			void GetDesc(RenderTargetViewDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const RenderTargetViewDesc* pDesc);

		private:
			RenderTargetViewDesc m_Desc;

			CounterType m_References;
		};
	}
}