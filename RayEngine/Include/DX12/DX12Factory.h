#pragma once

#include "..\Graphics\IFactory.h"
#include "DX12Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX12Factory : public IFactory
		{
		public:
			DX12Factory(const DX12Factory& other) = delete;
			DX12Factory& operator=(const DX12Factory& other) = delete;

		public:
			DX12Factory(bool debugLayer);
			DX12Factory(DX12Factory&& other);
			~DX12Factory();

			void EnumerateAdapters(AdapterList& list) const override final;

			bool CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const override final;
			bool CreateSwapchain(ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const override final;
			bool CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo,
				ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const override final;
			bool CreateShaderCompiler(IShaderCompiler** compiler) const override final;

			GRAPHICS_API GetGraphicsApi() const override final;

			DX12Factory& operator=(DX12Factory&& other);

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