#pragma once

#include "..\Graphics\IDevice.h"
#include "VKSwapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VKDevice : public IDevice
		{
		public:
			VKDevice(const VKDevice& other) = delete;
			VKDevice& operator=(const VKDevice& other) = delete;

		public:
			VKDevice(VkInstance instance, const DeviceInfo& deviceInfo);
			VKDevice(VKDevice&& other);
			~VKDevice();

			bool CreateCommandQueue(ICommandQueue** commandQueue, const CommanQueueInfo& info) const override final;
			bool CreateShader(IShader** shader, const ShaderByteCode& byteCode) const override final;
			bool CreateRenderTargetView(IRenderTargetView** view, const RenderTargetViewInfo& info) const override final;
			bool CreateDepthStencilView(IDepthStencilView** view, const DepthStencilViewInfo& info) const override final;
			bool CreateTexture(ITexture** texture, const TextureInfo& info) const override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) const override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) const override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) const override final;

			VKDevice& operator=(VKDevice&& other);

			VKSwapchain* CreateVKSwapchain(VkSurfaceKHR surface);

			VkDevice GetVkDevice() const;

		private:
			void Create(VkInstance instance, const DeviceInfo& deviceInfo);

		private:
			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;
		};
	}
}