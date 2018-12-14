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
#include <DX12/DX12Device.h>
#include <DX12/DX12Buffer.h>
#include <DX12/DX12Shader.h>
#include <DX12/DX12Texture.h>
#include <DX12/DX12Sampler.h>
#include <DX12/DX12RootLayout.h>
#include <DX12/DX12DeviceContext.h>
#include <DX12/DX12PipelineState.h>
#include <DX12/DX12DescriptorHeap.h>
#include <DX12/DX12RenderTargetView.h>
#include <DX12/DX12DepthStencilView.h>
#include <DX12/DX12DynamicUploadHeap.h>
#include <DX12/DX12ShaderResourceView.h>
#include <DX12/DX12UnorderedAccessView.h>
#include <DX12/DX12Renderer.h>

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Device::DX12Device(const DeviceDesc* pDesc)
			: m_Factory(nullptr),
			m_Adapter(nullptr),
			m_Device(nullptr),
			m_DebugController(nullptr),
			m_DebugDevice(nullptr),
			m_UploadHeap(nullptr),
			m_pImmediateContext(nullptr),
			m_ResourceHeap(nullptr),
			m_DsvHeap(nullptr),
			m_RtvHeap(nullptr),
			m_SamplerHeap(nullptr),
			m_References(0)
		{
			AddRef();
			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Device::~DX12Device()
		{
			ReRelease_S(m_Factory);
			ReRelease_S(m_DsvHeap);
			ReRelease_S(m_RtvHeap);
			ReRelease_S(m_ResourceHeap);
			ReRelease_S(m_pImmediateContext);
			ReRelease_S(m_UploadHeap);
			ReRelease_S(m_SamplerHeap);
			ReRelease_S(m_pEmptySRV);
			ReRelease_S(m_pEmptyRTV);
			ReRelease_S(m_pEmptyDSV);
			ReRelease_S(m_pEmptyUAV);

			if (m_DebugDevice != nullptr)
			{
				m_DebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_SUMMARY | D3D12_RLDO_DETAIL);
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateShader(IShader** ppShader, const ShaderDesc* pDesc)
		{
			return ((*ppShader = new DX12Shader(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc)
		{
			return ((*ppView = new DX12RenderTargetView(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc)
		{
			return (*ppView = new DX12DepthStencilView(this, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc)
		{
			return ((*ppView = new DX12ShaderResourceView(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc)
		{
			return ((*ppView = new DX12UnorderedAccessView(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc)
		{
			return ((*ppSampler = new DX12Sampler(this, pDesc)) != nullptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc)
		{
			return ((*ppTexture) = new DX12Texture(this, pInitialData, pDesc)) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc)
		{
			return ((*ppBuffer) = new DX12Buffer(this, pInitialData, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc)
		{
			return ((*ppRootLayout) = new DX12RootLayout(this, pDesc));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc)
		{
			return ((*ppPipelineState = new DX12PipelineState(this, pDesc)));
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Device::AddRef()
		{
			return ++m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CounterType DX12Device::Release()
		{
			CounterType counter = --m_References;
			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Device::CreateSamplerDescriptorHandle(const D3D12_SAMPLER_DESC& desc, DX12DescriptorHandle& destHandle) const
		{
			destHandle = m_SamplerHeap->GetNext();
			m_Device->CreateSampler(&desc, destHandle.CpuDescriptor);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IRenderer* DX12Device::CreateRenderer()
		{
			DX12Swapchain* ptr = nullptr;
			return new DX12Renderer(this, ptr);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::GetImmediateContext(IDeviceContext** ppContext)
		{
			return ((*ppContext) = m_pImmediateContext->QueryReference<DX12DeviceContext>()) != nullptr;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::CreateDefferedContext(IDeviceContext** ppContext)
		{
			return ((*ppContext) = new DX12DeviceContext(this, true)) != nullptr;
		}
	

		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Device::Create(const DeviceDesc* pDesc)
		{
			int32 factoryFlags = 0;
			if (pDesc->DeviceFlags & DEVICE_FLAG_DEBUG)
			{
				factoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
				if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_DebugController))))
				{
					LOG_ERROR("D3D12: Could not retrive the Debug interface");
					return;
				}
				else
				{
					m_DebugController->EnableDebugLayer();
				}
			}

			if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_Factory))))
			{
				LOG_ERROR("D3D12: Could not create factory");
				return;
			}

			if (!QueryAdapter())
			{
				LOG_ERROR("D3D12: Failed to query adapter");
				return;
			}

			DXGI_ADAPTER_DESC1 desc = {};
			m_Adapter->GetDesc1(&desc);

			LOG_INFO("Model: " + CompressString(desc.Description));

			HRESULT hr = D3D12CreateDevice(m_Adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
			if (SUCCEEDED(hr))
			{
				if (pDesc->DeviceFlags & DEVICE_FLAG_DEBUG)
				{
					hr = m_Device->QueryInterface<ID3D12DebugDevice>(&m_DebugDevice);
					if (FAILED(hr))
					{
						LOG_ERROR("D3D12: Could not create DebugDevice. " + DXErrorString(hr));
						return;
					}
				}

				m_UploadHeap = new DX12DynamicUploadHeap(this, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 20);
				m_DsvHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, pDesc->DepthStencilDescriptorCount);
				m_RtvHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, pDesc->RendertargetDescriptorCount);
				m_ResourceHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, pDesc->ResourceDescriptorCount);
				m_SamplerHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, pDesc->SamplerDescriptorCount);
				m_pImmediateContext = new DX12DeviceContext(this, false);

				CreateEmptyDescriptors();
			}
			else
			{
				LOG_ERROR("D3D12: Could not create Device. " + DXErrorString(hr));
			}
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::QueryAdapter()
		{
			HRESULT hr = 0;
			int32 adapterCount = 0;
			int32 bestAdapterIndex = -1;
			uint32 vendorID = 0;

			Microsoft::WRL::ComPtr<IDXGIAdapter1> adapter = nullptr;
			for (int32 i = 0; m_Factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
			{
				DXGI_ADAPTER_DESC1 desc = {};
				adapter->GetDesc1(&desc);

				if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)))
				{
					//We skip the intel once for now
					if ((desc.VendorId != 0x163C && desc.VendorId != 0x8086 && desc.VendorId != 0x8087) ||
						(bestAdapterIndex < 0))
					{
						bestAdapterIndex = i;
						vendorID = desc.VendorId;
					}

					adapterCount++;
				}
				else
				{
					LOG_WARNING("D3D12: Adapter " + std::to_string(i) + " does not support D3D12");
				}
			}

			if (adapterCount < 1 || bestAdapterIndex < 0)
			{
				return false;
			}
			else
			{
				if (FAILED(m_Factory->EnumAdapters1(bestAdapterIndex, &m_Adapter)))
				{
					return false;
				}
			}
			return true;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Device::CreateEmptyDescriptors()
		{
			ShaderResourceViewDesc srv = {};
			srv.Flags = SHADER_RESOURCE_VIEW_FLAG_NONE;
			srv.Format = FORMAT_R8G8B8A8_UNORM;
			srv.pResource = nullptr;
			srv.ViewDimension = VIEWDIMENSION_TEXTURE2D;
			srv.Texture2D.MinLODClamp = 0.0f;
			srv.Texture2D.MipLevels = 0;
			srv.Texture2D.MostDetailedMip = 0;
			srv.Texture2D.PlaneSlice = 0;
			
			m_pEmptySRV = new DX12ShaderResourceView(this, &srv);

			RenderTargetViewDesc rtv = {};
			rtv.Format = FORMAT_R8G8B8A8_UNORM;
			rtv.pResource = nullptr;
			rtv.ViewDimension = VIEWDIMENSION_TEXTURE2D;
			rtv.Texture2D.MipSlice = 0;
			rtv.Texture2D.PlaneSlice = 0;
			
			m_pEmptyRTV = new DX12RenderTargetView(this, &rtv);

			DepthStencilViewDesc dsv = {};
			dsv.Format = FORMAT_D24_UNORM_S8_UINT;
			dsv.Flags = DEPTH_STENCIL_VIEW_FLAGS_NONE;
			dsv.pResource = nullptr;
			dsv.ViewDimension = VIEWDIMENSION_TEXTURE2D;
			dsv.Texture2D.MipSlice = 0;

			m_pEmptyDSV = new DX12DepthStencilView(this, &dsv);

			UnorderedAccessViewDesc uav = {};
			uav.Format = FORMAT_R8G8B8A8_UNORM;
			uav.Flags = UNORDERED_ACCESS_VIEW_FLAG_NONE;
			uav.pResource = nullptr;
			uav.pCounterResource = nullptr;
			uav.ViewDimension = VIEWDIMENSION_TEXTURE2D;
			uav.Texture2D.MipSlice = 0;
			uav.Texture2D.PlaneSlice = 0;

			m_pEmptyUAV = new DX12UnorderedAccessView(this, &uav);
		}
	}
}

#endif