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
#include <RayEngine.h>
#include <Graphics/ISwapchain.h>

#if defined(RE_PLATFORM_WINDOWS)
#include "DX11Common.h"
#include "DX11DeviceContext.h"

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Device;
		class DX11Texture;
		class DX11RenderTargetView;
		class DX11DepthStencilView;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX11Swapchain final : public ISwapchain
		{
			RE_IMPLEMENT_INTERFACE(DX11Swapchain);

		public:
			DX11Swapchain(IDevice* pDevice, const SwapchainDesc* pDesc, HWND hwnd);
			~DX11Swapchain();

			inline DX11RenderTargetView* GetDX11RenderTargetView() const
			{
				return m_Rtv;
			}

			inline DX11DepthStencilView* GetDX11DepthStencilView() const
			{
				return m_Dsv;
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
			DX11Device* m_Device;
			IDXGISwapChain* m_Swapchain;
			ID3D11DeviceContext* m_pImmediateContext;
			DX11Texture* m_BackBuffer;
			DX11Texture* m_MSAABackBuffer;
			DX11Texture* m_DepthStencil;
			DX11RenderTargetView* m_Rtv;
			DX11DepthStencilView* m_Dsv;

			SwapchainDesc m_Desc;
			
			DXGI_FORMAT m_Format;

			uint32 m_Flags;

			bool m_UseMSAA;

			CounterType m_References;
		};
	}
}

#endif