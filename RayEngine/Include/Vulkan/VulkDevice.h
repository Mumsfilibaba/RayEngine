#pragma once

#include "..\Graphics\IDevice.h"
#include "VulkSwapchain.h"

#define QueryVulkDevice(pDevice) reinterpret_cast<VulkDevice*>(pDevice->QueryReference())

namespace RayEngine
{
	namespace Graphics
	{
		class VulkDevice : public IDevice
		{
		public:
			VulkDevice(const VulkDevice& other) = delete;
			VulkDevice& operator=(const VulkDevice& other) = delete;
			VulkDevice(VulkDevice&& other) = delete;
			VulkDevice& operator=(VulkDevice&& other) = delete;

		public:
			VulkDevice(IFactory* pFactory, const DeviceInfo& deviceInfo);
			~VulkDevice();

			VkDevice GetVkDevice() const;
			VkPhysicalDevice GetVkPhysicalDevice() const;

			bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info) override final;
			bool CreateShader(IShader** ppShader, const ShaderInfo& info) override final;
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) override final;
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) override final;
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) override final;
			void QueryFactory(IFactory** ppFactory) const override final;
			System::Log* GetDeviceLog() override final;

		private:
			void Create(const DeviceInfo& deviceInfo);

		private:
			VulkFactory* m_Factory;
			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;
			System::Log m_Log;
		};
	}
}