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
			DX12Device(IFactory* pFactory, const DeviceDesc& info, bool debugLayer);
			~DX12Device();

			ID3D12Device* GetD3D12Device() const;
			
			DX12DescriptorHeap* GetDX12DepthStencilViewHeap() const;
			
			DX12DescriptorHeap* GetDX12RenderTargetViewHeap() const;
			
			DX12DescriptorHeap* GetDX12ResourceHeap() const;
			
			DX12DescriptorHeap* GetDX12SamplerHeap() const;
			
			DX12DynamicUploadHeap* GetDX12UploadHeap() const;

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			
			bool CreateShader(IShader** ppShader, const ShaderDesc& info) override final;
			
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewDesc& info) override final;
			
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewDesc& info) override final;
			
			bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewDesc& info) override final;
			
			bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewDesc& info) override final;
			
			bool CreateSampler(ISampler** ppSampler, const SamplerDesc& info) override final;
			
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureDesc& info) override final;
			
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferDesc& info) override final;
			
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutDesc& info) override final;
			
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateDesc& info) override final;
			
			void SetName(const std::string& name) override final;
			
			void QueryFactory(IFactory** ppFactory) const override final;
			
			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;
			
			IObject::CounterType AddRef() override final;

			System::Log* GetDeviceLog() override final;

		private:
			void Create(IFactory* pFactory, const DeviceDesc& info, bool debugLayer);

		private:
			DX12Factory* mFactory;
			IDXGIAdapter1* m_Adapter;
			ID3D12Device* m_Device;
			ID3D12DebugDevice* m_DebugDevice;
			DX12DeviceContext* mImmediateContext;
			DX12DynamicUploadHeap* m_UploadHeap;
			DX12DescriptorHeap* m_ResourceHeap;
			DX12DescriptorHeap* m_DsvHeap;
			DX12DescriptorHeap* m_RtvHeap;
			DX12DescriptorHeap* m_SamplerHeap;

			mutable System::Log mLog;

			IObject::CounterType mReferences;
		};
	}
}

#endif