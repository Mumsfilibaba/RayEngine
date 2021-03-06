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
#include <Graphics\IDevice.h>
#include <Graphics\ITexture.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Resource.h"
#include "DX12Device.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Texture final : public DX12Resource, public ITexture
		{
			RE_IMPLEMENT_INTERFACE(DX12Texture);

		public:
			DX12Texture(IDevice* pDevice, const ResourceData* const pInitialData, const TextureDesc* pDesc);
			DX12Texture(IDevice* pDevice, ID3D12Resource* pResource);
			~DX12Texture();

			void GetDesc(TextureDesc* pDesc) const override final;

			CounterType Release() override final;

			CounterType AddRef() override final;

		private:
			void Create(const ResourceData* const pInitialData, const TextureDesc* pDesc);

		private:
			DX12Device* m_Device;

			TextureDesc m_Desc;

			CounterType m_References;
		};
	}
}

#endif