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
			VulkDevice(IFactory* pFactory, const DeviceInfo& deviceInfo);
			VulkDevice(VulkDevice&& other);
			~VulkDevice();

			IFactory* GetFactory() const;
			VkDevice GetVkDevice() const;
			VkPhysicalDevice GetVkPhysicalDevice() const;

			VulkDevice& operator=(VulkDevice&& other);
			
			bool CreateCommandQueue(ICommandQueue** ppCommandQueue, const CommandQueueInfo& info) override final;
			bool CreateShader(IShader** ppShader, const ShaderByteCode& byteCode) override final;
			bool CreateRenderTargetView(IRenderTargetView** ppView, const RenderTargetViewInfo& info) override final;
			bool CreateDepthStencilView(IDepthStencilView** ppView, const DepthStencilViewInfo& info) override final;
			bool CreateTexture(ITexture** ppTexture, const ResourceData* const pInitialData, const TextureInfo& info) override final;
			bool CreateBuffer(IBuffer** ppBuffer, const ResourceData* const pInitialData, const BufferInfo& info) override final;
			bool CreateRootSignature(IRootSignature** ppRootSignature, const RootSignatureInfo& info) override final;
			bool CreatePipelineState(IPipelineState** ppPipelineState, const PipelineStateInfo& info) override final;
			System::Log* GetDeviceLog() override final;

			IReferenceCounter* QueryReference() override final;
			uint32 GetReferenceCount() const override final;
			void Release() const override final;

		protected:
			uint32 AddRef() override final;

		private:
			void Create(IFactory* pFactory, const DeviceInfo& deviceInfo);

		private:
			IFactory* m_Factory;

			VkDevice m_Device;
			VkPhysicalDevice m_Adapter;
			System::Log m_Log;

			mutable uint32 m_ReferenceCount;
		};
	}
}