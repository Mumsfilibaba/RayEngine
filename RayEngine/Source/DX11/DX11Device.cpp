#include "..\..\Include\DX11\DX11Device.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "..\..\Include\DX11\DX11Factory.h"
#include "..\..\Include\DX11\DX11CommandQueue.h"
#include "..\..\Include\DX11\DX11RenderTargetView.h"
#include "..\..\Include\DX11\DX11Texture.h"
#include "..\..\Include\DX11\DX11DepthStencilView.h"
#include "..\..\Include\DX11\DX11Shader.h"
#include "..\..\Include\DX11\DX11RootSignature.h"
#include "..\..\Include\DX11\DX11PipelineState.h"
#include "..\..\Include\DX11\DX11Buffer.h"
#include <d3dcommon.h>

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX11Device::DX11Device(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
			: m_Factory(nullptr),
			m_Adapter(nullptr),
			m_Device(nullptr),
			m_DebugDevice(nullptr),
			m_ImmediateContext(nullptr),
			m_ReferenceCount(0),
			m_FeatureLevel()
		{
			AddRef();
			m_Factory = reinterpret_cast<IFactory*>(pFactory->QueryReference());
			
			Create(pFactory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX11Device::DX11Device(DX11Device&& other)
			: m_Factory(other.m_Factory),
			m_Adapter(other.m_Adapter),
			m_Device(other.m_Device),
			m_DebugDevice(other.m_DebugDevice),
			m_ImmediateContext(other.m_ImmediateContext),
			m_ReferenceCount(other.m_ReferenceCount),
			m_FeatureLevel(other.m_FeatureLevel)
		{
			other.m_Factory = nullptr;
			other.m_Adapter = nullptr;
			other.m_Device = nullptr;
			other.m_DebugDevice = nullptr;
			other.m_ImmediateContext = nullptr;
			other.m_ReferenceCount = 0;
		}



		/////////////////////////////////////////////////////////////
		DX11Device::~DX11Device()
		{
			D3DRelease_S(m_Adapter);
			D3DRelease_S(m_Device);
			D3DRelease_S(m_ImmediateContext);

			if (m_Factory != nullptr)
			{
				m_Factory->Release();
				m_Factory = nullptr;
			}

			m_DebugDevice->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_SUMMARY);
			D3DRelease_S(m_DebugDevice);
		}



		/////////////////////////////////////////////////////////////
		DX11Device& DX11Device::operator=(DX11Device&& other)
		{
			if (this != &other)
			{
				D3DRelease_S(m_Adapter);
				D3DRelease_S(m_Device);
				D3DRelease_S(m_DebugDevice);
				D3DRelease_S(m_ImmediateContext);

				if (m_Factory != nullptr)
				{
					m_Factory->Release();
					m_Factory = nullptr;
				}


				m_Factory = other.m_Factory;
				m_Adapter = other.m_Adapter;
				m_Device = other.m_Device;
				m_DebugDevice = other.m_DebugDevice;
				m_ImmediateContext = other.m_ImmediateContext;
				m_ReferenceCount = other.m_ReferenceCount;
				m_FeatureLevel = other.m_FeatureLevel;


				other.m_Factory = nullptr;
				other.m_Adapter = nullptr;
				other.m_Device = nullptr;
				other.m_DebugDevice = nullptr;
				other.m_ImmediateContext = nullptr;
				other.m_ReferenceCount = 0;
			}

			return *this;
		}



		/////////////////////////////////////////////////////////////
		ID3D11Device* DX11Device::GetD3D11Device() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		ID3D11DeviceContext* DX11Device::GetD3D11DeviceContext() const
		{
			return m_ImmediateContext;
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info)
		{
			return ((*ppCommandQueue = new DX11CommandQueue(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateShader(IShader** ppShader, const ShaderByteCode& byteCode)
		{
			return ((*ppShader = new DX11Shader(this, byteCode)));
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info)
		{
			return ((*ppView = new DX11RenderTargetView(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info)
		{
			return ((*ppView = new DX11DepthStencilView(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			return ((*ppTexture = new DX11Texture(this, pInitialData, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info)
		{
			return ((*ppBuffer = new DX11Buffer(this, pInitialData, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info)
		{
			return ((*ppRootSignature = new DX11RootSignature(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX11Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return ((*ppPipelineState = new DX11PipelineState(this, info)));
		}



		/////////////////////////////////////////////////////////////
		System::Log* DX11Device::GetDeviceLog()
		{
			return &m_Log;
		}



		/////////////////////////////////////////////////////////////
		IFactory* DX11Device::GetFactory() const
		{
			return m_Factory;
		}



		/////////////////////////////////////////////////////////////
		IReferenceCounter* DX11Device::QueryReference()
		{
			AddRef();
			return this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Device::GetReferenceCount() const
		{
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Device::Release() const
		{
			m_ReferenceCount--;
			if (m_ReferenceCount < 1)
				delete this;
		}



		/////////////////////////////////////////////////////////////
		uint32 DX11Device::AddRef()
		{
			m_ReferenceCount++;
			return m_ReferenceCount;
		}



		/////////////////////////////////////////////////////////////
		void DX11Device::Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
		{
			using namespace System;

			IDXGIFactory* pDXGIFactory = reinterpret_cast<DX11Factory*>(pFactory)->GetDXGIFactory();
			HRESULT hr = pDXGIFactory->EnumAdapters(info.pAdapter->ApiID, &m_Adapter);
			if (FAILED(hr))
			{
				m_Log.Write(LOG_SEVERITY_ERROR, "D3D11: Could not retrive adapter. " + DXErrorString(hr));
				return;
			}


			uint32 deviceFlags = 0;
			if (debugLayer)
				deviceFlags = D3D11_CREATE_DEVICE_DEBUG;

			D3D_FEATURE_LEVEL supportedFeatureLevel = D3D_FEATURE_LEVEL_11_0;
			hr = D3D11CreateDevice(m_Adapter, D3D_DRIVER_TYPE_UNKNOWN, 0, deviceFlags, &supportedFeatureLevel, 1,
				D3D11_SDK_VERSION, &m_Device, &m_FeatureLevel, &m_ImmediateContext);
			if (FAILED(hr))
			{
				m_Log.Write(LOG_SEVERITY_ERROR, "D3D11: Could not create Device and Immediate Context. " + DXErrorString(hr));
				return;
			}


			m_Device->SetPrivateData(WKPDID_D3DDebugObjectName, static_cast<uint32>(info.Name.size()), info.Name.c_str());
			if (debugLayer)
			{
				hr = m_Device->QueryInterface<ID3D11Debug>(&m_DebugDevice);
				if (FAILED(hr))
				{
					m_Log.Write(LOG_SEVERITY_ERROR, "D3D11: Could not create DebugDevice. " + DXErrorString(hr));
					return;
				}
			}

			return;
		}
	}
}

#endif