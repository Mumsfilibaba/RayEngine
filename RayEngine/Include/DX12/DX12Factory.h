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

		public:
			DX12Factory(bool debugLayer);
			~DX12Factory();

			void EnumerateAdapters(AdapterInfo** adapters, int32& count) const override final;

			bool CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const override final;
			bool CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo,
				ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const override final;

			void DestroySwapchain(const IDevice* const device, ISwapchain** swapchain) const override final;

			GRAPHICS_API GetGraphicsApi() const override final;

		private:
			void Create(bool debugLayer);

		private:
			ID3D12Debug* m_DebugController;
			IDXGIFactory5* m_Factory;
		};
	}
}