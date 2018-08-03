#pragma once

#include "..\Graphics\IFactory.h"
#include "DX12Common.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Factory : public IFactory
		{
		public:
			DX12Factory(const DX12Factory& other) = delete;
			DX12Factory& operator=(const DX12Factory& other) = delete;
			DX12Factory(DX12Factory&& other) = delete;
			DX12Factory& operator=(DX12Factory&& other) = delete;

		public:
			DX12Factory(bool debugLayer);
			~DX12Factory();

			IDXGIFactory5* GetDXGIFactory() const;
			
			void EnumerateAdapters(AdapterList& list) const override final;
			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) override final;
			bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateShaderCompiler(IShaderCompiler** ppCompiler) override final;

			GRAPHICS_API GetGraphicsApi() const override final;

		private:
			void Create(bool debugLayer);

		private:
			ID3D12Debug* m_DebugController;
			IDXGIFactory5* m_Factory;

		private:
			static void FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC1& desc);
		};
	}
}

#endif