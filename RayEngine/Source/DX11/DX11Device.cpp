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
		DX11Device::DX11Device(IFactory* pFactory, const DeviceDesc& info, bool debugLayer)
			: mFactory(nullptr),
			m_Adapter(nullptr),
			m_Device(nullptr),
			m_DebugDevice(nullptr),
			mImmediateContext(nullptr),
			m_FeatureLevel(),
			mReferences(0)
		{
			AddRef();
			mFactory = pFactory->QueryReference<DX11Factory>();
			
			Create(pFactory, info, debugLayer);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX11Device::~DX11Device()
		{
			ReRelease_S(mImmediateContext);
			ReRelease_S(mFactory);
			
			D3DRelease_S(m_Adapter);
			D3DRelease_S(m_Device);

			if (m_DebugDevice != nullptr)
			{
				m_DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_SUMMARY);
				D3DRelease(m_DebugDevice);
			}
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ID3D11Device* DX11Device::GetD3D11Device() const
		{
			return m_Device;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::GetImmediateContext(IDeviceContext** ppContext)
		{
			if (ppContext == nullptr)
				return false;

			(*ppContext) = mImmediateContext->QueryReference<DX11DeviceContext>();
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
			(*ppFactory) = mFactory->QueryReference<DX11Factory>();
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Device::GetReferenceCount() const
		{
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Device::AddRef()
		{
			mReferences++;
			return mReferences;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX11Device::Release()
		{
			mReferences--;
			IObject::CounterType counter = mReferences;

			if (counter < 1)
				delete this;

			return counter;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateShader(IShader** ppShader, const ShaderDesc& info)
		{
			return ((*ppShader = new DX11Shader(this, info)));
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc& info)
		{
			return ((*ppView = new DX11RenderTargetView(this, info)) != nullptr);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc& info)
		{
			return ((*ppView = new DX11DepthStencilView(this, info)) != nullptr);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc& info)
		{
			return ((*ppView = new DX11ShaderResourceView(this, info)) != nullptr);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc& info)
		{
			return ((*ppView = new DX11UnorderedAccessView(this, info)) != nullptr);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateSampler(ISampler** ppSampler, const SamplerDesc& info)
		{
			return ((*ppSampler = new DX11Sampler(this, info)));;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc& info)
		{
			return ((*ppTexture = new DX11Texture(this, pInitialData, info)) != nullptr);
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc& info)
		{
			return ((*ppBuffer = new DX11Buffer(this, pInitialData, info)));
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc& info)
		{
			return ((*ppRootLayout = new DX11RootLayout(this, info)));
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX11Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc& info)
		{
			return ((*ppPipelineState = new DX11PipelineState(this, info)));
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		System::Log* DX11Device::GetDeviceLog()
		{
			return &mLog;
		}



		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX11Device::Create(IFactory* pFactory, const DeviceDesc& info, bool debugLayer)
		{
			using namespace System;

			IDXGIFactory* pDXGIFactory = reinterpret_cast<DX11Factory*>(pFactory)->GetDXGIFactory();
			HRESULT hr = pDXGIFactory->EnumAdapters(info.pAdapter->ApiID, &m_Adapter);
			if (FAILED(hr))
			{
				mLog.Write(LOG_SEVERITY_ERROR, "D3D11: Could not retrive adapter. " + DXErrorString(hr));
				return;
			}


			uint32 deviceFlags = 0;
			if (debugLayer)
				deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;

			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(m_Adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceFlags, &supportedFeatureLevel, 1,
				D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, nullptr);
			if (FAILED(hr))
			{
				mLog.Write(LOG_SEVERITY_ERROR, "D3D11: Could not create Device and Immediate Context. " + DXErrorString(hr));
				return;
			}
			else
			{
				m_Device->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());

				mImmediateContext = new DX11DeviceContext(this, false);
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