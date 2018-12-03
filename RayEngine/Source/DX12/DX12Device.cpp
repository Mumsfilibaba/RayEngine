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

#include "../../Include/Debug/Debug.h"
#include "../../Include/DX12/DX12Device.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "../../Include/DX12/DX12Factory.h"
#include "../../Include/DX12/DX12Buffer.h"
#include "../../Include/DX12/DX12Shader.h"
#include "../../Include/DX12/DX12Texture.h"
#include "../../Include/DX12/DX12DeviceContext.h"
#include "../../Include/DX12/DX12PipelineState.h"
#include "../../Include/DX12/DX12Sampler.h"
#include "../../Include/DX12/DX12ShaderResourceView.h"
#include "../../Include/DX12/DX12UnorderedAccessView.h"
#include "../../Include/DX12/DX12RootLayout.h"
#include "../../Include/DX12/DX12DepthStencilView.h"
#include "../../Include/DX12/DX12RenderTargetView.h"
#include "../../Include/DX12/DX12DynamicUploadHeap.h"
#include "../../Include/DX12/DX12DescriptorHeap.h"

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
			m_ImmediateContext(nullptr),
			m_ResourceHeap(nullptr),
			m_DsvHeap(nullptr),
			m_RtvHeap(nullptr),
			m_SamplerHeap(nullptr),
			m_Desc(),
			m_References(0)
		{
			AddRef();
			
			m_UploadHeap = new DX12DynamicUploadHeap(this, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT, D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT * 20);
			m_UploadHeap->SetName(pDesc->Name + ": Dynamic Upload-Heap");

			Create(pDesc);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		DX12Device::~DX12Device()
		{
			ReRelease_S(m_Factory);
			ReRelease_S(m_DsvHeap);
			ReRelease_S(m_RtvHeap);
			ReRelease_S(m_ResourceHeap);
			ReRelease_S(m_ImmediateContext);
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
		void DX12Device::SetName(const std::string& name)
		{
			D3D12SetName(m_Device, name);
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Device::GetDesc(DeviceDesc* pDesc) const
		{
			*pDesc = m_Desc;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DX12Device::GetAdapterDesc(AdapterDesc* pDesc) const
		{
			DXGI_ADAPTER_DESC1 desc = {};
			m_Adapter->GetDesc1(&desc);

			constexpr int32 len = sizeof(desc.Description) / sizeof(WCHAR);
			char str[len];
			wcstombs(str, desc.Description, len);

			pDesc->ModelName = str;
			pDesc->VendorName = AdapterDesc::GetVendorString(desc.VendorId);
			pDesc->VendorID = desc.VendorId;
			pDesc->DeviceID = desc.DeviceId;

			pDesc->Flags |= ADAPTER_FLAGS_SWAPCHAIN;
			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
				pDesc->Flags |= ADAPTER_FLAGS_SOFTWARE;


			//These are constants for D3D_FEATURE_LEVEL_11_0 the lowest level RayEngine supports
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
		IObject::CounterType DX12Device::GetReferenceCount() const
		{
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Device::AddRef()
		{
			m_References++;
			return m_References;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		IObject::CounterType DX12Device::Release()
		{
			m_References--;
			IObject::CounterType counter = m_References;

			if (counter < 1)
				delete this;

			return counter;
		}


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bool DX12Device::GetImmediateContext(IDeviceContext** ppContext)
		{
			return ((*ppContext) = m_ImmediateContext->QueryReference<DX12DeviceContext>()) != nullptr;
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
					LOG_ERROR("DX12: Could not retrive the Debug interface");
					return;
				}
				else
				{
					m_DebugController->EnableDebugLayer();
				}
			}

			if (FAILED(CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&m_Factory))))
			{
				LOG_ERROR("DX12: Could not create factory");
				return;
			}

			HRESULT hr = m_Factory->EnumAdapters1(0, &m_Adapter);
			if (SUCCEEDED(hr))
			{
				hr = D3D12CreateDevice(m_Adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_Device));
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


					m_DsvHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, pDesc->DepthStencilDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_DsvHeap->SetName(pDesc->Name + ": DSV-Heap");

					m_RtvHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, pDesc->RendertargetDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_NONE);
					m_RtvHeap->SetName(pDesc->Name + ": RTV-Heap");

					m_ResourceHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, pDesc->ResourceDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
					m_ResourceHeap->SetName(pDesc->Name + ": Resource-Heap (CBV/SRV)");

					m_SamplerHeap = new DX12DescriptorHeap(this, D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER, pDesc->SamplerDescriptorCount, D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE);
					m_SamplerHeap->SetName(pDesc->Name + ": Sampler-Heap");

					m_ImmediateContext = new DX12DeviceContext(this, false);

					D3D12SetName(m_Device, pDesc->Name);

					m_Desc = *pDesc;
				}
				else
				{
					LOG_ERROR("D3D12: Could not create Device. " + DXErrorString(hr));
				}
			}
			else
			{
				LOG_ERROR("D3D12: Could not enumerate adapters. " + DXErrorString(hr));
			}
		}
	}
}

#endif