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

#include <RayEngine.h>

#if defined(RE_PLATFORM_WINDOWS)
#include <DX11/DX11Device.h>
#include <DX11/DX11DeviceContext.h>
#include <DX11/DX11RenderTargetView.h>
#include <DX11/DX11DepthStencilView.h>
#include <DX11/DX11Texture.h>
#include <DX11/DX11Shader.h>
#include <DX11/DX11Sampler.h>
#include <DX11/DX11ShaderResourceView.h>
#include <DX11/DX11UnorderedAccessView.h>
#include <DX11/DX11RootLayout.h>
#include <DX11/DX11PipelineState.h>
#include <DX11/DX11Buffer.h>
#include <d3dcommon.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Device::DX11Device(const DeviceDesc* pDesc)
			: m_Factory(nullptr),
			m_Adapter(nullptr),
			m_Device(nullptr),
			m_DebugDevice(nullptr),
			m_pImmediateContext(nullptr),
			m_FeatureLevel(),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Device::~DX11Device()
		{
			ReRelease_S(m_pImmediateContext);
			
			D3DRelease_S(m_Factory);
			D3DRelease_S(m_Adapter);
			D3DRelease_S(m_Device);

			if (m_DebugDevice != nullptr)
			{
				m_DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_SUMMARY);
				D3DRelease(m_DebugDevice);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::GetImmediateContext(IDeviceContext** ppContext)
		{
			if (ppContext == nullptr)
				return false;

			(*ppContext) = m_pImmediateContext->QueryReference<DX11DeviceContext>();
			return (*ppContext) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return ((*ppContext) = new DX11DeviceContext(this, true));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::GetDesc(DeviceDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::GetAdapterDesc(AdapterDesc* pDesc) const
		{
			using namespace Microsoft::WRL;

			DXGI_ADAPTER_DESC desc = {};
			m_Adapter->GetDesc(&desc);

			constexpr int32 len = sizeof(desc.Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, desc.Description, len);
			
			strcpy(pDesc->ModelName, str);
			strcpy(pDesc->VendorName, AdapterDesc::GetVendorString(desc.VendorId));

			pDesc->VendorID = desc.VendorId;
			pDesc->DeviceID = desc.DeviceId;

			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
			pDesc->Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			pDesc->Flags |= ADAPTER_FLAGS_TESSELATIONSHADERS;
			pDesc->Flags |= ADAPTER_FLAGS_GEOMETRYSHADER;
			pDesc->Flags |= ADAPTER_FLAGS_COMPUTE;
			pDesc->Flags |= ADAPTER_FLAGS_GRAPHICS;


			pDesc->Limits.RenderTargetCount = 8;

			pDesc->Limits.Texture1D.Width = 16384;

			pDesc->Limits.Texture2D.Width = 16384;
			pDesc->Limits.Texture2D.Height = 16384;

			pDesc->Limits.Texture3D.Width = 2048;
			pDesc->Limits.Texture3D.Height = 2048;
			pDesc->Limits.Texture3D.Depth = 2048;

			pDesc->Limits.TextureCube.Width = 16384;
			pDesc->Limits.TextureCube.Height = 16384;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11Device::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX11Device::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateShader(IShader** ppShader, const ShaderDesc* pDesc)
		{
			return ((*ppShader = new DX11Shader(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc)
		{
			return ((*ppView = new DX11RenderTargetView(this, pDesc)) != nullptr);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc)
		{
			return ((*ppView = new DX11DepthStencilView(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc)
		{
			return ((*ppView = new DX11ShaderResourceView(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc)
		{
			return ((*ppView = new DX11UnorderedAccessView(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc)
		{
			return ((*ppSampler = new DX11Sampler(this, pDesc)));;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
			return ((*ppTexture = new DX11Texture(this, pInitialData, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			return ((*ppBuffer = new DX11Buffer(this, pInitialData, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc)
		{
			return ((*ppRootLayout = new DX11RootLayout(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc)
		{
			return ((*ppPipelineState = new DX11PipelineState(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::Create(const DeviceDesc* pDesc)
		{
			if (FAILED(CreateDXGIFactory(IID_PPV_ARGS(&m_Factory))))
			{
				LOG_ERROR("DX11: Could not create factoiry");
				return;
			}

			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
			if (!QueryAdapter(&supportedFeatureLevel))
			{
				LOG_ERROR("D3D11: Failed to query adapter");
				return;
			}
			
			uint32 deviceFlags = 0;
			if (pDesc->DeviceFlags & DEVICE_FLAG_DEBUG)
			{
				deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
			}

			HRESULT hr = D3D11CreateDevice(m_Adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceFlags, &supportedFeatureLevel, 1, D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, nullptr);
			if (FAILED(hr))
			{
				LOG_ERROR("D3D11: Could not create Device and Immediate Context. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_pImmediateContext = new DX11DeviceContext(this, false);

				m_Desc = *pDesc;
			}

			if (pDesc->DeviceFlags & DEVICE_FLAG_DEBUG)
			{
				hr = m_Device->QueryInterface<ID3D11Debug>(&m_DebugDevice);
				if (FAILED(hr))
				{
					LOG_ERROR("D3D11: Could not create Debug Interface. " + DXErrorString(hr));
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::QueryAdapter(D3D_FEATURE_LEVEL* pFeatureLevel)
		{
			D3D_FEATURE_LEVEL supportedFeatureLevels[] =
			{
				D3D_FEATURE_LEVEL_12_1,
				D3D_FEATURE_LEVEL_12_0,
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_1,
			};

			int32 featureLevelCount = 9;
			int32 adapterCount = 0;
			HRESULT hr = 0;

			int32 bestAdapterIndex = -1;
			uint32 vendorID = 0;
			D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
			std::string adapterDesc;

			Microsoft::WRL::ComPtr<IDXGIAdapter> adapter = nullptr;
			for (int32 i = 0; m_Factory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				DXGI_ADAPTER_DESC desc = {};
				adapter->GetDesc(&desc);

#if defined(RE_DEBUG)
				constexpr int32 len = sizeof(desc.Description) / sizeof(WCHAR);
				char str[len];
				wcstombs(str, desc.Description, len);
				std::string description = str;

				LOG_INFO("D3D11: Adapter(" + std::to_string(i) + ") " + description);
#endif

				D3D_FEATURE_LEVEL fl;
				hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, 0, supportedFeatureLevels, featureLevelCount, D3D11_SDK_VERSION, nullptr, &fl, nullptr);
				if (hr == E_INVALIDARG)
				{
					LOG_WARNING("D3D11: Adapter " + std::to_string(i) + " does not support a D3D_FEATURE_LEVEL higher than D3D_FEATURE_LEVEL_11_0");

					D3D_FEATURE_LEVEL featureLevel_11_0 = D3D_FEATURE_LEVEL_11_0;
					hr = D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, 0, 0, &featureLevel_11_0, 1, D3D11_SDK_VERSION, nullptr, &fl, nullptr);
					if (FAILED(hr))
					{
						LOG_WARNING("D3D11: Adapter " + std::to_string(i) + " does not support D3D11.");
						continue;
					}
				}

				if ((desc.VendorId != 0x163C && desc.VendorId != 0x8086 && desc.VendorId != 0x8087 && desc.VendorId != 0x1414) ||
					(desc.VendorId == vendorID && fl > featureLevel) || 
					(bestAdapterIndex < 0))
				{
					bestAdapterIndex = i;
					featureLevel = fl;
					vendorID = desc.VendorId;
					adapterDesc = description;
				}

				adapterCount++;
			}

			if (adapterCount < 1 || bestAdapterIndex < 0)
			{
				return false;
			}
			else
			{
				if (FAILED(m_Factory->EnumAdapters(bestAdapterIndex, &m_Adapter)))
				{
					return false;
				}
			}

			LOG_INFO("D3D11: Chosen adapter " + adapterDesc);
			*pFeatureLevel = featureLevel;
			return true;
		}
	}
}

#endif