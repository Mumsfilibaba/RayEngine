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
#include <vector>
#include "..\Graphics\ISwapchain.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Factory;
		class DX12Texture;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Swapchain final : public ISwapchain
		{
			RE_IMPLEMENT_INTERFACE(DX12Swapchain);

		public:
			DX12Swapchain(IDevice* pDevice, const SwapchainDesc* pDesc, HWND hwnd);
			~DX12Swapchain();

			void Resize(int32 width, int32 height) override final;

			void Present() const override final;

			void SetName(const std::string& name) override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(SwapchainDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const SwapchainDesc* pDesc, HWND hwnd);
			
			void CreateTextures(const SwapchainDesc* pDesc);

		private:
			DX12Device* m_Device;
			DX12DeviceContext* m_Context;
			IDXGISwapChain1* m_Swapchain;
			
			std::vector<DX12Texture*> m_Textures;
			
			SwapchainDesc m_Desc;

			mutable int32 m_CurrentBuffer;

			IObject::CounterType m_References;
		};
	}
}

#endif