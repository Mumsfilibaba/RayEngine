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
#include <Graphics\ISwapchain.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Texture.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Texture;
		class DX12RenderTargetView;
		class DX12DepthStencilView;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Swapchain final : public ISwapchain
		{
			RE_IMPLEMENT_INTERFACE(DX12Swapchain);

		public:
			DX12Swapchain(IDevice* pDevice, const SwapchainDesc* pDesc, HWND hwnd);
			~DX12Swapchain();
			
			inline DX12Texture* GetCurrentDX12Buffer() const
			{
				return m_Textures[m_CurrentBuffer];
			}

			inline DX12DepthStencilView* GetDX12DepthStencilView() const
			{
				return m_DSV;
			}

			inline DX12RenderTargetView* GetCurrentDX12RenderTargetView() const
			{
				return (m_UseMSAA) ? m_RTVs[0] : m_RTVs[m_CurrentBuffer];
			}

			void Resize(int32 width, int32 height) override final;

			void Present() const override final;
			
			void QueryDevice(IDevice** ppDevice) const override final;

			void GetDesc(SwapchainDesc* pDesc) const override final;
			
			CounterType Release() override final;
			
			CounterType AddRef() override final;

		private:
			void Create(const SwapchainDesc* pDesc, HWND hwnd);
			
			void CreateTextures();

			void CreateViews();

			void ReleaseResources();

		private:
			DX12Device* m_Device;
			DX12DeviceContext* m_Context;
			Microsoft::WRL::ComPtr<IDXGISwapChain3> m_Swapchain;
			DX12Texture* m_MSAABackBuffer;
			DX12Texture* m_DepthStencil;
			DX12DepthStencilView* m_DSV;

			std::vector<DX12Texture*> m_Textures;
			std::vector<DX12RenderTargetView*> m_RTVs;

			SwapchainDesc m_Desc;

			DXGI_FORMAT m_Format;

			mutable int32 m_CurrentBuffer;

			bool m_UseMSAA;

			CounterType m_References;
		};
	}
}

#endif