#pragma once

#include "..\Graphics\IFactory.h"
#include "DX11Common.h"

namespace RayEngine
{
	namespace Graphics
	{
		class DX11Factory : public IFactory
		{
		public:
			DX11Factory(const DX11Factory& other) = delete;
			DX11Factory& operator=(const DX11Factory& other) = delete;
			DX11Factory(DX11Factory&& other) = delete;
			DX11Factory& operator=(DX11Factory&& other) = delete;

		public:
			DX11Factory(bool debugLayer);
			~DX11Factory();

			IDXGIFactory* GetDXGIFactory() const;

			void EnumerateAdapters(AdapterList& list) const override final;
			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) override final;
			bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) override final;
			bool CreateShaderCompiler(IShaderCompiler** ppCompiler) override final;

			GRAPHICS_API GetGraphicsApi() const override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create();

		private:
			IDXGIFactory* m_Factory;

			bool m_DebugLayer;

			mutable uint32 m_ReferenceCounter;
			
		private:
			static void FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC& desc);
		};
	}
}