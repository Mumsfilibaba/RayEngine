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
			VulkDevice(IFactory* pFactory, const DeviceInfo& deviceInfo);
			~VulkDevice();

			VkDevice GetVkDevice() const;
			
			VkPhysicalDevice GetVkPhysicalDevice() const;

			bool GetImmediateContext(IDeviceContext** ppContext) override final;
			
			bool CreateDefferedContext(IDeviceContext** ppContext) override final;
			
			bool CreateShader(IShader** ppShader, const ShaderInfo& info) override final;
			
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) override final;
			
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) override final;
			
			bool CreateShaderResourceView(IShaderResourceView** ppView, const ShaderResourceViewInfo& info) override final;
			
			bool CreateUnorderedAccessView(IUnorderedAccessView** ppView, const UnorderedAccessViewInfo& info) override final;
			
			bool CreateSampler(ISampler** ppSampler, const SamplerInfo& info) override final;
			
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) override final;
			
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) override final;
			
			bool CreateRootLayout(IRootLayout** ppRootLayout, const RootLayoutInfo& info) override final;
			
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) override final;

			void SetName(const std::string& name) override final;
			
			void QueryFactory(IFactory** ppFactory) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

			System::Log* GetDeviceLog() override final;

		private:
			void Create(const DeviceInfo& deviceInfo);

		private:
			VulkFactory* mFactory;
			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;
			
			IObject::CounterType mReferences;
			
			System::Log mLog;
		};
	}
}