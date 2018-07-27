#pragma once

#include "..\Graphics\IDevice.h"
#include "VulkSwapchain.h"

namespace RayEngine
{
	namespace Graphics
	{
		class VulkDevice : public IDevice
		{
		public:
			VulkDevice(const VulkDevice& other) = delete;
			VulkDevice& operator=(const VulkDevice& other) = delete;

		public:
			VulkDevice(IFactory* pInstance, const DeviceInfo& deviceInfo);
			VulkDevice(VulkDevice&& other);
			~VulkDevice();

			bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info) const override final;
			bool CreateShader(IShader** ppShader, const ShaderByteCode& byteCode) const override final;
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) const override final;
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) const override final;
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) const override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) const override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) const override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) const override final;
			System::Log* GetDeviceLog() const override final;

			VulkDevice& operator=(VulkDevice&& other);

			VKSwapchain* CreateVKSwapchain(VkSurfaceKHR surface);

			VkDevice GetVkDevice() const;

			IFactory* GetFactory() const;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(VkInstance instance, const DeviceInfo& deviceInfo);

		private:
			IFactory* m_Facory;

			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;

			mutable uint32 m_ReferenceCount;
		};
	}
}