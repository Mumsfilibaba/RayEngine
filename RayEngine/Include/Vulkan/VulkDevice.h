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
			VulkDevice(IFactory* pFactory, const DeviceDesc* pDesc);
			~VulkDevice();

			inline VkDevice GetVkDevice() const
			{
				return m_Device;
			}
			
			inline VkPhysicalDevice GetVkPhysicalDevice() const
			{
				return m_Adapter;
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
			
			void QueryFactory(IFactory** ppFactory) const override final;

			void GetDesc(DeviceDesc* pDesc) const override final;

			IObject::CounterType GetReferenceCount() const override final;
			
			IObject::CounterType Release() override final;

			IObject::CounterType AddRef() override final;

		private:
			void Create(const DeviceDesc* pDesc);

		private:
			VulkFactory* m_Factory;
			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;
			
			DeviceDesc m_Desc;

			IObject::CounterType m_References;
		};
	}
}