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
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class IDevice;

		class VulkRootLayout final : public IRootLayout
		{
			RE_IMPLEMENT_INTERFACE(VulkRootLayout);

		public:
			VulkRootLayout(IDevice* pDevice, const RootLayoutDesc* pDesc);
			~VulkRootLayout();

			inline VkPipelineLayout GetVkPipelineLayout() const
			{
				return m_Layout;
			}

			void GetDesc(RootLayoutDesc* pDesc) const override final;
						
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(IDevice* pDevice, const RootLayoutDesc* pDesc);

		private:
			IDevice* m_Device;
			
			VkPipelineLayout m_Layout;

			RootLayoutDesc m_Desc;
			
			CounterType m_References;
		};
	}
}