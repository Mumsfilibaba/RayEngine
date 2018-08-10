#include "..\..\Include\DX12\DX12Device.h"
#include "..\..\Include\DX12\DX12CommandQueue.h"
#include "..\..\Include\DX12\DX12Shader.h"
#include "..\..\Include\DX12\DX12RenderTargetView.h"
#include "..\..\Include\DX12\DX12DepthStencilView.h"
#include "..\..\Include\DX12\DX12RootSignature.h"
#include "..\..\Include\DX12\DX12PipelineState.h"
#include "..\..\Include\DX12\DX12Texture.h"
#include "..\..\Include\DX12\DX12Buffer.h"
#include "..\..\Include\DX12\DX12Factory.h"

#if defined(RE_PLATFORM_WINDOWS)

namespace RayEngine
{
	namespace Graphics
	{
		/////////////////////////////////////////////////////////////
		DX12Device::DX12Device(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
			: m_Factory(nullptr),
			m_Adapter(nullptr),
			m_Device(nullptr),
			m_DebugDevice(nullptr),
			m_UploadHeap(nullptr),
			m_UploadQueue(nullptr),
			m_ResourceHeap(nullptr),
			m_DsvHeap(nullptr),
			m_RtvHeap(nullptr),
			m_SamplerHeap(nullptr)
		{
			AddRef();
			m_Factory = reinterpret_cast<DX12Factory*>(pFactory->QueryReference());
			
			m_UploadHeap = new DX12DynamicUploadHeap(this, info.Name + ": Dynamic Upload-Heap", D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 20);

			Create(pFactory, info, debugLayer);
		}



		/////////////////////////////////////////////////////////////
		DX12Device::~DX12Device()
		{
			ReRelease_S(m_Factory);
			ReRelease_S(m_DsvHeap);
			ReRelease_S(m_RtvHeap);
			ReRelease_S(m_ResourceHeap);
			ReRelease_S(m_UploadQueue);
			ReRelease_S(m_UploadHeap);
			ReRelease_S(m_SamplerHeap);

			D3DRelease_S(m_Device);
			D3DRelease_S(m_Adapter);

			if (m_DebugDevice != nullptr)
			{
				m_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL);
				D3DRelease(m_DebugDevice);
			}
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info)
		{
			return ((*ppCommandQueue = new DX12CommandQueue(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateShader(IShader** ppShader, const ShaderInfo& info)
		{
			return ((*ppShader = new DX12Shader(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info)
		{
			return ((*ppView = new DX12RenderTargetView(this, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info)
		{
			return (*ppView = new DX12DepthStencilView(this, info));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info)
		{
			return ((*ppTexture = new DX12Texture(this, pInitialData, info)) != nullptr);
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info)
		{
			return ((*ppBuffer = new DX12Buffer(this, pInitialData, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info)
		{
			return ((*ppRootSignature = new DX12RootSignature(this, info)));
		}



		/////////////////////////////////////////////////////////////
		bool DX12Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info)
		{
			return ((*ppPipelineState = new DX12PipelineState(this, info)));
		}



		/////////////////////////////////////////////////////////////
		void DX12Device::QueryFactory(IFactory** ppFactory) const
		{
			(*ppFactory) = reinterpret_cast<DX12Factory*>(m_Factory->QueryReference());
		}



		/////////////////////////////////////////////////////////////
		System::Log* DX12Device::GetDeviceLog()
		{
			return &m_Log;
		}


		/////////////////////////////////////////////////////////////
		ID3D12Device* DX12Device::GetD3D12Device() const
		{
			return m_Device;
		}



		/////////////////////////////////////////////////////////////
		DX12CommandQueue* DX12Device::GetDX12CommandQueue() const
		{
			return m_UploadQueue;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap* DX12Device::GetDX12DepthStencilViewHeap() const
		{
			return m_DsvHeap;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap* DX12Device::GetDX12RenderTargetViewHeap() const
		{
			return m_RtvHeap;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap* DX12Device::GetDX12ResourceHeap() const
		{
			return m_ResourceHeap;
		}



		/////////////////////////////////////////////////////////////
		DX12DescriptorHeap* DX12Device::GetDX12SamplerHeap() const
		{
			return m_SamplerHeap;
		}



		/////////////////////////////////////////////////////////////
		DX12DynamicUploadHeap* DX12Device::GetDX12UploadHeap() const
		{
			return m_UploadHeap;
		}
		


		/////////////////////////////////////////////////////////////
		void DX12Device::Create(IFactory* pFactory, const DeviceInfo& info, bool debugLayer)
		{
			using namespace System;

			IDXGIFactory5* pDXGIFactory = reinterpret_cast<DX12Factory*>(pFactory)->GetDXGIFactory();
			HRESULT hr = pDXGIFactory->EnumAdapters1(info.pAdapter->ApiID, &m_Adapter);
			if (SUCCEEDED(hr))
			{
				hr = D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
				if (SUCCEEDED(hr))
				{
					if (debugLayer)
					{
						hr = m_Device->QueryInterface<ID3D12DebugDevice>(&m_DebugDevice);
						if (FAILED(hr))
						{
							m_Log.Write(LOG_SEVERITY_ERROR, "D3D12: Could not create DebugDevice. " + DXErrorString(hr));
							return;
						}
					}


					m_DsvHeap = new DX12DescriptorHeap(this, info.Name + ": DSV-Heap", D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 8, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_RtvHeap = new DX12DescriptorHeap(this, info.Name + ": RTV-Heap", D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 64, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_ResourceHeap = new DX12DescriptorHeap(this, info.Name + ": Resource-Heap (CBV/SRV)", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 128, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
					m_SamplerHeap = new DX12DescriptorHeap(this, info.Name + ": Sampler-Heap", D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, 128, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);


					CommandQueueInfo queueInfo = {};
					queueInfo.Name = "Device UploadQueue";
					m_UploadQueue = new DX12CommandQueue(this, queueInfo);
					

					D3D12SetName(m_Device, info.Name);
				}
				else
				{
					m_Log.Write(LOG_SEVERITY_ERROR, "D3D12: Could not create Device. " + DXErrorString(hr));
				}
			}
			else
			{
				m_Log.Write(LOG_SEVERITY_ERROR, "D3D12: Could not enumerate adapters. " + DXErrorString(hr));
			}
		}
	}
}

#endif