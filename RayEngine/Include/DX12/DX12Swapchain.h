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
#include "..\Graphics\ISwapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		class DX12Factory;
		class DX12Texture;



		/////////////////////////////////////////////////////////////
		class DX12Swapchain final : public ISwapchain
		{
		public:
			DX12Swapchain(const DX12Swapchain& other) = delete;
			DX12Swapchain& operator=(const DX12Swapchain& other) = delete;
			DX12Swapchain(DX12Swapchain&& other) = delete;
			DX12Swapchain& operator=(DX12Swapchain&& other) = delete;

		public:
			DX12Swapchain(IFactory* pFactory, IDevice* pDevice, const SwapchainInfo& info);
			~DX12Swapchain();

			void Resize(int32 width, int32 height) override final;

			void Present() const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;
			
			void QueryFactory(IFactory** ppFactory) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const SwapchainInfo& info);
			
			void CreateTextures(const SwapchainInfo& info);

		private:
			DX12Factory* m_Factory;
			DX12Device* m_Device;
			DX12DeviceContext* m_Context;
			IDXGISwapChain1* m_Swapchain;
			
			std::vector<DX12Texture*> m_Textures;
			
			mutable int32 m_CurrentBuffer;

			IObject::CounterType m_References;
		};
	}
}

#endif