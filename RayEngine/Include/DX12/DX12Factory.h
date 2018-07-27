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

		public:
			DX12Factory(bool debugLayer);
			DX12Factory(DX12Factory&& other);
			~DX12Factory();

			void EnumerateAdapters(AdapterList& list) const override final;

			bool CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo) const override final;
			bool CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) const override final;
			bool CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo) const override final;
			bool CreateShaderCompiler(IShaderCompiler** ppCompiler) const override final;

			GRAPHICS_API GetGraphicsApi() const override final;

			DX12Factory& operator=(DX12Factory&& other);

			IDXGIFactory5* GetDXGIFactory() const;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(bool debugLayer);

		private:
			ID3D12Debug* m_DebugController;
			IDXGIFactory5* m_Factory;

			mutable uint32 m_ReferenceCounter;

		private:
			static void FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC1& desc);
		};
	}
}

#endif