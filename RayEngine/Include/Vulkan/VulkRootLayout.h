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
#include "..\Graphics\IRootLayout.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkRootLayout : public IRootLayout
		{
		public:
			VulkRootLayout(const VulkRootLayout& other) = delete;
			VulkRootLayout& operator=(const VulkRootLayout& other) = delete;
			VulkRootLayout(VulkRootLayout&& other) = delete;
			VulkRootLayout& operator=(VulkRootLayout&& other) = delete;

		public:
			VulkRootLayout(IDevice* pDevice, const RootLayoutInfo& info);
			~VulkRootLayout();

			VkPipelineLayout GetVkPipelineLayout() const;

			void SetName(const std::string& name) override final;
			IDevice* GetDevice() const override final;

		private:
			void Create(IDevice* pDevice, const RootLayoutInfo& info);

		private:
			IDevice* m_Device;
			VkPipelineLayout m_Layout;
		};
	}
}