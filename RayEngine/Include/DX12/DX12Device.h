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

#pragma once
#include "..\Graphics\IDevice.h"

#if defined(RE_PLATFORM_WINDOWS)
#include "DX12Common.h"

#define QueryDX12Device(pDevice) reinterpret_cast<DX12Device*>(pDevice->QueryReference())

namespace RayEngine
{
	namespace Graphics
	{
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Factory;
		class DX12DeviceContext;
		class DX12DescriptorHeap;
		class DX12DynamicUploadHeap;


		//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DX12Device final : public IDevice
		{
			RE_IMPLEMENT_INTERFACE(DX12Device);

		public:
			DX12Device(const DeviceDesc* pDesc);
			~DX12Device();

			inline ID3D12Device* GetD3D12Device() const
			{
				return m_Device;
			}

			inline DX12DescriptorHeap* GetDX12DepthStencilViewHeap() const
			{
				return m_DsvHeap;
			}

			inline DX12DescriptorHeap* GetDX12RenderTargetViewHeap() const
			{
				return m_RtvHeap;
			}

			inline DX12DescriptorHeap* GetDX12ResourceHeap() const
			{
				return m_ResourceHeap;
			}

			inline DX12DescriptorHeap* GetDX12SamplerHeap() const
			{
				return m_SamplerHeap;
			}

			inline DX12DynamicUploadHeap* GetDX12UploadHeap() const
			{
				return m_UploadHeap;
			}

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			
			bool CreateShader(IShader** ppShader, const ShaderDesc* pDesc) override final;
			
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc* pDesc) override final;
			
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc* pDesc) override final;
			
			bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc* pDesc) override final;
			
			bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc* pDesc) override final;
			
			bool CreateSampler(ISampler** ppSampler, const SamplerDesc* pDesc) override final;
			
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc* pDesc) override final;
			
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc* pDesc) override final;
			
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc* pDesc) override final;
			
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc* pDesc) override final;
			
			void SetName(const std::string& name) override final;

			void GetDesc(DeviceDesc* pDesc) const override final;

			void GetAdapterDesc(AdapterDesc* pDesc) const override final;
			
			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

		private:
			void Create(const DeviceDesc* pDesc);

		private:
			IDXGIFactory5* m_Factory;
			IDXGIAdapter1* m_Adapter;
			ID3D12Device* m_Device;
			ID3D12Debug* m_DebugController;
			ID3D12DebugDevice* m_DebugDevice;
			DX12DeviceContext* m_ImmediateContext;
			DX12DynamicUploadHeap* m_UploadHeap;
			DX12DescriptorHeap* m_ResourceHeap;
			DX12DescriptorHeap* m_DsvHeap;
			DX12DescriptorHeap* m_RtvHeap;
			DX12DescriptorHeap* m_SamplerHeap;

			DeviceDesc m_Desc;

			IObject::CounterType m_References;
		};
	}
}

#endif