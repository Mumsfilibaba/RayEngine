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
#include "..\Graphics\IDevice.h"
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkImageView
		{
		public:
			VulkImageView(const VulkImageView& other) = delete;
			VulkImageView& operator=(const VulkImageView& other) = delete;
			VulkImageView(VulkImageView&& other) = delete;
			VulkImageView& operator=(VulkImageView&& other) = delete;

		protected:
			VulkImageView(IDevice* pDevice);
			~VulkImageView();

		public:
			VkImageView GetVkImageView() const;

		protected:
			IDevice* m_Device;
			
			VkImageView m_View;
		};
	}
}