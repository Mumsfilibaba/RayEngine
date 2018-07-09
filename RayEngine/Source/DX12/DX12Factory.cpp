#include "..\..\Include\DX12\DX12Factory.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Factory::DX12Factory(bool debugLayer)
			: m_Factory(nullptr),
			m_DebugController(nullptr)
		{
			Create(debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX12Factory::~DX12Factory()
		{
			D3DRelease_S(m_Factory);
			D3DRelease_S(m_DebugController);
		}



		/////////////////////////////////////////////////////////////
		void DX12Factory::EnumerateAdapters(AdapterInfo** adapters, int32& count) const
		{
		}



		/////////////////////////////////////////////////////////////
		bool DX12Factory::CreateDevice(IDevice** device, const DeviceInfo& deviceInfo) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool DX12Factory::CreateDeviceAndSwapchain(IDevice** device, const DeviceInfo& deviceInfo, 
			ISwapchain** swapchain, const SwapchainInfo& swapchainInfo) const
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		void DX12Factory::DestroySwapchain(const IDevice* const device, ISwapchain** swapchain) const
		{
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API DX12Factory::GetGraphicsApi() const
		{
			return GRAPHICS_API_D3D12;
		}



		/////////////////////////////////////////////////////////////
		void DX12Factory::Create(bool debugLayer)
		{
			uint32 factoryFlags = 0;
			if (debugLayer)
			{
				factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

				if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_DebugController))))
					return;

				m_DebugController->EnableDebugLayer();
			}

			if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_Factory))))
				return;

			return;
		}
	}
}

#endif