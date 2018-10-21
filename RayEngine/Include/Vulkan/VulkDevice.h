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
#include "VulkSwapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkDevice final : public IDevice
		{
			RE_IMPLEMENT_INTERFACE(VulkDevice);

		public:
			VulkDevice(IFactory* pFactory, const DeviceDesc& deviceInfo);
			~VulkDevice();

			VkDevice GetVkDevice() const;
			
			VkPhysicalDevice GetVkPhysicalDevice() const;

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
			void Create(const DeviceDesc& deviceInfo);

		private:
			VulkFactory* mFactory;
			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;
			
			IObject::CounterType mReferences;
			
			System::Log mLog;
		};
	}
}