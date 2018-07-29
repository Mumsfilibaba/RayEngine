#include <vector>
#include "..\..\Include\DX11\DX11Factory.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Device.h"
#include "..\..\Include\DX11\DX11Swapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Factory::DX11Factory(bool debugLayer)
			: m_Factory(nullptr),
			m_ReferenceCounter(0),
			m_DebugLayer(debugLayer)
		{
			AddRef();
			Create();
		}



		/////////////////////////////////////////////////////////////
		DX11Factory::DX11Factory(DX11Factory&& other)
			: m_Factory(other.m_Factory),
			m_ReferenceCounter(other.m_ReferenceCounter),
			m_DebugLayer(other.m_DebugLayer)
		{
			other.m_Factory = nullptr;
			other.m_ReferenceCounter = 0;
			other.m_DebugLayer = false;
		}



		/////////////////////////////////////////////////////////////
		DX11Factory::~DX11Factory()
		{
			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}
		}



		/////////////////////////////////////////////////////////////
		IDXGIFactory* DX11Factory::GetDXGIFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::EnumerateAdapters(AdapterList& list) const
		{
			using namespace Microsoft::WRL;

			std::vector<AdapterInfo> infos;
			ComPtr<IDXGIAdapter> pAdapter = nullptr;
			ComPtr<ID3D11Device> pDevice = nullptr;
			ComPtr<ID3D11DeviceContext> pDeviceContext = nullptr;
			D3D_FEATURE_LEVEL featureLevel;
			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;


			for (int32 i = 0; m_Factory->EnumAdapters(i, &pAdapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				HRESULT hr = D3D11CreateDevice(pAdapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, 0, &supportedFeatureLevel, 1,
					D3D11_SDK_VERSION, &pDevice, &featureLevel, &pDeviceContext);

				if (SUCCEEDED(hr))
				{
					DXGI_ADAPTER_DESC desc = {};
					if (SUCCEEDED(pAdapter->GetDesc(&desc)))
					{
						infos.push_back(AdapterInfo());
						FillAdapterInfo(i, infos[infos.size() - 1], desc);
					}
				}
			}


			list = AdapterList(static_cast<int32>(infos.size()));
			for (int32 i = 0; i < list.Count; i++)
				list[i] = infos[i];
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateDevice(IDevice** ppDevice, const DeviceInfo& deviceInfo)
		{
			return (*ppDevice = new DX11Device(this, deviceInfo, m_DebugLayer));
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateSwapchain(ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			return ((*ppSwapchain = new DX11Swapchain(this, swapchainInfo)));
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateDeviceAndSwapchain(IDevice** ppDevice, const DeviceInfo& deviceInfo, ISwapchain** ppSwapchain, const SwapchainInfo& swapchainInfo)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		bool DX11Factory::CreateShaderCompiler(IShaderCompiler** ppCompiler)
		{
			return false;
		}



		/////////////////////////////////////////////////////////////
		GRAPHICS_API DX11Factory::GetGraphicsApi() const
		{
			return GRAPHICS_API_D3D11;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11Factory::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Factory::GetReferenceCount() const
		{
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::Release() const
		{
			m_ReferenceCounter--;
			if (m_ReferenceCounter < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Factory::AddRef()
		{
			m_ReferenceCounter++;
			return m_ReferenceCounter;
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::Create()
		{
			if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory))))
				return;

			return;
		}



		/////////////////////////////////////////////////////////////
		void DX11Factory::FillAdapterInfo(int32 adapterID, AdapterInfo& info, DXGI_ADAPTER_DESC& desc)
		{
			info.ApiID = adapterID;
			info.VendorID = desc.VendorId;
			info.DeviceID = desc.DeviceId;


			constexpr int32 len = sizeof(desc.Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, desc.Description, len);
			info.ModelName = str;

			info.VendorName = AdapterInfo::GetVendorString(desc.VendorId);


			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
			info.Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			info.Flags |= ADAPTER_FLAGS_TESSELATIONSHADERS;
			info.Flags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			info.Flags |= ADAPTER_FLAGS_COMPUTE;
			info.Flags |= ADAPTER_FLAGS_GRAPHICS;


			info.Limits.RenderTargetCount = 8;

			info.Limits.Texture1D.Width = 16384;

			info.Limits.Texture2D.Width = 16384;
			info.Limits.Texture2D.Height = 16384;

			info.Limits.Texture3D.Width = 2048;
			info.Limits.Texture3D.Height = 2048;
			info.Limits.Texture3D.Depth = 2048;

			info.Limits.TextureCube.Width = 16384;
			info.Limits.TextureCube.Height = 16384;
		}
	}
}

#endif