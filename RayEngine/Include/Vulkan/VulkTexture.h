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
#include <Graphics\ITexture.h>
#include "VulkCommon.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class IDevice;
		class VulkDevice;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class VulkTexture final : public ITexture
		{
			RE_IMPLEMENT_INTERFACE(VulkTexture);

		public:
			VulkTexture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureDesc* pDesc);
			VulkTexture(IDevice* pDevice, VkImage image);
			~VulkTexture();

			inline VkImage GetVkImage() const
			{
				return m_Image;
			}

			void InvalidateResource();

			void GetDesc(TextureDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const ResourceData* const pInitialData, const TextureDesc* pDesc);

		private:
			VulkDevice* m_Device;
			
			VkImage m_Image;
			
			TextureDesc m_Desc;

			CounterType m_References;
		};
	}
}