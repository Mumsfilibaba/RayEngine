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

#include "..\..\Include\DX11\DX11Device.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Factory.h"
#include "..\..\Include\DX11\DX11DeviceContext.h"
#include "..\..\Include\DX11\DX11RenderTargetView.h"
#include "..\..\Include\DX11\DX11DepthStencilView.h"
#include "..\..\Include\DX11\DX11Texture.h"
#include "..\..\Include\DX11\DX11Shader.h"
#include "..\..\Include\DX11\DX11Sampler.h"
#include "..\..\Include\DX11\DX11ShaderResourceView.h"
#include "..\..\Include\DX11\DX11UnorderedAccessView.h"
#include "..\..\Include\DX11\DX11RootLayout.h"
#include "..\..\Include\DX11\DX11PipelineState.h"
#include "..\..\Include\DX11\DX11Buffer.h"
#include <d3dcommon.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Device::DX11Device(IFactory* pFactory, const DeviceDesc* pDesc, bool debugLayer)
			: m_Factory(nullptr),
			m_Adapter(nullptr),
			m_Device(nullptr),
			m_DebugDevice(nullptr),
			m_ImmediateContext(nullptr),
			m_FeatureLevel(),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			m_Factory = pFactory->QueryReference<DX11Factory>();
			
			Create(pFactory, pDesc, debugLayer);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Device::~DX11Device()
		{
			ReRelease_S(m_ImmediateContext);
			ReRelease_S(m_Factory);
			
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

			(*ppContext) = m_ImmediateContext->QueryReference<DX11DeviceContext>();
			return (*ppContext) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return ((*ppContext) = new DX11DeviceContext(this, true));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::SetName(const std::string& name)
		{
			m_Device->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(name.size()), name.c_str());
			
			std::string adapterName = name + " : Adapter";
			m_Adapter->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(adapterName.size()), adapterName.c_str());
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = m_Factory->QueryReference<DX11Factory>();
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::GetDesc(DeviceDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Device::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Device::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Device::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

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
		System::Log* DX11Device::GetDeviceLog()
		{
			return &mLog;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::Create(IFactory* pFactory, const DeviceDesc* pDesc, bool debugLayer)
		{
			using namespace System;

			IDXGIFactory* pDXGIFactory = reinterpret_cast<DX11Factory*>(pFactory)->GetDXGIFactory();
			HRESULT hr = pDXGIFactory->EnumAdapters(pDesc->pAdapter->ApiID, &m_Adapter);
			if (FAILED(hr))
			{
				mLog.Write(LOG_SEVERITY_ERROR, "D3D11: Could not retrive adapter. " + DXErrorString(hr));
				return;
			}


			uint32 deviceFlags = 0;
			if (debugLayer)
				deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(m_Adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceFlags, &supportedFeatureLevel, 1, D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, nullptr);
			if (FAILED(hr))
			{
				mLog.Write(LOG_SEVERITY_ERROR, "D3D11: Could not create Device and Immediate Context. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_Device->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(pDesc->Name.size()), pDesc->Name.c_str());

				m_ImmediateContext = new DX11DeviceContext(this, false);

				m_Desc = *pDesc;
			}


			if (debugLayer)
			{
				hr = m_Device->QueryInterface<ID3D11Debug>(&m_DebugDevice);
				if (FAILED(hr))
				{
					mLog.Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DebugDevice. " + DXErrorString(hr));
				}
			}
		}
	}
}

#endif